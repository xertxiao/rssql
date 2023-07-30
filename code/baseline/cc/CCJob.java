/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package tcExample;

import com.codahale.metrics.ConsoleReporter;
import org.apache.commons.collections.ListUtils;
import org.apache.flink.api.common.eventtime.TimestampAssigner;
import org.apache.flink.api.common.eventtime.WatermarkStrategy;
import org.apache.flink.api.common.functions.FilterFunction;
import org.apache.flink.api.common.functions.JoinFunction;
import org.apache.flink.api.common.functions.ReduceFunction;
import org.apache.flink.api.common.functions.RichMapFunction;
import org.apache.flink.configuration.Configuration;
import org.apache.flink.dropwizard.metrics.DropwizardMeterWrapper;
import org.apache.flink.metrics.Meter;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.datastream.IterativeStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.api.functions.sink.SinkFunction;
import org.apache.flink.streaming.api.functions.windowing.AllWindowFunction;
import org.apache.flink.streaming.api.functions.windowing.WindowFunction;
import org.apache.flink.streaming.api.windowing.assigners.GlobalWindows;
import org.apache.flink.streaming.api.windowing.assigners.SlidingEventTimeWindows;
import org.apache.flink.streaming.api.windowing.assigners.TumblingEventTimeWindows;
import org.apache.flink.streaming.api.windowing.assigners.TumblingProcessingTimeWindows;
import org.apache.flink.streaming.api.windowing.time.Time;
import org.apache.flink.api.java.functions.KeySelector;
import org.apache.flink.streaming.api.windowing.windows.TimeWindow;
import org.apache.flink.streaming.api.windowing.windows.Window;
import org.apache.flink.util.Collector;
import org.apache.flink.walkthrough.common.entity.Transaction;
import tcExample.common.entity.Arc;

import tcExample.common.entity.ArcSlide;
import tcExample.common.sink.ArcSink;
import tcExample.common.source.ArcSource;
import tcExample.metadata.qbi.EDBQbi;
import tcExample.metadata.qbi.IDBQbi;
import tcExample.metadata.qbi.Slide;
import tcExample.windowfunc.CCWindowFuncWithOutQbi;
import tcExample.windowfunc.CCWindowFuncWithQbi;
import tcExample.windowfunc.PRWindowFuncWithOutQbi;
import tcExample.windowfunc.PRWindowFuncWithQbi;

import java.sql.Timestamp;
import java.time.Duration;
import java.util.*;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static java.lang.Math.max;
import static java.lang.Math.min;


/**
	Code for Transitive Closure
 */
public class TransitiveClosureJob {

	private static final long TO_SEC = 1000;
	private static final long WINDOW_SIZE = 3 * TO_SEC;
//	private static final long WINDOW_SIZE = 20 * TO_SEC;

	private static final long WINDOW_SLIDE = 1 * TO_SEC;
	//TODO: This must be divisible
	private static final long SLIDE_PER_WINDOW = WINDOW_SLIDE / WINDOW_SLIDE;

	private static final Timestamp INITIAL_TIMESTAMP = Timestamp.valueOf("2000-01-01 00:00:00");

	private static IDBQbi idbQbi = new IDBQbi();
//	private static EDBQbi edbQbi = new EDBQbi();
	private static long totalDura = 0L;
	private static long totalSize = 0L;

	private static long _gap = 10000;
	private static long _start = 10000;

	private static List<ArcSlide> loopJoinArcs (List<ArcSlide> l, List<ArcSlide> r) throws Exception{
		List<ArcSlide> _ret = new ArrayList<>();
		for (ArcSlide e1 : l) {
			for (ArcSlide e2 : r) {
				if (e1.getToId() == e2.getFromId()) {
					Long newIter = max(e1.getIteration(), e2.getIteration()) + 1L;
					Long newTs= max(e1.getTimestamp(), e2.getTimestamp());
					// Min mSlide along derivation path
					Long newSlide = min(e1.getSlide().getSlideNum(), e2.getSlide().getSlideNum());
					_ret.add(new ArcSlide(e1.getFromId(), e2.getToId(), newTs, new Slide(newSlide, WINDOW_SLIDE), newIter));
				} else if (e1.getFromId() == e2.getToId()) {
					Long newIter = max(e1.getIteration(), e2.getIteration()) + 1L;
					Long newTs= max(e1.getTimestamp(), e2.getTimestamp());
					// Min mSlide along derivation path
					Long newSlide = min(e1.getSlide().getSlideNum(), e2.getSlide().getSlideNum());
					_ret.add(new ArcSlide(e2.getFromId(), e1.getToId(), newTs, new Slide(newSlide, WINDOW_SLIDE), newIter));
				}
			}
		}
		return _ret;
	}

	private static List<ArcSlide> QBIJoinNewEDBWithAll (List<ArcSlide> newEDBs, List<ArcSlide> allAtoms) throws Exception{
		List<ArcSlide> newWithAll = loopJoinArcs(newEDBs, allAtoms);
		return newWithAll;
	}


	public static void main(String[] args) throws Exception {

//		StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();

		Configuration conf = new Configuration();

		StreamExecutionEnvironment env = StreamExecutionEnvironment.createLocalEnvironmentWithWebUI(conf);


		env.setParallelism(1);


		WatermarkStrategy<Arc> strategy = WatermarkStrategy
				.<Arc>forBoundedOutOfOrderness(Duration.ofSeconds(0))
				.withTimestampAssigner((event, timestamp) -> event.getTimestamp());

		DataStream<Arc> arcs = env
			.addSource(new ArcSource())
			.assignTimestampsAndWatermarks(strategy)
			.name("arc");


		DataStream<ArcSlide> processedArcs = arcs.process(new ArcProcessor(WINDOW_SIZE, WINDOW_SLIDE));

		// Without QBI - CC
//		DataStream<ArcSlide> out = processedArcs
//				.windowAll(SlidingEventTimeWindows.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
//				.apply(new CCWindowFuncWithOutQbi());
//
//		out.print();
//		out.addSink(new ArcSink());


		//With QBI - CC
//		DataStream<ArcSlide> out = processedArcs
//				.windowAll(SlidingEventTimeWindows.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
//				.apply(new CCWindowFuncWithQbi());
//
//		out.print();
//		out.addSink(new ArcSink());





//		// Without QBI - PR
//		DataStream<ArcSlide> out = processedArcs
//				.windowAll(SlidingEventTimeWindows.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
//				.apply(new PRWindowFuncWithOutQbi());
//
//		out.print();
//		out.addSink(new ArcSink());

		// With QBI - PR
//		DataStream<ArcSlide> out = processedArcs
//				.windowAll(SlidingEventTimeWindows.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
//				.apply(new PRWindowFuncWithQbi());
//
//		out.print();
//		out.addSink(new ArcSink());



						// Without QBI
//		DataStream<ArcSlide> out = processedArcs
//				.windowAll(SlidingEventTimeWindows.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
//				.apply(new AllWindowFunction<ArcSlide, ArcSlide, TimeWindow>() {
//
//					@Override
//					public void apply (TimeWindow window,
//									   Iterable<ArcSlide> arcs,
//									   Collector<ArcSlide> out) throws Exception {
//
//						long startTime = System.nanoTime();
//
//												long windowBeg = window.getStart();
//						long windowEnd = window.getEnd();
//						long currentWindowLeftMostSlide = (windowBeg - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE;
//						long currentSlide = ((windowEnd - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE) - 1;
//
//						// Expire all atoms
//						idbQbi.expireAtoms(currentWindowLeftMostSlide - 1);
//						List<ArcSlide> EDB = new ArrayList<>();
//						Set<ArcSlide> _ret = new HashSet<>();
//
//						for (ArcSlide a : arcs) {
//							EDB.add(a);
//						}
//						_ret.addAll(EDB);
//
//
//						List<ArcSlide> IDB = loopJoinArcs(EDB, EDB);
//						_ret.addAll(IDB);
//
//						while (IDB.size() > 0) {
////							System.err.println("IDB Sie:" + IDB.size());
////							System.err.println("_ret Sie:" + _ret.size());
//							IDB = loopJoinArcs(EDB, IDB);
//							IDB.removeAll(_ret);
//							_ret.addAll(IDB);
//						}
//						List<ArcSlide> IDBOld = idbQbi.fetchAtomsAll();
//
//						for (ArcSlide a : _ret) {
//							if (IDBOld == null || IDBOld.isEmpty() || !IDBOld.contains(a)){
//								idbQbi.insertAtom(a);
//								out.collect (a);
//							}
//						}
//
//						long endTime = System.nanoTime();
//
//
//						totalDura += (endTime - startTime);
//						totalSize += EDB.size();
//						if (totalSize > _start) {
//							System.err.println("Total Dura: " + totalDura);
//							System.err.println("Total Size: " + totalSize);
//							_start += _gap;
//						}
//
//
//					}
//					});
//
////		out.print();
//		out.addSink(new ArcSink());


//		// With QBI
//		DataStream<ArcSlide> outQbi = processedArcs.windowAll(SlidingEventTimeWindows.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
//				.apply(new AllWindowFunction<ArcSlide, ArcSlide, TimeWindow>() {
//
//					@Override
//					public void apply (TimeWindow window,
//									   Iterable<ArcSlide> arcs,
//									   Collector<ArcSlide> out) throws Exception {
//
//						List<ArcSlide> oldEDB = new ArrayList<>();
//						List<ArcSlide> newEDB = new ArrayList<>();
//						Set<ArcSlide> _ret = new HashSet<>();
//
//						long startTime = System.nanoTime();
//
//
//						long windowBeg = window.getStart();
//						long windowEnd = window.getEnd();
//						long currentWindowLeftMostSlide = (windowBeg - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE;
//						long currentSlide = ((windowEnd - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE) - 1;
//
//						// Expire all atoms
////						edbQbi.expireAtoms(currentWindowLeftMostSlide - 1);
//						idbQbi.expireAtoms(currentWindowLeftMostSlide - 1);
//
//						// This is all EDBs in this window
//						for (ArcSlide e : arcs) {
//							if (e.getSlide().getSlideNum() == currentSlide) {
//								newEDB.add(e);
//							} else {
//								oldEDB.add(e);
//							}
//						}
//
//						if (newEDB.size() != 0) {
//							// Exit rule specifically for TC
//							_ret.addAll(newEDB);
//
//							List<ArcSlide> IDB = new ArrayList<>();
//
//							// newEDB JOIN allEDB
//							IDB.addAll(loopJoinArcs(newEDB, Stream.concat(newEDB.stream(), oldEDB.stream())
//									.collect(Collectors.toList())));
//
//							// Iter #1 with new IDB
//							idbQbi.insertAtoms(IDB);
//							_ret.addAll(IDB);
//							Long thisIter = 1L;
//
//							while (IDB.size() > 0) {
//								List<ArcSlide> newJoinAll = loopJoinArcs(newEDB, Stream.concat(IDB.stream(), idbQbi.fetchAtomsByIteration(thisIter).stream())
//										.collect(Collectors.toList()));
//								List<ArcSlide> oldJoinNew = loopJoinArcs(oldEDB, IDB);
//								IDB = Stream.concat(newJoinAll.stream(), oldJoinNew.stream())
//										.collect(Collectors.toList());
//								IDB.removeAll(_ret);
//								_ret.addAll(IDB);
//								thisIter++;
//							}
//
//							for (ArcSlide a : _ret) {
//								out.collect (a);
//							}
//						}
//
//
//						long endTime = System.nanoTime();
//
//						totalDura += (endTime - startTime);
//						totalSize += (newEDB.size() + oldEDB.size());
//
//						if (totalSize > _start) {
//							System.err.println("Total Dura: " + totalDura);
//							System.err.println("Total Size: " + totalSize);
//							_start += _gap;
//						}
//
//					}
//				});
//
////		outQbi.print();
//		outQbi.addSink(new ArcSink());


		// With QBI - 2
//		DataStream<ArcSlide> outQbi = processedArcs.windowAll(SlidingEventTimeWindows.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
//				.apply(new AllWindowFunction<ArcSlide, ArcSlide, TimeWindow>() {
//
//					@Override
//					public void apply (TimeWindow window,
//									   Iterable<ArcSlide> arcs,
//									   Collector<ArcSlide> out) throws Exception {
//
//						List<ArcSlide> oldEDB = new ArrayList<>();
//						List<ArcSlide> newEDB = new ArrayList<>();
//						List<ArcSlide> allEDB = new ArrayList<>();
//
//						Set<ArcSlide> _ret = new HashSet<>();
//
//						long startTime = System.nanoTime();
//
//
//						long windowBeg = window.getStart();
//						long windowEnd = window.getEnd();
//						long currentWindowLeftMostSlide = (windowBeg - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE;
//						long currentSlide = ((windowEnd - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE) - 1;
//
//						// Expire all atoms
////						edbQbi.expireAtoms(currentWindowLeftMostSlide - 1);
//						idbQbi.expireAtoms(currentWindowLeftMostSlide - 1);
//
//						// This is all EDBs in this window
//						for (ArcSlide e : arcs) {
//							if (e.getSlide().getSlideNum() == currentSlide) {
//								newEDB.add(e);
//							} else {
//								oldEDB.add(e);
//							}
//							allEDB.add(e);
//						}
//
//						if (newEDB.size() != 0) {
//							// Exit rule specifically for TC
//							_ret.addAll(newEDB);
//
//							List<ArcSlide> newIDB = new ArrayList<>();
//
//							// newEDB JOIN allEDB
//							newIDB.addAll(loopJoinArcs(newEDB, Stream.of(newEDB, oldEDB, idbQbi.fetchAtomsAll()).flatMap(Collection::stream).collect(Collectors.toList())));
//
////							List<ArcSlide> oldIDB = idbQbi.fetchAtomsAll();
////							newIDB.addAll()
//
//							// Iter #1 with new IDB
////							idbQbi.insertAtoms(newIDB);
//							_ret.addAll(newIDB);
//
//							while (newIDB.size() > 0) {
//								newIDB = loopJoinArcs(newIDB, allEDB);
////								List<ArcSlide> oldJoinNew = loopJoinArcs(oldEDB, IDB);
////								IDB = Stream.concat(newJoinAll.stream(), oldJoinNew.stream())
////										.collect(Collectors.toList());
//								newIDB.removeAll(_ret);
//								_ret.addAll(newIDB);
////								thisIter++;
//							}
//
//							for (ArcSlide a : _ret) {
//								idbQbi.insertAtom(a);
//								out.collect (a);
//							}
//						}
//
//
//						long endTime = System.nanoTime();
//
//						totalDura += (endTime - startTime);
//						totalSize += (newEDB.size() + oldEDB.size());
//
//						if (totalSize > _start) {
//							System.err.println("Total Dura: " + totalDura);
//							System.err.println("Total Size: " + totalSize);
//							_start += _gap;
//						}
//
//					}
//				});
//
//		outQbi.print();
//		outQbi.addSink(new ArcSink());



		env.execute("Transitive Closure");
	}

//	private static class MyMapper extends RichMapFunction<ArcSlide, ArcSlide> {
//
//		private transient Meter meter;
//
//		final ConsoleReporter reporter = ConsoleReporter.forRegistry(registry)
//				.convertRatesTo(TimeUnit.SECONDS)
//				.convertDurationsTo(TimeUnit.MILLISECONDS)
//				.build();
//
//		@Override
//		public void open(Configuration parameters) throws Exception {
//			super.open(parameters);
//			this.meter = getRuntimeContext()
//					.getMetricGroup()
//					.meter("myMeter", new DropwizardMeterWrapper(new com.codahale.metrics.Meter()));
//		}
//
//		@Override
//		public ArcSlide map(ArcSlide value) throws Exception {
//			this.meter.markEvent();
//			return value;
//		}
//	}
}

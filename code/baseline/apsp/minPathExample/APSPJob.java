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

package minPathExample;

import minPathExample.entity.WeightedArc;
import minPathExample.entity.WeightedArcSlide;
import minPathExample.qbi.EDBQbi;
import minPathExample.qbi.IDBQbi;
import minPathExample.qbi.Slide;
import minPathExample.sink.WeightedArcSlideSink;
import minPathExample.source.WeightedArcSource;
import org.apache.flink.api.common.eventtime.WatermarkStrategy;
import org.apache.flink.api.java.tuple.Tuple2;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.api.functions.windowing.AllWindowFunction;
import org.apache.flink.streaming.api.windowing.assigners.SlidingEventTimeWindows;
import org.apache.flink.streaming.api.windowing.time.Time;
import org.apache.flink.streaming.api.windowing.windows.TimeWindow;
import org.apache.flink.util.Collector;

import java.sql.Timestamp;
import java.time.Duration;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static java.lang.Math.max;
import static java.lang.Math.min;


/**
	Code for Transitive Closure
 */
public class SSSPJob {

	private static final long TO_SEC = 1000;
	private static final long WINDOW_SIZE = 20 * TO_SEC;
	private static final long WINDOW_SLIDE = 1 * TO_SEC;
	//TODO: This must be divisible
	private static final long SLIDE_PER_WINDOW = WINDOW_SLIDE / WINDOW_SLIDE;

	private static final Timestamp INITIAL_TIMESTAMP = Timestamp.valueOf("2000-01-01 00:00:00");


	private static IDBQbi idbQbi = new IDBQbi();
	private static EDBQbi edbQbi = new EDBQbi();

	private static long _gap = 10000;
	private static long _start = 10000;
	private static long totalDura = 0L;
	private static long totalSize = 0L;

	private static  List<WeightedArcSlide> loopJoinArcs (List<WeightedArcSlide> l,List<WeightedArcSlide> r,
														 HashMap<Tuple2<Long, Long>,
															WeightedArcSlide> minPath) throws Exception{


		List<WeightedArcSlide> _ret = new ArrayList<>();
		for (WeightedArcSlide e1 : l) {
			for (WeightedArcSlide e2 : r) {
				if (e1.getToId() == e2.getFromId()) {
					// Min slide along derivation path
					Long newSlide = min(e1.getSlide().getSlideNum(), e2.getSlide().getSlideNum());
					// Other fields
					Long newIter = max(e1.getIteration(), e2.getIteration()) + 1L;
					Long newTs= max(e1.getTimestamp(), e2.getTimestamp());
					Double newWeight = e1.getWeight() + e2.getWeight();
					Tuple2<Long, Long> reachable = new Tuple2<>(e1.getFromId(), e2.getToId());
					if (!minPath.containsKey(reachable) ||
							minPath.get(reachable).getWeight() > newWeight) {
//							(minPath.get(reachable).getWeight() <= newWeight && minPath.get(reachable).getSlide().getSlideNum() < newSlide)) {
						minPath.put(reachable, new WeightedArcSlide(e1.getFromId(), e2.getToId(),
								newWeight, newTs, new Slide(newSlide, WINDOW_SLIDE), newIter));
						_ret.add(new WeightedArcSlide(e1.getFromId(), e2.getToId(),
								newWeight, newTs, new Slide(newSlide, WINDOW_SLIDE), newIter));
					}

				} else if (e1.getFromId() == e2.getToId()) {
					// Min slide along derivation path
					Long newSlide = min(e1.getSlide().getSlideNum(), e2.getSlide().getSlideNum());
					// Other fields
					Long newIter = max(e1.getIteration(), e2.getIteration()) + 1L;
					Long newTs= max(e1.getTimestamp(), e2.getTimestamp());
					Double newWeight = e1.getWeight() + e2.getWeight();
					Tuple2<Long, Long> reachable = new Tuple2<>(e2.getFromId(), e1.getToId());
					if (!minPath.containsKey(reachable) ||
							minPath.get(reachable).getWeight() > newWeight) {
//							(minPath.get(reachable).getWeight() <= newWeight && minPath.get(reachable).getSlide().getSlideNum() < newSlide)) {
						minPath.put(reachable, new WeightedArcSlide(e2.getFromId(), e1.getToId(),
								newWeight, newTs, new Slide(newSlide, WINDOW_SLIDE), newIter));
						_ret.add(new WeightedArcSlide(e2.getFromId(), e1.getToId(),
								newWeight, newTs, new Slide(newSlide, WINDOW_SLIDE), newIter));
					}
//					else if (minPath.get(reachable).getWeight() <= newWeight && minPath.get(reachable).getSlide().getSlideNum() < newSlide) {
//						minPath.put(reachable, new WeightedArcSlide(e1.getFromId(), e2.getToId(), newWeight, newTs, new Slide(newSlide, WINDOW_SLIDE), newIter));
//					}
				}
			}
		}
		return _ret;
	}


	public static void main(String[] args) throws Exception {

		StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();

		env.setParallelism(1);


		WatermarkStrategy<WeightedArc> strategy = WatermarkStrategy
				.<WeightedArc>forBoundedOutOfOrderness(Duration.ofSeconds(0))
				.withTimestampAssigner((event, timestamp) -> event.getTimestamp());

		DataStream<WeightedArc> arcs = env
			.addSource(new WeightedArcSource())
			.assignTimestampsAndWatermarks(strategy)
			.name("weighted-arc");


		DataStream<WeightedArcSlide> processedArcs = arcs.process(new WeightedArcProcessor(WINDOW_SIZE, WINDOW_SLIDE));


		// Without QBI
//		DataStream<WeightedArcSlide> out = processedArcs.windowAll(SlidingEventTimeWindows
//				.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
//				.apply(new AllWindowFunction<WeightedArcSlide, WeightedArcSlide, TimeWindow>() {
//
//					@Override
//					public void apply (TimeWindow window,
//									   Iterable<WeightedArcSlide> arcs,
//									   Collector<WeightedArcSlide> out) throws Exception {
//
//						List<WeightedArcSlide> EDB = new ArrayList<>();
//						Set<WeightedArcSlide> _ret = new HashSet<>();
//						HashMap<Tuple2<Long, Long>, WeightedArcSlide> minPath = new HashMap<>();
//
//						long startTime = System.nanoTime();
//
//						// First fill in minPath (per-Window) with current all first-stratum IDBs (EDBs)
//						for (WeightedArcSlide e : arcs) {
//							EDB.add(e);
//							Tuple2<Long, Long> reachable = new Tuple2<>(e.getFromId(), e.getToId());
//							minPath.put(reachable, e);
//						}
//
//
//						// Join EDB with first-stratum IDB (which is also EDB for this query)
//						List<WeightedArcSlide> IDB = loopJoinArcs(EDB, EDB, minPath);
//
//						// Perform SNE until no more new IDBs are produced, during which minPath helps to achieve PreM
//						while (IDB.size() > 0) {
//							IDB = loopJoinArcs(EDB, IDB, minPath);
//						}
//
//						// Fetch all IDB during this Window in HashMap to check duplicate
//						HashMap<Tuple2<Long, Long>, WeightedArcSlide> oldAggregate = idbQbi.fetchFinalMap();
//
//
//						// Now to output, we need to make sure that it must either 1) a new IDB or 2) old IDB with less
//						// weight
//						for (Tuple2<Long, Long> key: minPath.keySet()) {
//							if (!oldAggregate.containsKey(key)
//									|| oldAggregate.get(key).getWeight() > minPath.get(key).getWeight()) {
//								out.collect (minPath.get(key));
//								idbQbi.insertAtom(minPath.get(key));
//							}
//						}
//
//						long endTime = System.nanoTime();
//
//						totalDura += (endTime - startTime);
//						totalSize += (EDB.size());
//
//						if (totalSize > _start) {
//							System.err.println("Total Dura: " + totalDura);
//							System.err.println("Total Size: " + totalSize);
//							_start += _gap;
//						}
//					}
//					});
//
//		out.addSink(new WeightedArcSlideSink());


		// With QBI
//
//		DataStream<WeightedArcSlide> outQbi = processedArcs.windowAll(SlidingEventTimeWindows.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
//				.apply(new AllWindowFunction<WeightedArcSlide, WeightedArcSlide, TimeWindow>() {
//
//					@Override
//					public void apply (TimeWindow window,
//									   Iterable<WeightedArcSlide> arcs,
//									   Collector<WeightedArcSlide> out) throws Exception {
//
//						List<WeightedArcSlide> oldEDB = new ArrayList<>();
//						List<WeightedArcSlide> newEDB = new ArrayList<>();
//						Set<WeightedArcSlide> _ret = new HashSet<>();
//						HashMap<Tuple2<Long, Long>, WeightedArcSlide> minPath = new HashMap<>();
//
//						long windowBeg = window.getStart();
//						long windowEnd = window.getEnd();
//						long currentWindowLeftMostSlide = (windowBeg - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE;
//						long currentSlide = ((windowEnd - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE) - 1;
//
//						// Expire all atoms
//						edbQbi.expireAtoms(currentWindowLeftMostSlide - 1);
//						idbQbi.expireAtoms(currentWindowLeftMostSlide - 1);
//
//						// This is all EDBs in this window
//						for (WeightedArcSlide e : arcs) {
//							if (e.getSlide().getSlideNum() == currentSlide) {
//								newEDB.add(e);
//							} else {
//								oldEDB.add(e);
//							}
//						}
//
//						// Exit rule specifically for TC
////						_ret.addAll(newEDB);
//
//						List<WeightedArcSlide> IDB = new ArrayList<>();
//
//						// newEDB JOIN allEDB
//						IDB.addAll(loopJoinArcs(newEDB, Stream.concat(newEDB.stream(), oldEDB.stream()).collect(Collectors.toList()), minPath));
//
//						// Iter #1 with new IDB
////						idbQbi.insertAtoms(IDB);
////						_ret.addAll(IDB);
//						Long thisIter = 1L;
//
//						while (IDB.size() > 0) {
//							List<WeightedArcSlide> newJoinAll = loopJoinArcs(newEDB, Stream.concat(IDB.stream(), idbQbi.fetchAtomsByIteration(thisIter).stream())
//									.collect(Collectors.toList()), minPath);
//							List<WeightedArcSlide> oldJoinNew = loopJoinArcs(oldEDB, IDB, minPath);
//							IDB = Stream.concat(newJoinAll.stream(), oldJoinNew.stream())
//									.collect(Collectors.toList());
////							IDB.removeAll(_ret);
////							_ret.addAll(IDB);
//							thisIter++;
//						}
//
//
//
//						for (WeightedArcSlide a : minPath.values()) {
//							out.collect (a);
//						}
//					}
//				});

//		outQbi.print();

//		With QBI - 2
		DataStream<WeightedArcSlide> outQbi = processedArcs.windowAll(SlidingEventTimeWindows.of(Time.milliseconds(WINDOW_SIZE), Time.milliseconds(WINDOW_SLIDE)))
				.apply(new AllWindowFunction<WeightedArcSlide, WeightedArcSlide, TimeWindow>() {

					@Override
					public void apply (TimeWindow window,
									   Iterable<WeightedArcSlide> arcs,
									   Collector<WeightedArcSlide> out) throws Exception {

						List<WeightedArcSlide> newEDB = new ArrayList<>();
						List<WeightedArcSlide> allEDB = new ArrayList<>();
//						List<WeightedArcSlide> _ret = new ArrayList<>();





						long startTime = System.nanoTime();
						long windowBeg = window.getStart();
						long windowEnd = window.getEnd();
						long currentWindowLeftMostSlide = (windowBeg - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE;
						long currentSlide = ((windowEnd - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE) - 1;

						// Expire all atoms
						idbQbi.expireAtoms(currentWindowLeftMostSlide - 1);

						// Fetch hashmap
						HashMap<Tuple2<Long, Long>, WeightedArcSlide> minPathNew = new HashMap<>();
						HashMap<Tuple2<Long, Long>, WeightedArcSlide> minPath = idbQbi.fetchFinalMap();



						// This is all EDBs in this window，minPath contains IDB for the first-stratum, do not
						// need to add all b/c qbi keeps previous IDBs
						for (WeightedArcSlide e : arcs) {
							if (e.getSlide().getSlideNum() == currentSlide) {
								Tuple2<Long, Long> reachable = new Tuple2<>(e.getFromId(), e.getToId());
								if (!minPath.containsKey(reachable) ||
										minPath.get(reachable).getWeight() > e.getWeight()) {


											minPath.put(reachable, new WeightedArcSlide(e.getFromId(), e.getToId(),
											e.getWeight(), e.getTimestamp(), e.getSlide(), e.getIteration()));

											minPathNew.put(reachable, new WeightedArcSlide(e.getFromId(), e.getToId(),
											e.getWeight(), e.getTimestamp(), e.getSlide(), e.getIteration()));

								}

								newEDB.add(e);
							}
							allEDB.add(e);
						}

						// We only eval if there are newEDBs, otherwise skip this window
						if (newEDB.size() != 0){

							// New IDBs
							List<WeightedArcSlide> newIDB = new ArrayList<>();

							// newEDB JOIN previous IDB & this IDB, b/c previous IDB can be fetched by fetchAtoms
							// and newEDBs will be added in the end
							newIDB.addAll(loopJoinArcs(newEDB, Stream.of(newEDB, idbQbi.fetchAtomsAll()).flatMap(Collection::stream).collect(Collectors.toList()), minPath));


							while (newIDB.size() > 0) {
								// Add to newly changed minPathNew
								for (WeightedArcSlide e : newIDB) {
									Tuple2<Long, Long> reachable = new Tuple2<>(e.getFromId(), e.getToId());
									if (!minPathNew.containsKey(reachable) ||
											minPathNew.get(reachable).getWeight() > e.getWeight()) {
										minPathNew.put(reachable, new WeightedArcSlide(e.getFromId(), e.getToId(),
												e.getWeight(), e.getTimestamp(), e.getSlide(), e.getIteration()));
									}
								}

								newIDB = loopJoinArcs(newIDB, allEDB, minPath);
							}
						}




						for (WeightedArcSlide e : minPathNew.values()) {
							out.collect (e);
							idbQbi.insertAtom(e);
						}

						long endTime = System.nanoTime();

						totalDura += (endTime - startTime);
						totalSize += (allEDB.size());

						if (totalSize > _start) {
							System.err.println("Total Dura: " + totalDura);
							System.err.println("Total Size: " + totalSize);
							_start += _gap;
						}
					}



				});

		outQbi.addSink(new WeightedArcSlideSink());


		env.execute("Single Source Shortest Path");
	}
}

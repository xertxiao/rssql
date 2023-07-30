package tcExample.windowfunc;

import org.apache.flink.streaming.api.functions.windowing.AllWindowFunction;
import org.apache.flink.streaming.api.windowing.windows.TimeWindow;
import org.apache.flink.util.Collector;
import tcExample.TransitiveClosureJob;
import tcExample.common.entity.ArcSlide;
import tcExample.metadata.qbi.IDBQbi;
import tcExample.metadata.qbi.Slide;

import java.sql.Timestamp;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static java.lang.Math.max;
import static java.lang.Math.min;

public class CCWindowFuncWithQbi implements AllWindowFunction<ArcSlide, ArcSlide, TimeWindow> {

    private static HashMap<Double, ArcSlide> minMap = new HashMap<Double, ArcSlide>();


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


    @Override
    public void apply(TimeWindow timeWindow, Iterable<ArcSlide> arcs, Collector<ArcSlide> out) throws Exception {

        long startTime = System.nanoTime();

        long windowBeg = timeWindow.getStart();
        long windowEnd = timeWindow.getEnd();
        long currentWindowLeftMostSlide = (windowBeg - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE;
        long currentSlide = ((windowEnd - INITIAL_TIMESTAMP.getTime()) / WINDOW_SLIDE) - 1;

        // Expire all atoms
        idbQbi.expireAtoms(currentWindowLeftMostSlide - 1);
        List<ArcSlide> allEDB = new ArrayList<>();
        List<ArcSlide> newIDB = new ArrayList<>();

        Set<ArcSlide> _ret = new HashSet<>();

//						for (ArcSlide e : arcs) {
//							if (e.getSlide().getSlideNum() == currentSlide) {
//								newEDB.add(e);
//							} else {
//								oldEDB.add(e);
//							}
//						}

        for (ArcSlide a : arcs) {
            allEDB.add(a);
            System.err.println(a);
            a.setToId(a.getFromId());
            if (idbQbi.updateOrInsertMinByFromWithToAggr(a) == 1) {
                newIDB.add(a);
            }
        }


        List<ArcSlide> IDB = loopJoinArcsCC(allEDB, newIDB);
        _ret.addAll(IDB);

        while (IDB.size() > 0) {
//							System.err.println("IDB Sie:" + IDB.size());
//							System.err.println("_ret Sie:" + _ret.size());
            IDB = loopJoinArcsCC(allEDB, IDB);
            IDB.removeAll(_ret);
            _ret.addAll(IDB);
        }

        List<ArcSlide> IDBOld = idbQbi.fetchAtomsAll();

        for (ArcSlide a : minMap.values()) {
            if (IDBOld == null || IDBOld.isEmpty() || !IDBOld.contains(a)){
                idbQbi.updateOrInsertMinByFromWithToAggr(a);
                out.collect (a);
                System.err.println(a);
            }
        }
        System.err.println("========w=========");

        minMap.clear();
        long endTime = System.nanoTime();


        totalDura += (endTime - startTime);
        totalSize += allEDB.size();
        if (totalSize > _start) {
            System.err.println("Total Dura: " + totalDura);
            System.err.println("Total Size: " + totalSize);
            _start += _gap;
        }


    }

    private List<ArcSlide> loopJoinArcsCC(List<ArcSlide> edb, List<ArcSlide> idb) {
        List<ArcSlide> _ret = new ArrayList<>();
        for (ArcSlide e : edb) {
            for (ArcSlide i : idb) {
                if (e.getFromId() == i.getFromId() &&
                        (!minMap.containsKey(e.getToId()) ||
                                minMap.get(e.getToId()).getToId() > i.getToId())) {

                    Long newIter = max(e.getIteration(), i.getIteration()) + 1L;
                    Long newTs= max(e.getTimestamp(), i.getTimestamp());
                    // Min slide along derivation path
                    Long newSlide = min(e.getSlide().getSlideNum(), i.getSlide().getSlideNum());
                    ArcSlide nac = new ArcSlide(e.getToId(), i.getToId(), newTs, new Slide(newSlide, WINDOW_SLIDE), newIter);
                    minMap.put(e.getToId(), nac);
                    _ret.add(nac);
                }
            }
        }
        return _ret;
    }
}

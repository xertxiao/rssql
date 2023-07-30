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

import static java.lang.Math.*;

public class PRWindowFuncWithOutQbi implements AllWindowFunction<ArcSlide, ArcSlide, TimeWindow> {

    private static HashMap<Double, ArcSlide> refMap = new HashMap<Double, ArcSlide>();
    private static HashMap<Double, ArcSlide> prMap = new HashMap<Double, ArcSlide>();



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
        List<ArcSlide> EDB = new ArrayList<>();
        Set<ArcSlide> _ret = new HashSet<>();


        for (ArcSlide a : arcs) {
            System.err.println("a: "+a);
            EDB.add(a);
            //idbQbi.insertAtom(a);
            prMap.put(a.getFromId(),
                    new ArcSlide(a.getFromId(), 1.0,
                            a.getTimestamp(), a.getSlide(), a.getIteration()));

            if(refMap.containsKey(a.getFromId())) {
                refMap.put(a.getFromId(), new ArcSlide(a.getFromId(), refMap.get(a.getFromId()).getToId() + 1.0,
                        a.getTimestamp(), a.getSlide(), a.getIteration()));
            } else {
                refMap.put(a.getFromId(),
                        new ArcSlide(a.getFromId(), 1.0,
                                a.getTimestamp(), a.getSlide(), a.getIteration()));
            }

        }
        //_ret.addAll(EDB);
        List<ArcSlide> refList = new ArrayList<>(refMap.values());
        List<ArcSlide> prList = new ArrayList<>(prMap.values());





        List<ArcSlide> IDB = loopJoinArcsPubRefPr(EDB, refList, prList);
        _ret.addAll(IDB);

        while (IDB.size() > 0) {
            System.err.println("IDB Sie:" + IDB.size());
            List<ArcSlide> refListRec = new ArrayList<>(refMap.values());
            List<ArcSlide> prListRec = new ArrayList<>(prMap.values());
            IDB = loopJoinArcsPubRefPr(EDB, refListRec, prListRec);
            IDB.removeAll(_ret);
            _ret.addAll(IDB);
        }

        List<ArcSlide> IDBOld = idbQbi.fetchAtomsAll();

        for (ArcSlide a : prMap.values()) {
            if (IDBOld == null || IDBOld.isEmpty() || !IDBOld.contains(a)){
                idbQbi.insertAtom(a);
                out.collect (a);
                System.err.println(a);
            }
        }
        System.err.println("========w=========");

        long endTime = System.nanoTime();


        totalDura += (endTime - startTime);
        totalSize += EDB.size();
        if (totalSize > _start) {
            System.err.println("Total Dura: " + totalDura);
            System.err.println("Total Size: " + totalSize);
            _start += _gap;
        }


    }

    private List<ArcSlide> loopJoinArcsPubRefPr(List<ArcSlide> pub, List<ArcSlide> ref, List<ArcSlide> pr) {
        List<ArcSlide> _ret = new ArrayList<>();
        for (ArcSlide e : pub) {
            for (ArcSlide r : ref) {
                for (ArcSlide p : pr) {

                    if (e.getFromId() == r.getFromId() && e.getFromId() == p.getFromId()) {

                    //double wdrby2 = (double) Math.round((p.getToId()/r.getToId()) * 100) / 100;
                    double wdrby2 = (double) p.getToId()/r.getToId();


                    Long newIter = max(e.getIteration(), p.getIteration()) + 1L;
                    Long newTs = max(max(e.getTimestamp(), p.getTimestamp()), r.getTimestamp());
                    // Min slide along derivation path
                    Long newSlide = min(min(e.getSlide().getSlideNum(), p.getSlide().getSlideNum()), r.getSlide().getSlideNum());




                    ArcSlide nac = new ArcSlide(e.getToId(), wdrby2, newTs, new Slide(newSlide, WINDOW_SLIDE), newIter);

                    if(prMap.containsKey(e.getToId()) && abs(prMap.get(e.getToId()).getToId() - wdrby2) <= 0.001) {
                        System.err.println("Diff: "+abs(prMap.get(e.getToId()).getToId() - wdrby2));
                        nac.setToId(nac.getToId() + prMap.get(e.getToId()).getToId());
                        prMap.put(e.getToId(), nac);
                        _ret.add(nac);
                    } else if (!prMap.containsKey(e.getToId())) {
                        prMap.put(e.getToId(), nac);
                        _ret.add(nac);
                    }
                }
                }
            }
        }
        return _ret;
    }
}

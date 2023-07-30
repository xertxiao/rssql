package tcExample.windowfunc;

import org.apache.flink.streaming.api.functions.windowing.AllWindowFunction;
import org.apache.flink.streaming.api.windowing.windows.TimeWindow;
import org.apache.flink.util.Collector;
import tcExample.TransitiveClosureJob;
import tcExample.common.entity.Arc;
import tcExample.common.entity.ArcSlide;
import tcExample.metadata.qbi.IDBQbi;
import tcExample.metadata.qbi.Slide;

import java.sql.Timestamp;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static java.lang.Math.*;

public class PRWindowFuncWithQbi implements AllWindowFunction<ArcSlide, ArcSlide, TimeWindow> {

    private static HashMap<Double, ArcSlide> refMap = new HashMap<Double, ArcSlide>();
    private static HashMap<Double, ArcSlide> prMap = new HashMap<Double, ArcSlide>();



    private static final long TO_SEC = 1000;
    private static final long WINDOW_SIZE = 3 * TO_SEC;
//	private static final long WINDOW_SIZE = 20 * TO_SEC;

    private static final long WINDOW_SLIDE = 1 * TO_SEC;
    //TODO: This must be divisible
    private static final long SLIDE_PER_WINDOW = WINDOW_SLIDE / WINDOW_SLIDE;

    private static final Timestamp INITIAL_TIMESTAMP = Timestamp.valueOf("2000-01-01 00:00:00");

    private static IDBQbi idbQbiPr = new IDBQbi();
    private static IDBQbi idbQbiRf = new IDBQbi();

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
        idbQbiPr.expireAtoms(currentWindowLeftMostSlide - 1);
        idbQbiRf.expireAtoms(currentWindowLeftMostSlide - 1);

        List<ArcSlide> EDB = new ArrayList<>();
        Set<ArcSlide> _ret = new HashSet<>();


        for (ArcSlide a : arcs) {
            System.err.println("a: "+a);
            EDB.add(a);
            if (a.getSlide().getSlideNum() == currentSlide) {
                ArcSlide pr = new ArcSlide(a);
                pr.setToId(1.0);
                idbQbiPr.updateOrInsertSumByFromWithToAggrInPlace(pr);
                prMap.put(a.getFromId(), pr);

                ArcSlide rf = new ArcSlide(a);
                idbQbiRf.updateOrInsertCountByFromWithToAggrInPlace(rf);
                refMap.put(a.getFromId(), rf);
            }
        }

        //_ret.addAll(EDB);
        List<ArcSlide> refList = new ArrayList<>(refMap.values());
        List<ArcSlide> prList = new ArrayList<>(prMap.values());



        for (ArcSlide e : EDB) System.err.println("edb-e: " + e);


        List<ArcSlide> IDB = loopJoinArcsPubRefPr(EDB, refList, prList);
        _ret.addAll(IDB);

        while (IDB.size() > 0) {
            System.err.println("IDB Sie:" + IDB.size());
            List<ArcSlide> prListRec = new ArrayList<>(prMap.values());
            IDB = loopJoinArcsPubRefPr(EDB, refList, prListRec);
            IDB.removeAll(_ret);
            _ret.addAll(IDB);
        }

        List<ArcSlide> IDBOld = idbQbiPr.fetchAtomsAll();

        for (ArcSlide a : prMap.values()) {
            if (IDBOld == null || IDBOld.isEmpty() || !IDBOld.contains(a)){
                idbQbiPr.replceSumByFromWithToAggrInPlace(a);
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

                        System.err.println("e: " + e);
                        System.err.println("r: " + e);
                        System.err.println("p: " + e);


                        //double wdrby2 = (double) Math.round((p.getToId()/r.getToId()) * 100) / 100;
                        double wdrby2 = (double) p.getToId()/r.getToId();


                        Long newIter = max(e.getIteration(), p.getIteration()) + 1L;
                        Long newTs = max(max(e.getTimestamp(), p.getTimestamp()), r.getTimestamp());
                        // Min slide along derivation path
                        Long newSlide = min(min(e.getSlide().getSlideNum(), p.getSlide().getSlideNum()), r.getSlide().getSlideNum());




                        ArcSlide nac = new ArcSlide(e.getToId(), wdrby2, newTs, new Slide(newSlide, WINDOW_SLIDE), newIter);

                        if(prMap.containsKey(e.getToId()) && abs(prMap.get(e.getToId()).getToId() - wdrby2) <= 0.01) {
                            System.err.println("Diff: "+abs(prMap.get(e.getToId()).getToId() - wdrby2));
                            System.err.println("nac: "+nac);
                            System.err.println("Actual: "+prMap.get(e.getToId()));
                            nac.setToId(nac.getToId() + prMap.get(e.getToId()).getToId());
                            prMap.put(e.getToId(), nac);
                            _ret.add(nac);
                        } else if (!prMap.containsKey(e.getToId())) {
                            System.err.println("Other: "+e);
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

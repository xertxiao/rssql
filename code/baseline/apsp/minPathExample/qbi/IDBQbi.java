package minPathExample.qbi;

import minPathExample.entity.WeightedArcSlide;
import org.apache.flink.api.java.tuple.Tuple2;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class IDBQbi implements Qbi {
    // TODO: do we need the per-iteration here?
    private HashMap<Long, HashMap<Long, HashMap<Tuple2<Long, Long>, WeightedArcSlide>>> mIndex = new HashMap<>();


    public HashMap<Tuple2<Long, Long>, WeightedArcSlide> fetchFinalMap() {
        HashMap<Tuple2<Long, Long>, WeightedArcSlide> _ret = new HashMap<>();
        for (Long key : mIndex.keySet()) {
            HashMap<Long, HashMap<Tuple2<Long, Long>, WeightedArcSlide>> temp = mIndex.get(key);
            for (long secondkey : temp.keySet()) {
                _ret.putAll(temp.get(secondkey));
            }
        }
        return _ret;
    }
    @Override
    public void expireAtoms(Long slideNum) {
        mIndex.remove(slideNum);
    }

    @Override
    public List<WeightedArcSlide> fetchAtomsAll() {
        List<WeightedArcSlide> _ret = new ArrayList<>();
        for (Long key : mIndex.keySet()) {
            HashMap<Long, HashMap<Tuple2<Long, Long>, WeightedArcSlide>> temp = mIndex.get(key);
            for (long secondkey : temp.keySet()) {
                HashMap<Tuple2<Long, Long>, WeightedArcSlide> temp2 = temp.get(secondkey);
                if(temp2 != null) {
                    _ret.addAll(temp2.values());
                }
            }
        }
        return _ret;
    }

    @Override
    public List<WeightedArcSlide> fetchAtomsByIteration(Long iter) {
        //TODO: Optimize this
        List<WeightedArcSlide> _ret = new ArrayList<>();
        for (Long key : mIndex.keySet()) {
            HashMap<Long, HashMap<Tuple2<Long, Long>, WeightedArcSlide>> temp = mIndex.get(key);
            if(temp != null && temp.get(iter) != null) {
                _ret.addAll(temp.get(iter).values());
            }
        }
        return _ret;
    }

    @Override
    public void insertAtoms(List<WeightedArcSlide> arcs) {
        for (WeightedArcSlide e : arcs) {
            insertAtom(e);
        }
    }

    @Override
    public void insertAtom(WeightedArcSlide arc) {
        Tuple2<Long, Long> key = new Tuple2<>(arc.getFromId(), arc.getToId());
        HashMap<Tuple2<Long, Long>, WeightedArcSlide> pathMap = mIndex.computeIfAbsent(arc.getSlide().getSlideNum(), k -> new HashMap<>())
                .computeIfAbsent(arc.getIteration(), k -> new HashMap<>());
        if (!pathMap.containsKey(key) || pathMap.get(key).getWeight() > arc.getWeight() ) {
            pathMap.put(key, arc);
        }
    }


}

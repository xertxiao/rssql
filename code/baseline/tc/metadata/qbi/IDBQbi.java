package tcExample.metadata.qbi;

import tcExample.common.entity.Arc;
import tcExample.common.entity.ArcSlide;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class IDBQbi implements Qbi {
    private HashMap<Long, HashMap<Double, List<ArcSlide>>> mIndex = new HashMap<>();

    @Override
    public void expireAtoms(Long slideNum) {
        mIndex.remove(slideNum);
    }

    @Override
    public List<ArcSlide> fetchAtomsAll() {
        List<ArcSlide> _ret = new ArrayList<>();
        for (Long key : mIndex.keySet()) {
            HashMap<Double, List<ArcSlide>> temp = mIndex.get(key);
            if(temp != null) {
                for (List<ArcSlide> al : temp.values()) _ret.addAll(al);
            }
        }
        return _ret;
    }

    @Override
    public List<ArcSlide> fetchAtomsByIteration(Long iter) {
        //TODO: Optimize this
        List<ArcSlide> _ret = new ArrayList<>();
        for (Long key : mIndex.keySet()) {
            HashMap<Double, List<ArcSlide>> temp = mIndex.get(key);
            if(temp != null && temp.get(iter) != null) {
                _ret.addAll(temp.get(iter));
            }
        }
        return _ret;
    }

    @Override
    public void insertAtoms(List<ArcSlide> arcs) {
        for (ArcSlide e : arcs) {
            insertAtom(e);
        }
    }

    @Override
    public void insertAtom(ArcSlide arc) {
        //TODO: use arc.getFromId for here for TC, CC, and PR
        mIndex.computeIfAbsent(arc.getSlide().getSlideNum(), k -> new HashMap<>())
                .computeIfAbsent(arc.getFromId(), k -> new ArrayList<>())
                .add(arc);
    }

    public int updateOrInsertMinByFromWithToAggr(ArcSlide arc){
        int ret = 0;
        if (mIndex.containsKey(arc.getSlide().getSlideNum())) {
            HashMap<Double, List<ArcSlide>> idToArc = mIndex.get(arc.getSlide().getSlideNum());
            if (idToArc.containsKey(arc.getFromId())
                    && idToArc.get(arc.getFromId()).get(0).getToId() > arc.getToId()) {
                idToArc.get(arc.getFromId()).get(0).setToId(arc.getToId());
            } else {
                this.insertAtom(arc);
                ret = 1;
            }
        } else {
            this.insertAtom(arc);
            ret = 1;
        }

        return ret;
    }

    // Void because it is always true, this assumes income is (id, sum), and will update (sum)
    public void updateOrInsertSumByFromWithToAggrInPlace(ArcSlide arc){
        if (mIndex.containsKey(arc.getSlide().getSlideNum())) {
            HashMap<Double, List<ArcSlide>> idToArc = mIndex.get(arc.getSlide().getSlideNum());
            if (idToArc.containsKey(arc.getFromId())) {
                Double nc = idToArc.get(arc.getFromId()).get(0).getToId() + arc.getToId();
                idToArc.get(arc.getFromId()).get(0).setToId(nc);
                arc.setToId(nc);
            } else {
                this.insertAtom(arc);
                arc.setToId(1.0);
            }
        } else {
            this.insertAtom(arc);
            arc.setToId(1.0);
        }
    }

    // Void because it is always true, this assumes income is (id, ct), and will update (ct)
    public void updateOrInsertCountByFromWithToAggrInPlace(ArcSlide arc){
        if (mIndex.containsKey(arc.getSlide().getSlideNum())) {
            HashMap<Double, List<ArcSlide>> idToArc = mIndex.get(arc.getSlide().getSlideNum());
            if (idToArc.containsKey(arc.getFromId())) {
                Double nc = idToArc.get(arc.getFromId()).get(0).getToId() + 1;
                idToArc.get(arc.getFromId()).get(0).setToId(nc);
                arc.setToId(nc);
            } else {
                this.insertAtom(arc);
                arc.setToId(1.0);
            }
        } else {
            this.insertAtom(arc);
            arc.setToId(1.0);
        }
    }

    public void replceSumByFromWithToAggrInPlace(ArcSlide arc){
        if (mIndex.containsKey(arc.getSlide().getSlideNum())) {
            HashMap<Double, List<ArcSlide>> idToArc = mIndex.get(arc.getSlide().getSlideNum());
            if (idToArc.containsKey(arc.getFromId())) {
                idToArc.get(arc.getFromId()).get(0).setToId(arc.getToId());
            }
        }
    }

}

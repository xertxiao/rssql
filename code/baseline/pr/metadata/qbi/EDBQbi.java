package tcExample.metadata.qbi;

import tcExample.common.entity.Arc;
import tcExample.common.entity.ArcSlide;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.stream.Collectors;

public class EDBQbi implements Qbi {
    private HashMap<Long, List<ArcSlide>> mIndex = new HashMap<>();

    @Override
    public void expireAtoms(Long slideNum) {
        mIndex.remove(slideNum);
    }

    @Override
    public List<ArcSlide> fetchAtomsAll() {
        return mIndex.values().stream().flatMap(List::stream).collect(Collectors.toList());
    }

    @Override
    public List<ArcSlide> fetchAtomsByIteration(Long iter) {
        return null;
    }

    @Override
    public void insertAtoms(List<ArcSlide> arcs) {
        //TODO: Optimize this?
        for (ArcSlide e : arcs) {
            assert e.getIteration() == 0L;
            mIndex.computeIfAbsent(e.getTimestamp(), k -> new ArrayList<>()).add(e);
        }
    }

    @Override
    public void insertAtom(ArcSlide arc) {
        assert arc.getIteration() == 0L;
        mIndex.computeIfAbsent(arc.getSlide().getSlideNum(), k -> new ArrayList<>()).add(arc);
    }
}

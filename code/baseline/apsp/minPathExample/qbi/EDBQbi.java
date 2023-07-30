package minPathExample.qbi;

import minPathExample.entity.WeightedArcSlide;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.stream.Collectors;

public class EDBQbi implements Qbi {
    private HashMap<Long, List<WeightedArcSlide>> mIndex = new HashMap<>();

    @Override
    public void expireAtoms(Long slideNum) {
        mIndex.remove(slideNum);
    }

    @Override
    public List<WeightedArcSlide> fetchAtomsAll() {
        return mIndex.values().stream().flatMap(List::stream).collect(Collectors.toList());
    }

    @Override
    public List<WeightedArcSlide> fetchAtomsByIteration(Long iter) {
        return null;
    }

    @Override
    public void insertAtoms(List<WeightedArcSlide> arcs) {
        //TODO: Optimize this?
        for (WeightedArcSlide e : arcs) {
            assert e.getIteration() == 0L;
            mIndex.computeIfAbsent(e.getTimestamp(), k -> new ArrayList<>()).add(e);
        }
    }

    @Override
    public void insertAtom(WeightedArcSlide arc) {
        assert arc.getIteration() == 0L;
        mIndex.computeIfAbsent(arc.getSlide().getSlideNum(), k -> new ArrayList<>()).add(arc);
    }
}

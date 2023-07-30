package minPathExample.qbi;

import minPathExample.entity.WeightedArcSlide;

import java.util.List;

interface Qbi {
    public void expireAtoms (Long ts);

    public List<WeightedArcSlide> fetchAtomsAll();
    public List<WeightedArcSlide> fetchAtomsByIteration(Long iter);

    public void insertAtoms(List<WeightedArcSlide> arcs);
    public void insertAtom(WeightedArcSlide arc);

}

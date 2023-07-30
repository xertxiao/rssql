package tcExample.metadata.qbi;

import tcExample.common.entity.Arc;
import tcExample.common.entity.ArcSlide;

import java.util.HashMap;
import java.util.List;

interface Qbi {
    public void expireAtoms (Long ts);

    public List<ArcSlide> fetchAtomsAll();
    public List<ArcSlide> fetchAtomsByIteration(Long iter);

    public void insertAtoms(List<ArcSlide> arcs);
    public void insertAtom(ArcSlide arc);

}

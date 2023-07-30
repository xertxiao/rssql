package tcExample;

import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;
import tcExample.common.entity.Arc;
import tcExample.common.entity.ArcSlide;
import tcExample.metadata.qbi.Slide;

import java.sql.Timestamp;

public class ArcProcessor extends ProcessFunction<Arc, ArcSlide> {

    private static final long serialVersionUID = 1L;
    private long windowSize;
    private long windowSlide;
    private static final Timestamp INITIAL_TIMESTAMP = Timestamp.valueOf("2000-01-01 00:00:00");

    private long getSlideNum (long windowSize, long windowSlide, long currentTime) {
        long timeDiff = currentTime - INITIAL_TIMESTAMP.getTime();
        long quotient = timeDiff / windowSlide;
//        long remainder = timeDiff % windowSize;
//        long windowBeg = (remainder > windowSlide) ? (quotient * windowSize + windowSlide) : (quotient * windowSize);
        return quotient;
    }

    public ArcProcessor(long windowSize, long windowSlide) {
        this.windowSize = windowSize;
        this.windowSlide = windowSlide;
    }

    @Override
    public void processElement(Arc arc, Context context, Collector<ArcSlide> collector) throws Exception {
        long mSlide = getSlideNum(windowSize, windowSlide, arc.getTimestamp());

        //System.err.println("arc: "+arc);
        // Set iter to 0L because processElement is only called on each EDB, which must have iter = 0.
        ArcSlide arcSlide = new ArcSlide(arc.getFromId(), arc.getToId(), arc.getTimestamp(), new Slide(mSlide, windowSlide), 0L);

        collector.collect(arcSlide);
    }
}

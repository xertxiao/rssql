package minPathExample;

import minPathExample.entity.WeightedArc;
import minPathExample.entity.WeightedArcSlide;
import minPathExample.qbi.Slide;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

import java.sql.Timestamp;

public class WeightedArcProcessor extends ProcessFunction<WeightedArc, WeightedArcSlide> {

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

    public WeightedArcProcessor(long windowSize, long windowSlide) {
        this.windowSize = windowSize;
        this.windowSlide = windowSlide;
    }

    @Override
    public void processElement(WeightedArc arc, Context context, Collector<WeightedArcSlide> collector) throws Exception {
        long mSlide = getSlideNum(windowSize, windowSlide, arc.getTimestamp());

        // Set iter to 0L because processElement is only called on each EDB, which must have iter = 0.
        WeightedArcSlide arcSlide = new WeightedArcSlide(arc.getFromId(), arc.getToId(), arc.getWeight(), arc.getTimestamp(), new Slide(mSlide, windowSlide), 0L);

        collector.collect(arcSlide);
    }
}

package tcExample.common.entity;
import tcExample.metadata.qbi.Slide;

import java.util.Objects;

/** A simple arc with mSlide info*/
public final class ArcSlide {
    private double fromId;

    private double toId;

    private long timestamp;

    private Slide mSlide;

    private long iteration;

    public ArcSlide() {}


    public ArcSlide(double fromId, double toId, long timestamp, Slide mSlide, long iteration) {
        this.fromId = fromId;
        this.toId = toId;
        this.timestamp = timestamp;
        this.mSlide = mSlide;
        this.iteration = iteration;
    }

    public ArcSlide(ArcSlide a) {
        this.fromId = a.fromId;
        this.toId = a.toId;
        this.timestamp = a.timestamp;
        this.mSlide = a.mSlide;
        this.iteration = a.iteration;
    }


    public double getFromId() {
        return fromId;
    }

    public void setFromId(double fromId) {
        this.fromId = fromId;
    }

    public double getToId() {
        return toId;
    }

    public void setToId(double toId) {
        this.toId = toId;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public Slide getSlide() {
        return mSlide;
    }

    public void setSlide(Slide mSlide) {
        this.mSlide = mSlide;
    }

    public long getIteration() {
        return iteration;
    }

    public void setIteration(long iteration) {
        this.iteration = iteration;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof ArcSlide)) return false;
        ArcSlide arcSlide = (ArcSlide) o;
        return fromId == arcSlide.fromId && toId == arcSlide.toId;
    }

    @Override
    public int hashCode() {
        return Objects.hash(fromId, toId);
    }

    @Override
    public String toString() {
        return "ArcSlide {" +
                "fromId=" + fromId +
                ", toId=" + toId +
                ", timestamp=" + timestamp +
                ", mSlide=" + mSlide +
                ", iteration=" + iteration +
                '}';
    }
}

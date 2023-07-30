package minPathExample.entity;

import minPathExample.qbi.Slide;

import java.util.Objects;

/** A simple arc with mSlide info*/
public final class WeightedArcSlide {
    private long fromId;

    private long toId;

    private double weight;

    private long timestamp;

    private Slide mSlide;

    private long iteration;

    public WeightedArcSlide() {}

    public WeightedArcSlide(long fromId, long toId, double weight, long timestamp, Slide mSlide, long iteration) {
        this.fromId = fromId;
        this.toId = toId;
        this.weight = weight;
        this.timestamp = timestamp;
        this.mSlide = mSlide;
        this.iteration = iteration;
    }

    public long getFromId() {
        return fromId;
    }

    public void setFromId(long fromId) {
        this.fromId = fromId;
    }

    public long getToId() {
        return toId;
    }

    public void setToId(long toId) {
        this.toId = toId;
    }

    public double getWeight() {
        return weight;
    }

    public void setWeight(double weight) {
        this.weight = weight;
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
        if (!(o instanceof WeightedArcSlide)) return false;
        WeightedArcSlide that = (WeightedArcSlide) o;
        return fromId == that.fromId && toId == that.toId && Double.compare(that.weight, weight) == 0;
    }

    @Override
    public int hashCode() {
        return Objects.hash(fromId, toId, weight);
    }


    @Override
    public String toString() {
        return "WeightedArcSlide{" +
                "fromId=" + fromId +
                ", toId=" + toId +
                ", weight=" + weight +
                ", timestamp=" + timestamp +
                ", mSlide=" + mSlide +
                ", iteration=" + iteration +
                '}';
    }
}

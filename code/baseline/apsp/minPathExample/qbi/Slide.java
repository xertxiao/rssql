package minPathExample.qbi;

import java.util.Objects;

public class Slide {
    private Long slideNum;
    private Long windowSlide;

    public Slide(Long slideNum, Long windowSlide) {
        this.slideNum = slideNum;
        this.windowSlide = windowSlide;
    }

    public Long getSlideNum() {
        return slideNum;
    }

    public Long getWindowSlide() {
        return windowSlide;
    }

    public void setSlideNum(Long slideNum) {
        this.slideNum = slideNum;
    }

    public void setWindowSlide(Long windowSlide) {
        this.windowSlide = windowSlide;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Slide)) return false;
        Slide mSlide = (Slide) o;
        return Objects.equals(slideNum, mSlide.slideNum) && Objects.equals(windowSlide, mSlide.windowSlide);
    }

    @Override
    public int hashCode() {
        return Objects.hash(slideNum, windowSlide);
    }

    @Override
    public String toString() {
        return "Slide{" +
                "slideNum=" + slideNum +
                ", windowSlide=" + windowSlide +
                '}';
    }
}

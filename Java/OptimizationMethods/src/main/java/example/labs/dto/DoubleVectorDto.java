package example.labs.dto;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.Getter;
import lombok.NonNull;
import lombok.Setter;

import java.util.List;

@Setter
@Getter
public class DoubleVectorDto {

    @NonNull
    private List<Double> points;

    @JsonIgnore
    private int size;

    @JsonProperty("points")
    public void setPoints(List<Double> points) {
        this.points = points;
        this.size = points.size();
    }

}

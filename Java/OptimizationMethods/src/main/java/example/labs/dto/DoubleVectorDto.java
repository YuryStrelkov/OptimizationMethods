package example.labs.dto;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.util.List;
import java.util.Objects;

@Setter
@Getter
@JsonInclude(JsonInclude.Include.NON_NULL)
public class DoubleVectorDto {

    @JsonIgnore
    private List<Double> row;
    @JsonIgnore
    private List<String> rowStr;

    @JsonIgnore
    private int size;

    @Schema(accessMode = Schema.AccessMode.READ_WRITE, example = "[-1, 2.5, -1.2]")
    @JsonProperty(value = "row", access = JsonProperty.Access.WRITE_ONLY)
    public void setRow(List<Double> row) {
        this.row = row;
        this.size = row.size();
    }

    @JsonProperty(value = "row", access = JsonProperty.Access.READ_ONLY)
    private List<? extends Serializable> getRowResponse() {
        if (Objects.nonNull(rowStr)) {
            return rowStr;
        }
        return row;
    }
}

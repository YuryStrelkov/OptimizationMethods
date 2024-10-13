package example.labs.dto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Setter
@Getter
public class DoubleMatrixDto {

    @Schema(example = "[{\"row\":[-1, 2.5]},{\"row\":[-2, 0.5]}]")
    private List<DoubleVectorDto> rows;

}

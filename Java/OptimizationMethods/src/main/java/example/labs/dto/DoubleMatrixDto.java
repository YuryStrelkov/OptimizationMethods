package example.labs.dto;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Setter
@Getter
public class DoubleMatrixDto {

    private List<DoubleVectorDto> vectors;

}

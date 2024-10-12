package example.labs.dto;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class MatrixOperationDto {

    private DoubleMatrixDto leftMx;
    private DoubleMatrixDto rightMx;
    private Double leftD;
    private Double rightD;

}

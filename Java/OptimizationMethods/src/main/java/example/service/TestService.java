package example.service;

import example.labs.dto.DoubleMatrixDto;
import example.labs.dto.DoubleVectorDto;
import example.labs.dto.MatrixOperationDto;
import example.mathUtils.*;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

@Service
public class TestService {

    public String someTest() {
        TemplateVector<Float> vector = new TemplateVector<>(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);

        return vector + "<br/>" +
                vector.insert(0, 111.0f) + "<br/>" +
                vector.insert(3, 333.0f) + "<br/>" +
                vector.insert(vector.size(), 999.0f) + "<br/>" +
                vector.pushBack(1.333f) + "<br/>" +
                vector.pushBack(1.333f) + "<br/>" +
                vector.pushBack(1.333f) + "<br/>" +
                vector.pushBack(1.333f) + "<br/>" +
                vector.pushBack(1.333f) + "<br/>" +
                vector.pushBack(1.333f) + "<br/>" +
                vector.pushBack(1.333f) + "<br/>" +
                vector.pushBack(1.333f) + "<br/>" +
                vector.pushBack(1.333f) + "<br/>" +
                vector.pushBack(9.999f) + "<br/>" +
                vector.removeAt(0) + "<br/>" +
                vector.removeAt(2) + "<br/>" +
                vector.removeAt(vector.size() - 1) + "<br/>" +
                new TemplateVector<>(vector) + "<br/>" +
                vector.get(new Slice(0, -1, 2)) + "<br/>";
    }

    public String vectorTest() {
        StringBuilder sb = new StringBuilder("///////////////////////////////////////////<br/>");
        sb.append("///////////////////////////////////////////<br/>")
                .append("////////       VectorSliceTest     ////////<br/>")
                .append("///////////////////////////////////////////<br/><br/>");
        DoubleVector longVector = new DoubleVector(2.0, 3.0, 4.0, 5.0, 6.0, 8.0);
        sb.append(String.format("l      = %s<br/>", longVector));
        Slice left = new Slice(0, 3);
        Slice right = new Slice(3, 6);
        sb.append(String.format("l[%s] = %s <br/>", left, longVector.get(left)))
                .append(String.format("r[%s] = %s <br/>", right, longVector.get(right)));
        DoubleVector a = longVector.get(left); // new DoubleVector(2.0, 3.0, 4.0);
        DoubleVector b = longVector.get(right); // new DoubleVector(5.0, 6.0, 8.0);

        sb.append("<br/><br/>///////////////////////////////////////////<br/>")
                .append("////////         VectorTest        ////////<br/>")
                .append("///////////////////////////////////////////<br/>")
                .append("////////      Internal methods     ////////<br/>")
                .append("///////////////////////////////////////////<br/><br/>")
                .append(String.format("%s += %s = ", b, a));

        b.add(a);
        sb.append(String.format("%s<br/>", b)).append(String.format("%s -= %s = ", b, a));
        b.sub(a);
        sb.append(String.format("%s<br/>", b)).append(String.format("%s *= %s = ", b, a));
        b.mul(a);
        sb.append(String.format("%s<br/>", b)).append(String.format("%s /= %s = ", b, a));
        b.div(a);
        sb.append(String.format("%s<br/>", b)).append(String.format("%s *= %s = ", b, 2));
        b.mul(2);
        sb.append(String.format("%s<br/>", b)).append(String.format("%s /= %s = ", b, 2));
        b.div(2);
        sb.append(String.format("%s<br/>", b)).append(String.format("%s += %s = ", b, 2));
        b.add(2);
        sb.append(String.format("%s<br/>", b)).append(String.format("%s -= %s = ", b, 2));
        b.sub(2);
        sb.append(String.format("%s<br/>", b));

        sb.append("<br/><br/>///////////////////////////////////////////<br/>")
                .append("////////      External methods     ////////<br/>")
                .append("///////////////////////////////////////////<br/><br/>")
                .append(String.format("%s + %s = %s<br/>", a, b, DoubleVectorUtils.add(a, b)))
                .append(String.format("%s - %s = %s<br/>", a, b, DoubleVectorUtils.sub(a, b)))
                .append(String.format("%s / %s = %s<br/>", a, b, DoubleVectorUtils.div(a, b)))
                .append(String.format("%s * %s = %s<br/>", a, b, DoubleVectorUtils.mul(a, b)))
                .append(String.format("%s + %s = %s<br/>", b, 2, DoubleVectorUtils.add(b, 2)))
                .append(String.format("%s + %s = %s<br/>", 2, b, DoubleVectorUtils.add(2, b)))
                .append(String.format("%s - %s = %s<br/>", 2, b, DoubleVectorUtils.sub(2, b)))
                .append(String.format("%s - %s = %s<br/>", b, 2, DoubleVectorUtils.sub(b, 2)))
                .append(String.format("%s * %s = %s<br/>", b, 2, DoubleVectorUtils.mul(b, 2)))
                .append(String.format("%s * %s = %s<br/>", 2, b, DoubleVectorUtils.mul(2, b)))
                .append(String.format("%s / %s = %s<br/>", b, 2,
                        NumericUtils.toRationalStr(DoubleVectorUtils.div(b, 2), false)))
                .append(String.format("%s / %s = %s<br/>", 2, b,
                        NumericUtils.toRationalStr(DoubleVectorUtils.div(2, b), false)))
                .append(String.format("| %s | = %s<br/>", b, b.magnitude()))
                .append(String.format("%s / | %s | = %s<br/>", b, b, b.normalized()))
                .append(String.format("(%s, %s) = %s<br/>", b.normalized(), b.normalized(),
                        b.normalized().dot(b.normalized())));
        return sb.toString();
    }

    public String matrixTest() {
        StringBuilder sb = new StringBuilder("<br/>////////////////////////////<br/>")
                .append("//////// MatrixTest ////////<br/>")
                .append("////////////////////////////<br/><br/>");

        DoubleMatrix matrix = new DoubleMatrix(new DoubleVector(8.0, 1.0, 6.0),
                new DoubleVector(3.0, 5.0, 7.0),
                new DoubleVector(4.0, 9.0, 2.0));

        DoubleMatrix matrix1 = new DoubleMatrix(new DoubleVector(2.0, 9.0, 4.0),
                new DoubleVector(7.0, 5.0, 3.0),
                new DoubleVector(6.0, 1.0, 8.0));

        sb.append(String.format("matrix1:<br/>%s<br/>", matrix))
                .append(String.format("matrix2:<br/>%s<br/>", matrix1))
                .append(String.format("matrix1 += matrix2 =<br/>%s<br/>", matrix.add(matrix1)))
                .append(String.format("matrix1 -= matrix2 =<br/>%s<br/>", matrix.sub(matrix1)))
                .append(String.format("matrix1 *= 2 =<br/>%s<br/> ", matrix.mul(2)))
                .append(String.format("matrix1 /= 2 =<br/>%s<br/> ", matrix.div(2)))
                .append(String.format("matrix1 += 2 =<br/>%s<br/> ", matrix.add(2)))
                .append(String.format("matrix1 -= 2 =<br/>%s<br/> ", matrix.sub(2)));

        sb.append("////////      External methods     ////////<br/>")
                .append(String.format("matrix1 + matrix2 =<br/>%s<br/>", DoubleMatrixUtils.add(matrix, matrix1)))
                .append(String.format("matrix1 - matrix2 =<br/>%s<br/>", DoubleMatrixUtils.sub(matrix, matrix1)))
                .append(String.format("matrix1 / matrix2 =<br/>%s<br/>", DoubleMatrixUtils.div(matrix, matrix1)))
                .append(String.format("matrix1 * matrix2 =<br/>%s<br/>", DoubleMatrixUtils.mul(matrix, matrix1)))
                .append(String.format("matrix1 +       2 =<br/>%s<br/>", DoubleMatrixUtils.add(matrix, 2)))
                .append(String.format("2       + matrix1 =<br/>%s<br/>", DoubleMatrixUtils.add(2, matrix)))
                .append(String.format("2       - matrix1 =<br/>%s<br/>", DoubleMatrixUtils.sub(2, matrix)))
                .append(String.format("matrix1 -       2 =<br/>%s<br/>", DoubleMatrixUtils.sub(matrix, 2)))
                .append(String.format("matrix1 *       2 =<br/>%s<br/>", DoubleMatrixUtils.mul(matrix, 2)))
                .append(String.format("2       * matrix1 =<br/>%s<br/>", DoubleMatrixUtils.mul(2, matrix)));

        sb.append(String.format("matrix + matrix:<br/>%s<br/>", DoubleMatrixUtils.add(matrix, matrix)))
                .append(String.format("matrix - matrix:<br/>%s<br/>", DoubleMatrixUtils.sub(matrix, matrix)))
                .append(String.format("matrix * matrix:<br/>%s<br/>", DoubleMatrixUtils.mul(matrix, matrix)))
                .append(String.format("matrix / matrix:<br/>%s<br/>", DoubleMatrixUtils.div(matrix, matrix)));

        DoubleMatrix[] lu = DoubleMatrixUtils.lu(matrix);
        sb.append(String.format("L  matrix:<br/>%s<br/>", lu[0]))
                .append(String.format("U  matrix:<br/>%s<br/>", lu[1]))
                .append(String.format("L * U - matrix:<br/>%s<br/>", DoubleMatrixUtils.mul(lu[0], lu[1]).sub(matrix)));

        DoubleVector b = new DoubleVector(1.0, 2.0, 3.0);
        /// x = {0.05,0.3,0.05};
        DoubleVector x = DoubleMatrixUtils.linsolve(matrix, b);
        sb.append(String.format("x vector  : %s<br/>", x))
                .append(String.format("Ax - b    : %s<br/>", DoubleMatrixUtils.mul(matrix, x).sub(b)))
                .append(String.format("A * inv(A):<br/>%s<br/>",
                        DoubleMatrixUtils.mul(matrix, Objects.requireNonNull(DoubleMatrixUtils.invert(matrix)))));

        DoubleMatrix matrix_ = new DoubleMatrix(new DoubleVector(8.0, 1.0, 6.0),
                new DoubleVector(3.0, 5.0, 7.0));

        sb.append(String.format("non rect mat:<br/>%s<br/>", matrix_))
                .append(String.format("non rect mat mul by transposed it self :<br/>%s<br/>",
                        DoubleMatrixUtils.mul(matrix_, DoubleMatrixUtils.transpose(matrix_))));

        DoubleMatrix hessian = DoubleMatrixUtils.hessian(NumericUtils.testFuncNd, new DoubleVector(0.0, 0.0, 0.0));
        sb.append(String.format("Hessian(enjoy.labs.MultiDimensional.TestFuncND):<br/>%s<br/>", hessian));

        DoubleMatrix bigMatrix = new DoubleMatrix(
                new DoubleVector(0.8147, 0.4218, 0.2769, 0.7094, 0.8909, 0.3517, 0.5688, 0.2290, 0.2599, 0.4018, 0.7803, 0.6491, 0.4868, 0.6225, 0.9049, 0.0855),
                new DoubleVector(0.9058, 0.9157, 0.0462, 0.7547, 0.9593, 0.8308, 0.4694, 0.9133, 0.8001, 0.0760, 0.3897, 0.7317, 0.4359, 0.5870, 0.9797, 0.2625),
                new DoubleVector(0.1270, 0.7922, 0.0971, 0.2760, 0.5472, 0.5853, 0.0119, 0.1524, 0.4314, 0.2399, 0.2417, 0.6477, 0.4468, 0.2077, 0.4389, 0.8010),
                new DoubleVector(0.9134, 0.9595, 0.8235, 0.6797, 0.1386, 0.5497, 0.3371, 0.8258, 0.9106, 0.1233, 0.4039, 0.4509, 0.3063, 0.3012, 0.1111, 0.0292),
                new DoubleVector(0.6324, 0.6557, 0.6948, 0.6551, 0.1493, 0.9172, 0.1622, 0.5383, 0.1818, 0.1839, 0.0965, 0.5470, 0.5085, 0.4709, 0.2581, 0.9289),
                new DoubleVector(0.0975, 0.0357, 0.3171, 0.1626, 0.2575, 0.2858, 0.7943, 0.9961, 0.2638, 0.2400, 0.1320, 0.2963, 0.5108, 0.2305, 0.4087, 0.7303),
                new DoubleVector(0.2785, 0.8491, 0.9502, 0.1190, 0.8407, 0.7572, 0.3112, 0.0782, 0.1455, 0.4173, 0.9421, 0.7447, 0.8176, 0.8443, 0.5949, 0.4886),
                new DoubleVector(0.5469, 0.9340, 0.0344, 0.4984, 0.2543, 0.7537, 0.5285, 0.4427, 0.1361, 0.0497, 0.9561, 0.1890, 0.7948, 0.1948, 0.2622, 0.5785),
                new DoubleVector(0.9575, 0.6787, 0.4387, 0.9597, 0.8143, 0.3804, 0.1656, 0.1067, 0.8693, 0.9027, 0.5752, 0.6868, 0.6443, 0.2259, 0.6028, 0.2373),
                new DoubleVector(0.9649, 0.7577, 0.3816, 0.3404, 0.2435, 0.5678, 0.6020, 0.9619, 0.5797, 0.9448, 0.0598, 0.1835, 0.3786, 0.1707, 0.7112, 0.4588),
                new DoubleVector(0.1576, 0.7431, 0.7655, 0.5853, 0.9293, 0.0759, 0.2630, 0.0046, 0.5499, 0.4909, 0.2348, 0.3685, 0.8116, 0.2277, 0.2217, 0.9631),
                new DoubleVector(0.9706, 0.3922, 0.7952, 0.2238, 0.3500, 0.0540, 0.6541, 0.7749, 0.1450, 0.4893, 0.3532, 0.6256, 0.5328, 0.4357, 0.1174, 0.5468),
                new DoubleVector(0.9572, 0.6555, 0.1869, 0.7513, 0.1966, 0.5308, 0.6892, 0.8173, 0.8530, 0.3377, 0.8212, 0.7802, 0.3507, 0.3111, 0.2967, 0.5211),
                new DoubleVector(0.4854, 0.1712, 0.4898, 0.2551, 0.2511, 0.7792, 0.7482, 0.8687, 0.6221, 0.9001, 0.0154, 0.0811, 0.9390, 0.9234, 0.3188, 0.2316),
                new DoubleVector(0.8003, 0.7060, 0.4456, 0.5060, 0.6160, 0.9340, 0.4505, 0.0844, 0.3510, 0.3692, 0.0430, 0.9294, 0.8759, 0.4302, 0.4242, 0.4889),
                new DoubleVector(0.1419, 0.0318, 0.6463, 0.6991, 0.4733, 0.1299, 0.0838, 0.3998, 0.5132, 0.1112, 0.1690, 0.7757, 0.5502, 0.1848, 0.5079, 0.6241)
        );

        sb.append(String.format("bigMatrix:<br/>%s<br/>", bigMatrix))
                .append(String.format("inv(bigMatrix):<br/>%s<br/>", DoubleMatrixUtils.invert(bigMatrix)))
                .append(String.format("bigMatrix^-1 x bigMatrix:<br/>%s<br/>",
                        DoubleMatrixUtils.mul(Objects.requireNonNull(DoubleMatrixUtils.invert(bigMatrix)), bigMatrix)))
                .append(String.format("bigMatrix x bigMatrix^-1:<br/>%s<br/><br/>",
                        DoubleMatrixUtils.mul(bigMatrix, Objects.requireNonNull(DoubleMatrixUtils.invert(bigMatrix)))));
        return sb.toString();
    }

    public DoubleMatrixDto invertMatrix(DoubleMatrixDto dto, boolean rational, boolean fullRational) {
        DoubleMatrix invert = DoubleMatrixUtils.invert(dto2Matrix(dto));

        if (Objects.isNull(invert)) {
            return null;
        }

        return matrix2dto(invert, rational, fullRational);
    }

    public DoubleMatrixDto mulMatrix(MatrixOperationDto dto, boolean rational, boolean fullRational) {
        DoubleMatrixDto leftMx = dto.getLeftMx();
        DoubleMatrixDto rightMx = dto.getRightMx();
        Double leftD = dto.getLeftD();
        Double rightD = dto.getRightD();

        if (Objects.nonNull(leftMx) && Objects.nonNull(rightMx)) {
            return matrix2dto(DoubleMatrixUtils.mul(dto2Matrix(leftMx), dto2Matrix(rightMx)), rational, fullRational);
        }

        if (Objects.nonNull(leftMx) && Objects.nonNull(rightD)) {
            return matrix2dto(DoubleMatrixUtils.mul(dto2Matrix(leftMx), rightD), rational, fullRational);
        }

        if (Objects.nonNull(rightMx) && Objects.nonNull(leftD)) {
            return matrix2dto(DoubleMatrixUtils.mul(leftD, dto2Matrix(rightMx)), rational, fullRational);
        }

        throw new IllegalArgumentException("unexpected arg");
    }

    private static DoubleMatrix dto2Matrix(DoubleMatrixDto dto) {
        List<DoubleVectorDto> rows = dto.getRows();

        // todo mb remove check
        int vectorSize = 0;
        for (DoubleVectorDto vectorDto : rows) {
            if (vectorSize != 0 && vectorDto.getSize() != vectorSize) {
                throw new IllegalArgumentException("Vectors need to have the same size");
            }
            vectorSize = vectorDto.getSize();
        }

        return new DoubleMatrix(rows.stream()
                .map(TestService::dto2Vector)
                .toArray(DoubleVector[]::new));
    }

    private static DoubleMatrixDto matrix2dto(DoubleMatrix matrix, boolean rational, boolean fullRational) {
        DoubleMatrixDto matrixDto = new DoubleMatrixDto();
        matrixDto.setRows(new ArrayList<>());

        for (int i = 0; i < matrix.rowsCount(); i++) {
            matrixDto.getRows().add(vector2dto(matrix.row(i), rational, fullRational));
        }
        return matrixDto;
    }

    private static DoubleVector dto2Vector(DoubleVectorDto dto) {
        return new DoubleVector(dto.getRow());
    }

    private static DoubleVectorDto vector2dto(DoubleVector vector, boolean rational, boolean fullRational) {
        DoubleVectorDto vectorDto = new DoubleVectorDto();
        if (rational) {
            vectorDto.setRowStr(new ArrayList<>());
            for (int i = 0; i < vector.size(); i++) {
                vectorDto.getRowStr().add(NumericUtils.toRationalStr(vector.get(i), fullRational));
            }
        } else {
            vectorDto.setRow(new ArrayList<>());
            for (int i = 0; i < vector.size(); i++) {
                vectorDto.getRow().add(vector.get(i));
            }
        }
        return vectorDto;
    }

}

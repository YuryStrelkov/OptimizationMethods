package example.rest;

import example.labs.dto.DoubleMatrixDto;
import example.labs.dto.MatrixOperationDto;
import example.service.TestService;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

/**
 * RestController for some matrix methods
 * Default URL: <a href="http://localhost:8080/test/v1">...</a>
 * Default Swagger URL: <a href="http://localhost:8080/swagger-ui/index.html">...</a>
 */
@RestController
@RequiredArgsConstructor
@RequestMapping("test/v1")
public class TestController {

    private final TestService testService;

    @GetMapping("some-test")
    public String getSomeTest() {
        return testService.someTest();
    }

    @GetMapping("vector-test")
    public String getVectorTest() {
        return testService.vectorTest();
    }

    @GetMapping("matrix-test")
    public String getMatrixTest() {
        return testService.matrixTest();
    }

    @PostMapping("matrix-invert")
    public DoubleMatrixDto invertMatrix(@RequestBody DoubleMatrixDto dto) {
        return testService.invertMatrix(dto);
    }

    @PostMapping("matrix-mul")
    public DoubleMatrixDto mulMatrix(@RequestBody MatrixOperationDto dto) {
        return testService.mulMatrix(dto);
    }
}

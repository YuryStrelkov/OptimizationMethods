// const { Vector } = require("./Math/vector.js");
// const { Matrix } = require("./Math/matrix.js");

import { Vector } from './Math/vector.js';
import { Matrix } from './Math/matrix.js';

const func = (x) => {
    return Vector.dot(x, x);
};

const vectorMathOperations = () => {
    console.log("<<<<mathOperations>>>>");
    var v1 = new Vector();
    var v2 = new Vector();
    var v3 = new Vector();
    v3.pushBack(1.0).pushBack(1.0).pushBack(1.0).pushBack(1.0);
    v1.pushBack(1.0).pushBack(2.0).pushBack(3.0).pushBack(4.0).pushBack(5.0);
    v2.pushBack(5.0).pushBack(4.0).pushBack(3.0).pushBack(2.0).pushBack(1.0);
    console.log(`v1 :                ${v1.toString()}`);
    console.log(`v2 :                ${v2.toString()}`);
    console.log(`v1 + v2 :           ${Vector.add(v1, v2).toString()}`);
    console.log(`v1 - v2 :           ${Vector.sub(v1, v2).toString()}`);
    console.log(`v1 * v2 :           ${Vector.mul(v1, v2).toString()}`);
    console.log(`v1 / v2 :           ${Vector.div(v1, v2).toString()}`);
    console.log(`v1 + 1.0 :          ${Vector.add(v1, 1.0).toString()}`);
    console.log(`v1 - 1.0 :          ${Vector.sub(v1, 1.0).toString()}`);
    console.log(`v1 * 1.0 :          ${Vector.mul(v1, 1.0).toString()}`);
    console.log(`v1 / 1.0 :          ${Vector.div(v1, 1.0).toString()}`);
    console.log(`1.0 + v2 :          ${Vector.add(1.0, v2).toString()}`);
    console.log(`1.0 - v2 :          ${Vector.sub(1.0, v2).toString()}`);
    console.log(`1.0 * v2 :          ${Vector.mul(1.0, v2).toString()}`);
    console.log(`1.0 / v2 :          ${Vector.div(1.0, v2).toString()}`);
    console.log(`|v1, v2| :          ${Vector.distance(v1, v2).toString()}`);
    console.log(`direction(v1, v2) : ${Vector.direction(v1, v2).toString()}`);
    console.log(`dot(v1, v2) :       ${Vector.dot(v1, v2).toString()}`);
    console.log(`gradient(f(x)) :\n${Vector.gradient(func, v3).toString()}`);
    console.log(`hessian (f(x)) :\n${Matrix.hessian(func, v3).toString()}`);
};

const listOperations = () => {
    console.log("<<<<listOperations>>>>");
    var v = new Vector();
    v.pushBack(1.0).pushBack(2.0).pushBack(3.0).pushBack(4.0).pushBack(5.0);
    console.log(`vector:            ${v.toString()}`);
    console.log(`v.indexOf(4.0):    ${v.indexOf(4.0)}`);
    v.remove(4.0);
    console.log(`v.remove(4.0):     ${v.toString()}`);
    v.removeAt(2);
    console.log(`v.removeAt(2):     ${v.toString()}`);
    v.insert(0, 0.0);
    console.log(`v.insert(0, 0.0):  ${v.toString()}`);
    v.insert(10, 0.0);
    console.log(`v.insert(10, 0.0): ${v.toString()}`);
    v.insert(3, 0.0);
    console.log(`v.insert(3, 0.0):  ${v.toString()}`);
    v.removeAll(0.0);
    console.log(`v.removeAll(0.0):  ${v.toString()}`);
};

const matrixMathOperations = () => {
    var mat1 = Matrix.identity(3, 3);
    var mat2 = Matrix.identity(3, 3);
    mat1.set(0, 0, 8.0);
    mat1.set(0, 1, 1.0);
    mat1.set(0, 2, 6.0);
    mat1.set(1, 0, 3.0);
    mat1.set(1, 1, 5.0);
    mat1.set(1, 2, 7.0);
    mat1.set(2, 0, 4.0);
    mat1.set(2, 1, 9.0);
    mat1.set(2, 2, 2.0);

    mat2.set(0, 0, 2.0);
    mat2.set(0, 1, 9.0);
    mat2.set(0, 2, 4.0);
    mat2.set(1, 0, 7.0);
    mat2.set(1, 1, 5.0);
    mat2.set(1, 2, 3.0);
    mat2.set(2, 0, 6.0);
    mat2.set(2, 1, 1.0);
    mat2.set(2, 2, 8.0);

    console.log(`mat1:\n${mat1.toString()}\n`);
    console.log(`mat2:\n${mat2.toString()}\n`);
    console.log(`Matrix.add(mat2, 2.0) = \n${Matrix.add(mat2, 2.0)}`);
    console.log(`Matrix.sub(mat2, 2.0) = \n${Matrix.sub(mat2, 2.0)}`);
    console.log(`Matrix.mul(mat2, 2.0) = \n${Matrix.mul(mat2, 2.0)}`);
    console.log(`Matrix.div(mat2, 2.0) = \n${Matrix.div(mat2, 2.0)}\n`);

    console.log(`Matrix.add(2.0, mat2) = \n${Matrix.add(2.0, mat2)}`);
    console.log(`Matrix.sub(2.0, mat2) = \n${Matrix.sub(2.0, mat2)}`);
    console.log(`Matrix.mul(2.0, mat2) = \n${Matrix.mul(2.0, mat2)}`);
    console.log(`Matrix.div(2.0, mat2) = \n${Matrix.div(2.0, mat2)}\n`);

    vector = new Vector();
    vector.pushBack(1.0).pushBack(1.0).pushBack(1.0);
    console.log(`Matrix.mul({1,1,1}, mat2) = \n${Matrix.mul(vector, mat2)}\n`);
    console.log(`Matrix.mul(mat2, {1,1,1}) = \n${Matrix.mul(mat2, vector)}\n`);

    console.log(`Matrix.add(mat2, mat2) = \n${Matrix.add(mat2, mat2)}`);
    console.log(`Matrix.sub(mat2, mat2) = \n${Matrix.sub(mat2, mat2)}`);
    console.log(`Matrix.mul(mat2, mat2) = \n${Matrix.mul(mat2, mat2)}`);
    console.log(`Matrix.div(mat2, mat2) = \n${Matrix.div(mat2, mat2)}`);

    console.log(`mat2 = \n${mat2}`);
    console.log(`inverted(mat2) = \n${mat2.inverted}`);
    console.log(`transposed(mat2) = \n${mat2.transposed}`);
    var arr = new Array(3);
    arr.fill(3.0);
    console.log(
        `mat2.addRow(mat2) = \n${mat2.addRows(mat2).addRows(vector).addRows(arr)}`
    );
    console.log(`mat2.addCols(mat2) = \n${mat2.addCols(mat2)}`);
    console.log(`mat2.getRow(0) = ${mat2.getRow(0)}`);
    console.log(`mat2.getRow(1) = ${mat2.getRow(1)}`);
    console.log(`mat2.getRow(2) = ${mat2.getRow(2)}`);
    console.log(`mat2.getRow(3) = ${mat2.getRow(3)}`);
    console.log(`mat2.getRow(4) = ${mat2.getRow(4)}`);
    console.log(`mat2.getRow(5) = ${mat2.getRow(5)}`);
    console.log(`mat2.getRow(6) = ${mat2.getRow(6)}`);
    console.log(`mat2.getRow(7) = ${mat2.getRow(7)}`);

    console.log(`mat2.getCol(0) = ${mat2.getCol(0)}`);
    console.log(`mat2.getCol(1) = ${mat2.getCol(1)}`);
    console.log(`mat2.getCol(2) = ${mat2.getCol(2)}`);
};

const PSI = 0.6180339887498948;

const MethodTypes = {
    Bisection: "Bisection",
    GoldenRatio: "GoldenRatio",
    Fibonacci: "Fibonacci",
    СoordinateDescent: "СoordinateDescent",
    None: "None",
};

class MethodResult {
    method;
    iterations;
    result;
    accuracy;
    probes;
    
    clear() {
        this.method = MethodTypes.None;
        this.iterations = 0;
        this.probes = 0;
        this.result = null;
        this.accuracy = 0.0;
    }

    toString() {
        return `method:     \"${this.method}\"\niterations: ${this.iterations
            }\nprobes:     ${this.probes}\naccuracy:   ${this.accuracy
            }\nresult:     ${this.result.toString()}\n`;
    }

    constructor() {
        this.clear();
    }
}

const testFunction = (x) => {
    return Vector.dot(Vector.sub(x, 1.0), x);
};

/// Для вектора размерности N минимум будет раполгаться в точке с коодиной: x_min = [0.0, 0.5, 1.0, 1.5, ..., N / 2]. 
const testFunction1 = (vect) => {
    var res = 0.;
    var index = 0;
    for(var item of vect)
        res += item * (item - index++);
    return res;
};

const bisection = (result, target, lhs, rhs, accuracy, iterations) => {
    if (!(lhs instanceof Vector && rhs instanceof Vector)) return;
    result.clear();
    result.method = MethodTypes.Bisection;
    var direction = Vector.sub(rhs, lhs).mulEqual(accuracy * 0.1);
    while (
        (result.accuracy = Vector.distance(rhs, lhs)) > 2 * accuracy &&
        result.iterations++ < iterations
    ) {
        var xc = Vector.add(rhs, lhs).mulEqual(0.5);
        var fl = target(xc.subEqual(direction));
        var fr = target(xc.addEqual(direction).addEqual(direction));
        if (fl < fr) rhs = xc;
        else lhs = xc;
    }
    result.probes = result.iterations * 2;
    result.result = Vector.add(rhs, lhs).mulEqual(0.5);
};

const goldenRatio = (result, target, lhs, rhs, accuracy, iterations) => {
    if (!(lhs instanceof Vector && rhs instanceof Vector)) return;
    result.clear();
    result.method = MethodTypes.GoldenRatio;
    var delta = Vector.sub(rhs, lhs).mulEqual(PSI);
    var x_l = Vector.sub(rhs, delta);
    var x_r = Vector.add(lhs, delta);
    var f_l = target(x_l);
    var f_r = target(x_r);
    for (
        ;
        result.iterations != iterations &&
        (result.accuracy = Vector.distance(rhs, lhs)) > 2 * accuracy;
        result.iterations++
    ) {
        if (f_l > f_r) {
            lhs = x_l;
            x_l = x_r;
            f_l = f_r;
            var delta = Vector.sub(rhs, lhs).mulEqual(PSI);
            x_r = Vector.add(lhs, delta);
            f_r = target(x_r);
        } else {
            rhs = x_r;
            x_r = x_l;
            f_r = f_l;
            var delta = Vector.sub(rhs, lhs).mulEqual(PSI);
            x_l = Vector.sub(rhs, delta);
            f_l = target(x_l);
        }
    }
    result.probes = result.iterations + 2;
    result.result = Vector.add(rhs, lhs).mulEqual(0.5);
};

const fibonacci = (result, target, lhs, rhs, accuracy) => {
    const fibonacciNext = (curr, next) => {
        return [next, curr + next];
    };

    const fibonacciPrev = (curr, next) => {
        return [next - curr, curr];
    };

    if (!(lhs instanceof Vector && rhs instanceof Vector)) return;
    result.clear();
    result.method = MethodTypes.Fibonacci;
    var iterations = 0;
    var condition = Vector.distance(rhs, lhs) / accuracy;
    var fib_1 = 1.0,
        fib_2 = 1.0;
    while (fib_2 < condition) {
        [fib_1, fib_2] = fibonacciNext(fib_1, fib_2);
        ++iterations;
    }

    var delta = Vector.sub(rhs, lhs);
    var x_l = Vector.add(lhs, Vector.mul(delta, (fib_2 - fib_1) / fib_2));
    var x_r = Vector.add(lhs, Vector.mul(delta, fib_1 / fib_2));
    var f_l = target(x_l);
    var f_r = target(x_r);
    var iteration = 0;
    for (; iteration != iterations; iteration++) {
        [fib_1, fib_2] = fibonacciPrev(fib_1, fib_2);
        if (f_l > f_r) {
            lhs = x_l;
            x_l = x_r;
            f_l = f_r;
            x_r = Vector.add(lhs, Vector.sub(rhs, lhs).mulEqual(fib_1 / fib_2));
            f_r = target(x_r);
        } else {
            rhs = x_r;
            x_r = x_l;
            f_r = f_l;
            x_l = Vector.add(
                lhs,
                Vector.sub(rhs, lhs).mulEqual((fib_2 - fib_1) / fib_2)
            );
            f_l = target(x_l);
        }
    }
    result.result = Vector.add(rhs, lhs).mulEqual(0.5);
    result.accuracy = Vector.distance(rhs, lhs);
    result.iterations = iterations;
    result.probes = result.iterations + 2;
};



export const coordinateDescent = (result, target, xStart, accuracy=1e-6, maxIterations=1000) => {
    if (!(xStart instanceof Vector))
         return;
    result.clear();
    result.method = MethodTypes.СoordinateDescent;
    var currPoint  = xStart.copy();
    var nextPoint  = xStart.copy();
    const lambdaStep = 1.0; // возможно закрываем build-in переменную
    const dimensions = currPoint.size; //размерность вектора (есть специальный Get метод)
    var currResult = new MethodResult();
    var optimalCoordinatesCount = 0; // Количество проекций, которые удовлетворяют условию |currPoint - nextPoint| < 2 * eps

    for (result.iterations = 0; result.iterations < maxIterations; result.iterations++) {
        // Получем индекс прокекции вдоль которой идёт поиск на текущей итерации 
        const itemIndex = result.iterations % dimensions; 
        // Создаем точки для исследования
        // Не делаем дополнительных копий. Модифицируем currPoint
        currPoint.set(itemIndex, currPoint.get(itemIndex) - accuracy);
        var fl = target(currPoint);
        currPoint.set(itemIndex, currPoint.get(itemIndex) + 2 * accuracy);
        var fr = target(currPoint);
        currPoint.set(itemIndex, currPoint.get(itemIndex) - accuracy);

        // Определяем промежуток неопределённости
        nextPoint.assign(currPoint); // nextPoint = currPoint;
        nextPoint.set   (itemIndex, nextPoint.get(itemIndex) + (fl > fr ? lambdaStep: -lambdaStep));
        // Ищем минимум на промежутке
        fibonacci(currResult, target, currPoint, nextPoint, accuracy);
        result.accuracy = Math.max(result.accuracy, currResult.accuracy);
        result.probes += currResult.iterations;

        // Проверяем сходимость // не сработает для функций с выраженной овражностью.
        // В случае, когда поиск начинается в точке, где вдоль искомого направления частная производная равна или близка к нулю, указанное ниже условие сразу остановит поиск.
        if(Math.abs(currPoint.get(itemIndex) - currResult.result.get(itemIndex)) < 2 * accuracy)
            optimalCoordinatesCount++;
        else
            optimalCoordinatesCount = 0;

        if(optimalCoordinatesCount == dimensions)
                break;

        currPoint.assign(currResult.result);
        // Проверяем сходимость // не сработает для функций с выраженной овражностью.
        // В случае, когда поиск начинается в точке, где вдоль искомого направления частная производная равна или близка к нулю, указанное ниже условие сразу остановит поиск.
        // if (result.iterations > 0) {
        //     result.accuracy = Vector.distance(x, xStart);
        //     if (result.accuracy < 2 * accuracy) {
        //         break;
        //     }
        // }
        // 
        // xStart = x.copy();
    }
    result.result = (currResult.result); 
};

const main = () => {
    // listOperations();
    vectorMathOperations();
    return;
    // matrixMathOperations();
    var result = new MethodResult();
    var accuracy = 1e-3;
    var iterations = 100;
    var lhs = new Vector();
    var rhs = new Vector();

    lhs.pushBack(-1.0).pushBack(-1.0).pushBack(-1.0);
    rhs.pushBack(2.0).pushBack(2.0).pushBack(2.0);

    bisection(result, testFunction, lhs, rhs, accuracy, iterations);
    console.log(result.toString());
    goldenRatio(result, testFunction, lhs, rhs, accuracy, iterations);
    console.log(result.toString());
    fibonacci(result, testFunction, lhs, rhs, 2 * accuracy);
    console.log(result.toString());

    var point = new Vector();
    point.pushBack(-4.0).pushBack(-3.0).pushBack(-2.0).pushBack(-1.0).pushBack(0.0);
    coordinateDescent(result, testFunction1, point,  accuracy);
    console.log(result.toString());
};

main();

const {Vector}  = require ('./Math/vector.js');
const {Matrix}  = require ('./Math/matrix.js');

const func = (x) =>
{
    return Vector.dot(x, x);
}

const vectorMathOperations = () =>
{
    console.log("<<<<mathOperations>>>>");
    v1 = new Vector();
    v2 = new Vector();
    v3 = new Vector();
    v3.pushBack(1.0).pushBack(1.0);
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
    console.log(`gradient(f(x)) :    ${Vector.gradient(func, v3).toString()}`); 
}

const listOperations = () => {
    console.log("<<<<listOperations>>>>");
    v = new Vector();
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
}

const matrixMathOperations = () =>{
 var mat1 = Matrix.identity(3, 3);
 var mat2 = Matrix.identity(3, 3);
 mat1.set(0, 0, 8.0); mat1.set(0, 1, 1.0); mat1.set(0, 2, 6.0);
 mat1.set(1, 0, 3.0); mat1.set(1, 1, 5.0); mat1.set(1, 2, 7.0);
 mat1.set(2, 0, 4.0); mat1.set(2, 1, 9.0); mat1.set(2, 2, 2.0);

 mat2.set(0, 0, 2.0); mat2.set(0, 1, 9.0); mat2.set(0, 2, 4.0);
 mat2.set(1, 0, 7.0); mat2.set(1, 1, 5.0); mat2.set(1, 2, 3.0);
 mat2.set(2, 0, 6.0); mat2.set(2, 1, 1.0); mat2.set(2, 2, 8.0);

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
 var arr =  new Array(3);
 arr.fill(3.0);
 console.log(`mat2.addRow(mat2) = \n${mat2.addRows(mat2).addRows(vector).addRows(arr)}`);
 console.log(`mat2.addCols(mat2) = \n${mat2.addCols(mat2)}`);
 console.log(`mat2.getRow(0) = ${mat2.getRow(0)}`)
 console.log(`mat2.getRow(1) = ${mat2.getRow(1)}`)
 console.log(`mat2.getRow(2) = ${mat2.getRow(2)}`)
 console.log(`mat2.getRow(3) = ${mat2.getRow(3)}`)
 console.log(`mat2.getRow(4) = ${mat2.getRow(4)}`)
 console.log(`mat2.getRow(5) = ${mat2.getRow(5)}`)
 console.log(`mat2.getRow(6) = ${mat2.getRow(6)}`)
 console.log(`mat2.getRow(7) = ${mat2.getRow(7)}`)

 console.log(`mat2.getCol(0) = ${mat2.getCol(0)}`)
 console.log(`mat2.getCol(1) = ${mat2.getCol(1)}`)
 console.log(`mat2.getCol(2) = ${mat2.getCol(2)}`)
}


const main = () => {
    listOperations();
    vectorMathOperations();
    matrixMathOperations();
}

main();
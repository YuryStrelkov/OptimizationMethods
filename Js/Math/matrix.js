const {
  NUMERIC_ACCURACY,
  OPERATOR_ADD,
  OPERATOR_SUB,
  OPERATOR_MUL,
  OPERATOR_DIV,
  VECTOR_AS_LEFT_OPERAND,
  VECTOR_AS_RIGHT_OPERAND,
  SCALAR_AS_LEFT_OPERAND,
  SCALAR_AS_RIGHT_OPERAND,
  MATRIX_AS_LEFT_OPERAND,
  MATRIX_AS_RIGHT_OPERAND,
  VECTOR_MATRIX_OPERATOR,
  MATRIX_VECTOR_OPERATOR,
  SCALAR_MATRIX_OPERATOR,
  MATRIX_SCALAR_OPERATOR,
  MATRIX_MATRIX_OPERATOR,
  VECTOR_VECTOR_OPERATOR,
  SCALAR_VECTOR_OPERATOR,
  VECTOR_SCALAR_OPERATOR,
  formatNumber,
  isNumber,
  operatorTypeError,
  isIterable,
  Slice,
} = require("./defines.js");
const { Vector } = require("./vector.js");

const matrixOperatorTypeError = (typeName, operatorType) => {
  operatorTypeError("Matrix", typeName, operatorType);
};

class LUSolver {
  #matrix;
  #indices;
  #resolution;
  #decomposed;

  #swapRows(row1, row2) {
    //pivoting P
    var r1 = this.#indices[row1];
    var r2 = this.#indices[row2];
    this.#indices[row2] = r1;
    this.#indices[row1] = r2;
    // swap rows
    for (var index = 0; index < this.#resolution; ++index) {
      var r1 = this.#get(row1, index);
      var r2 = this.#get(row2, index);
      this.#set(row1, index, r2);
      this.#set(row2, index, r1);
    }
  }

  #set(row, col, value) {
    this.#matrix.setUnchecked(row * this.#resolution + col, value);
  }

  #get(row, col) {
    return this.#matrix.getUnchecked(row * this.#resolution + col);
  }

  #decomposeLU() {
    var returnValue = true;
    var i, j, k, imax, maxA, absA;
    for (i = 0; i <= this.#resolution; i++) this.#indices[i] = i;
    //Unit permutation matrix, P[N] initialized with N
    for (i = 0; i < this.#resolution; i++) {
      maxA = 0.0;
      imax = i;

      for (k = i; k < this.#resolution; k++)
        if ((absA = Math.abs(this.#get(k, i))) > maxA) {
          maxA = absA;
          imax = k;
        }

      if (maxA < NUMERIC_ACCURACY) {
        returnValue = false; //failure, matrix is degenerate
        break;
      }

      if (imax != i) {
        //pivoting P
        this.#swapRows(i, imax);
        //counting pivots starting from N (for determinant)
        this.#indices[this.#resolution]++;
      }

      for (j = i + 1; j < this.#resolution; j++) {
        this.#set(j, i, this.#get(j, i) / this.#get(i, i));
        for (k = i + 1; k < this.#resolution; k++)
          this.#set(j, k, this.#get(j, k) - this.#get(j, i) * this.#get(i, k));
      }
    }
    return returnValue; //decomposition done
  }

  constructor(resolution, data) {
    this.#resolution = resolution;
    this.#matrix = new Vector();
    this.#matrix.append(data);
    this.#indices = new Array(resolution + 1);
    this.#decomposed = this.#decomposeLU();
  }

  solve(result) {
    if (!this.#decomposed) return false; //  new Vector(this.#resolution);
    // Тип LUSolver определён до Matrix, но при выполнении обращения матрицы считает, что Matrix уже определён. Это плохо.
    var b = new Vector(this.#resolution);

    var row, col, k;

    for (col = 0; col < this.#resolution; col++) {
      b.setUnchecked(col, 1.0);
      for (row = 0; row < this.#resolution; row++) {
        result.setUnchecked(row, col, b.getUnchecked(this.#indices[row]));
        for (k = 0; k < row; k++)
          result.setUnchecked(
            row,
            col,
            result.getUnchecked(row, col) -
              this.#get(row, k) * result.getUnchecked(k, col)
          );
      }

      for (row = this.#resolution - 1; row >= 0; row--) {
        for (k = row + 1; k < this.#resolution; k++)
          result.setUnchecked(
            row,
            col,
            result.getUnchecked(row, col) -
              this.#get(row, k) * result.getUnchecked(k, col)
          );
        result.setUnchecked(
          row,
          col,
          result.getUnchecked(row, col) / this.#get(row, row)
        );
      }
      b.setUnchecked(col, 0.0);
    }
    return true;
  }
}

class Matrix extends Vector {
  #cols;
  #rows;

  #accessGuard(row, col) {
    if (col < 0 || col >= this.#cols)
      throw new Error(
        `Index of col ${col} is out of Matrix columns indices...`
      );
    if (row < 0 || row >= this.#rows)
      throw new Error(
        `Index of col ${row} is out of Matrix columns indices...`
      );
  }

  static #multiplication(lhs, rhs) {
    if (lhs.cols != rhs.rows)
      throw new Error("Error matrix multiplication::a.cols != b.rows");
    // Возможно версия в лоб для наших целей будет быстрее, чем транспонирование и поэлементное
    var result = new Matrix(lhs.rows, rhs.cols);
    var rowIndex, colIndex, index;
    for (rowIndex = 0; rowIndex < lhs.rows; rowIndex++) {
      for (colIndex = 0; colIndex < rhs.cols; colIndex++) {
        var res = 0.0;
        for (index = 0; index < rhs.rows; index++)
          res +=
            rhs.getUnchecked(index, colIndex) *
            lhs.getUnchecked(rowIndex, index);
        result.setUnchecked(rowIndex, colIndex, res);
      }
    }
    return result;
  }

  static #vectorMultiplicationLeft(matrix, vector) {
    if (matrix.cols != vector.size)
      throw new Error("Error matrix multiplication::a.cols != b.rows");
    var result = new Matrix(matrix.rows, 1);
    for (var index = 0; index < matrix.size; ++index) {
      var row = (index / matrix.cols) | 0;
      var col = index % matrix.cols;
      result.setUnchecked(
        row,
        0,
        result.getUnchecked(row, 0) +
          matrix.getUnchecked(row, col) * vector.getUnchecked(col)
      );
    }
    return result;
  }

  static #vectorMultiplicationRight(vector, matrix) {
    if (matrix.rows != vector.size)
      throw new Error("Error matrix multiplication::a.cols != b.rows");
    var result = new Matrix(1, matrix.cols);
    for (var index = 0; index < matrix.size; ++index) {
      var row = (index / matrix.cols) | 0;
      var col = index % matrix.cols;
      result.setUnchecked(
        0,
        col,
        result.getUnchecked(0, col) +
          matrix.getUnchecked(row, col) * vector.getUnchecked(row)
      );
    }
    return result;
  }

  static inversion(orig, res) {
    if (orig.rows !== orig.cols)
      console.log(
        new Error("Matrix::Inversion::Matrix has to be square matrix...")
      );

    var solver = new LUSolver(orig.rows, orig);
    var returnValue = false;

    if ((returnValue = solver.solve(res))) {
      res.#rows = orig.rows;
      res.#cols = orig.cols;
    }
    return returnValue;
  }

  unaryOperator(other, operatorType) {
    if (isNumber(other)) super.unaryOperator(other, operatorType);
    else if (other instanceof Matrix) {
      switch (operatorType) {
        case OPERATOR_ADD:
        case OPERATOR_SUB:
          if (other.cols === this.cols && other.rows === this.rows)
            super.unaryOperator(other, operatorType);
          else matrixOperatorTypeError(typeof other, operatorType);
          break;
        case OPERATOR_MUL:
          this.swap(Matrix.#multiplication(this, other));
          break;
        case OPERATOR_DIV:
          var iOther = new Matrix(this.#rows, this.#cols);
          if (Matrix.inversion(other, iOther))
            this.swap(Matrix.#multiplication(this, iOther));
          break;
        default:
          matrixOperatorTypeError(typeof other, operatorType);
          break;
      }
    } else if (other instanceof Vector)
      this.swap(Matrix.#vectorMultiplicationLeft(this, other));
    return this;
  }

  static binaryOperator(lhs, rhs, operatorType) {
    var operandsCase = 0;
    operandsCase |=
      lhs instanceof Vector && !(lhs instanceof Matrix)
        ? VECTOR_AS_LEFT_OPERAND
        : 0;
    operandsCase |=
      rhs instanceof Vector && !(rhs instanceof Matrix)
        ? VECTOR_AS_RIGHT_OPERAND
        : 0;
    operandsCase |= lhs instanceof Matrix ? MATRIX_AS_LEFT_OPERAND : 0;
    operandsCase |= rhs instanceof Matrix ? MATRIX_AS_RIGHT_OPERAND : 0;
    operandsCase |= isNumber(lhs) ? SCALAR_AS_LEFT_OPERAND : 0;
    operandsCase |= isNumber(rhs) ? SCALAR_AS_RIGHT_OPERAND : 0;
    switch (operandsCase) {
      case VECTOR_VECTOR_OPERATOR:
      case SCALAR_VECTOR_OPERATOR:
      case VECTOR_SCALAR_OPERATOR:
        return Vector.binaryOperator(lhs, rhs, operatorType);
      case MATRIX_MATRIX_OPERATOR:
      case MATRIX_SCALAR_OPERATOR:
      case MATRIX_VECTOR_OPERATOR:
        var result = lhs.copy();
        return result.unaryOperator(rhs, operatorType);
      case SCALAR_MATRIX_OPERATOR:
        var result = rhs.copy();
        result.rightScalarOperator(lhs, operatorType);
        return result;
      case VECTOR_MATRIX_OPERATOR:
        return Matrix.#vectorMultiplicationRight(lhs, rhs);
      default:
        console.error(
          new Error(
            `Matrix math unsupported upon lhs of \"${typeof lhs}\" type and rhs of \"${typeof rhs}\" type...`
          )
        );
    }
  }

  constructor(rows = 3, cols = 3) {
    super(Math.max(1, rows) * Math.max(1, cols));
    this.#cols = Math.max(1, cols);
    this.#rows = Math.max(1, rows);
  }

  get cols() {
    return this.#cols;
  }

  get rows() {
    return this.#rows;
  }

  get inverted() {
    if (this.#rows === this.#cols) {
      var result = new Matrix(this.#rows, this.#cols);
      Matrix.inversion(this, result);
      return result;
    }
    console.log(new Error(`It is possible to invert only square matrices...`));
  }

  invert() {
    this.swap(this.inverted);
    return this;
  }

  get transposed() {
    var trans = new Matrix(this.cols, this.rows);
    for (var row = 0; row < this.rows; row++)
      for (var col = 0; col < this.cols; col++)
        trans.setUnchecked(col, row, this.getUnchecked(row, col));
    return trans;
  }

  transpose() {
    this.swap(this.transposed);
    return this;
  }

  #addRowIterable(iterable, elementsCount) {
    this.reserve(this.size + elementsCount);
    this.append(iterable);
    this.#rows += 1;
  }

  addRows(row) {
    if (row instanceof Vector && !(row instanceof Matrix)) {
      if (row.size === this.cols) {
        this.#addRowIterable(row, row.size);
      } else {
        console.error(
          new Error(
            `Unable to append values from vector of size \"${row.size}\"...`
          )
        );
      }
    } else if (row instanceof Matrix) {
      if (row.cols === this.cols) {
        if(row === this) row = row.copy();
        for(var r of row.getRows())
          this.#addRowIterable(r, r.length);
      } else {
        console.error(
          new Error(
            `Unable to append rows from matrix with cols count \"${row.cols}\"...`
          )
        );
      }
    } else if ((row instanceof Array && row.length  === this.cols) || (row instanceof Slice && row.length  === this.cols)) {
      this.#addRowIterable(row, row.length);
    } else {
      console.error(
        new Error(
          `Unable to append rows from argument of type \"${typeof row}\"...`
        )
      );
    }
    return this;
  }

  #addColsIterable(iterable, elementsCount) {
    this.reserve(this.size + elementsCount);
    var index = 0, shift = 0;
    for (var value of iterable) 
      this.insert(this.cols * ((index++) + 1) + (shift++), value);
    this.#cols += 1;
  }

  addCols(col) {
    if (col instanceof Vector && !(col instanceof Matrix)) {
      if (col.size === this.rows) {
        this.#addColsIterable(col, col.size);
      } else {
        console.error(
          new Error(
            `Unable to append values from vector of size \"${col.size}\"...`
          )
        );
      }
    } else if (col instanceof Matrix) {
      if (col.rows === this.rows) {
        if(col === this) col = col.copy();
        for (var _col of col.getCols()) this.#addColsIterable(_col, _col.size);
      } else {
        console.error(
          new Error(
            `Unable to append rows from matrix with cols count \"${col.cols}\"...`
          )
        );
      }
    } else if (
      (col instanceof Array && col.length === this.rows) ||
      (col instanceof Slice && col.get.length === this.rows)
    ) {
      this.#addColsIterable(col, this.rows);
    } else {
      console.error(
        new Error(
          `Unable to append rows from argument of type \"${typeof col}\"...`
        )
      );
    }
    return this;
  }

  getUnchecked(row, col) {
    return super.getUnchecked(col + row * this.#cols);
  }

  setUnchecked(row, col, value) {
    super.setUnchecked(col + row * this.#cols, value);
  }

  get(row, col) {
    this.#accessGuard(row, col);
    return super.getUnchecked(col + row * this.#cols);
  }

  set(row, col, value) {
    this.#accessGuard(row, col);
    super.setUnchecked(col + row * this.#cols, value);
  }

  getRow(row) {
    return new Slice(super.data, row * this.cols, (row + 1) * this.cols, 1);
  }

  getCol(col) {
    return new Slice(super.data, col, col + this.cols * this.rows, this.cols);
  }

  *getRows() {
    for (var index = 0; index < this.rows; ++index) yield this.getRow(index);
  }

  *getCols() {
    for (var index = 0; index < this.cols; ++index) yield this.getCol(index);
  }

  copy() {
    return new Matrix(this.#rows, this.#cols).assign(this);
  }

  swap(other) {
    if (other instanceof Matrix && other !== this) {
      super.swap(other);
      this.#rows = other.#rows;
      other.#rows = 0;
      this.#cols = other.#cols;
      other.#cols = 0;
    }
    return this;
  }

  // ex.: this += value
  addEqual(value) {
    return this.unaryOperator(value, OPERATOR_ADD);
  }

  // ex.: this -= value
  subEqual(value) {
    return this.unaryOperator(value, OPERATOR_SUB);
  }

  // ex.: this *= value
  mulEqual(value) {
    return this.unaryOperator(value, OPERATOR_MUL);
  }

  // ex.: this /= value
  divEqual(value) {
    return this.unaryOperator(value, OPERATOR_DIV);
  }

  /// ex.: result = vector | scalar + vector | scalar
  static add(lhs, rhs) {
    return Matrix.binaryOperator(lhs, rhs, OPERATOR_ADD);
  }

  /// ex.: result = vector | scalar - vector | scalar
  static sub(lhs, rhs) {
    return Matrix.binaryOperator(lhs, rhs, OPERATOR_SUB);
  }

  /// ex.: result = vector | scalar * vector | scalar
  static mul(lhs, rhs) {
    return Matrix.binaryOperator(lhs, rhs, OPERATOR_MUL);
  }

  /// ex.: result = vector | scalar / vector | scalar
  static div(lhs, rhs) {
    return Matrix.binaryOperator(lhs, rhs, OPERATOR_DIV);
  }

  forEach(callback) {
    super.forEach(callback);
    return this;
  }

  toString() {
    return `[\n\t${[...this.getRows()]
      .map((row) => {
        return row.toString();
      })
      .join(",\n\t")}\n]`;
  }

  static identity(rows, cols) {
    var res = new Matrix(rows, cols);
    for (var index = 0; index < Math.min(rows, cols); index++)
      res.setUnchecked(index, index, 1.0);
    return res;
  }

  static hessian(functionND, point, step) {
    if (point instanceof Vector) {
      var res = new Matrix(point.size, point.size);
      for (var row = 0; row < res.rows; ++row) {
        for (var col = 0; col <= row; ++col) {
          var val = Vector.partial2(functionND, point, row, col, step);
          res.setUnchecked(row, col, val).setUnchecked(col, row, val);
        }
      }
      return res;
    }
    console.log(new Error("hessian"));
  }
}

module.exports = { Matrix };

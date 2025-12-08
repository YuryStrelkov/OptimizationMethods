const {
  NUMERIC_ACCURACY,
  OPERATOR_ADD,
  OPERATOR_SUB,
  OPERATOR_MUL,
  OPERATOR_DIV,
  VECTOR_VECTOR_OPERATOR,
  SCALAR_VECTOR_OPERATOR,
  VECTOR_SCALAR_OPERATOR,
  VECTOR_AS_LEFT_OPERAND,
  VECTOR_AS_RIGHT_OPERAND,
  SCALAR_AS_LEFT_OPERAND,
  SCALAR_AS_RIGHT_OPERAND,
  formatNumber,
  isNumber,
  operatorTypeError,
  isIterable,
} = require("./defines.js");


const vectorOperatorTypeError = (typeName, operatorType) => {
  operatorTypeError("Vector", typeName, operatorType);
};

/**
 * Vector
 *
 * A simple resizable numeric vector with array-backed storage, numeric utilities and
 * element-wise arithmetic operations. Instances manage an internal capacity and a
 * current filling count (size). Most mutating methods return the instance to allow
 * chaining.
 *
 * The class is intended for numeric computations and uses a small numeric tolerance
 * (NUMERIC_ACCURACY) for equality checks and finite-difference approximations.
 *
 * Usage overview:
 * - Construct with an optional initial size: new Vector(size)
 * - Read-only properties: size, length, lengthSquared, isEmpty, data (underlying array)
 * - Mutating instance operations: pushBack, insert, removeAt, remove, removeAll, clear,
 *   normalize, addEqual/subEqual/mulEqual/divEqual, assign, swap, append, forEach
 * - Non-mutating helpers that return new Vector instances: copy(), static add/sub/mul/div,
 *   static gradient/direction/distance
 * - Numeric utilities: static dot, static partial/partial2 (finite differences)
 *
 * @class
 * @classdesc Resizable numeric vector with element-wise and scalar arithmetic helpers,
 *             iteration support, and finite-difference helpers for numeric derivatives.
 *
 * @param {number} [size=0] Initial number of elements. If > 0 the backing storage is
 *                          allocated and filled with 0.0. If omitted or 0, the vector
 *                          starts empty with zero capacity until first push/insert.
 *
 * @property {Array<number>|null} data
 *           Underlying storage array. May be null for an empty/unallocated Vector.
 *           This is provided for diagnostic/interop use; mutating this directly may
 *           break invariants (prefer the provided API).
 * @property {number} size
 *           Number of elements currently stored (filling).
 * @property {number} lengthSquared
 *           Sum of squares of components (||v||^2).
 * @property {number} length
 *           Euclidean norm (sqrt(lengthSquared)).
 * @property {boolean} isEmpty
 *           True when size === 0.
 * @property {Vector} normalized
 *           Returns a normalized copy of the vector (does not mutate the original).
 *
 * Instance methods (high level):
 * @method reserve
 * @param {number} [newCapacity=-1] Ensure at least newCapacity or apply growth heuristic.
 *
 * @method clear
 * @returns {this} Clears the vector (size -> 0).
 *
 * @method normalize
 * @returns {this} Normalize in-place. If length is approximately zero (NUMERIC_ACCURACY)
 *                 the vector is left unchanged.
 *
 * @method getUnchecked
 * @param {number} index
 * @returns {number} Returns element without bounds checking.
 *
 * @method setUnchecked
 * @param {number} index
 * @param {number} value
 * @returns {void} Set element without bounds checking.
 *
 * @method get
 * @param {number} index
 * @returns {number} Get element with bounds checking; throws Error if out of range.
 *
 * @method set
 * @param {number} index
 * @param {number} value
 * @returns {void} Set element with bounds checking; throws Error if out of range.
 *
 * @method append
 * @param {Iterable<number>} iterableSequence
 * @returns {this} Append all items from an iterable, converting items with Number(...).
 *
 * @method pushBack
 * @param {number} value
 * @returns {this} Append a single numeric value (grows capacity as needed).
 *
 * @method insert
 * @param {number} position
 * @param {number} value
 * @returns {this} Insert value at position (if position > size behaves like pushBack).
 *
 * @method removeAt
 * @param {number} position
 * @returns {boolean} Remove element at position; returns false for out-of-range.
 *
 * @method remove
 * @param {number} value
 * @returns {boolean} Remove first occurrence of value (within NUMERIC_ACCURACY), false if not found.
 *
 * @method removeAll
 * @param {number} value
 * @returns {number} Remove all occurrences of value (within NUMERIC_ACCURACY) and return count removed.
 *
 * @method indexOf
 * @param {number} value
 * @returns {number} Index of first occurrence (within NUMERIC_ACCURACY) or -1.
 *
 * @method forEach
 * @param {function(number, number=): number} callback
 * @returns {this} Calls callback for each element and replaces each element with the callback return value.
 *
 * @method copy
 * @returns {Vector} Shallow copy: new Vector with the same capacity/filling and elements up to the filling.
 *
 * @method swap
 * @param {Vector} other
 * @returns {this} Swap internal storage with another Vector instance. No-op if same instance or not a Vector.
 *
 * @method assign
 * @param {Vector|Iterable<number>} other
 * @returns {this} Clear and assign values from another Vector or iterable (no-op if same instance).
 *
 * @method addEqual
 * @method subEqual
 * @method mulEqual
 * @method divEqual
 * @param {number|Array<number>|Vector} value
 * @returns {this} In-place element-wise or scalar arithmetic. Accepts numbers, arrays/iterables or Vector.
 *
 * @method unaryOperator
 * @private
 * @param {number|Vector} other
 * @param {number} operatorType
 * @returns {this} Internal helper used by addEqual/subEqual/mulEqual/divEqual.
 *
 * Static methods:
 * @method static add
 * @method static sub
 * @method static mul
 * @method static div
 * @param {Vector|number} lhs
 * @param {Vector|number} rhs
 * @returns {Vector} Returns a new Vector that is the result of element-wise or mixed scalar operations.
 *                   Supports vector-vector, vector-scalar and scalar-vector combinations.
 *
 * @method static distance
 * @param {Vector|Array<number>} lhs
 * @param {Vector|Array<number>} rhs
 * @returns {number} Euclidean distance between two vectors (uses min dimension).
 *
 * @method static direction
 * @param {Vector} lhs
 * @param {Vector} rhs
 * @returns {Vector} Returns a normalized vector pointing from lhs to rhs (result is a Vector).
 *
 * @method static dot
 * @param {Vector} lhs
 * @param {Vector} rhs
 * @returns {number} Dot product computed up to the min of the two sizes.
 *
 * @method static partial
 * @param {function(Vector): number} func
 * @param {Vector} arg
 * @param {number} index
 * @param {number} [step=NUMERIC_ACCURACY]
 * @returns {number} Finite-difference approximation of partial derivative w.r.t. index.
 *
 * @method static partial2
 * @param {function(Vector): number} func
 * @param {Vector} arg
 * @param {number} index1
 * @param {number} index2
 * @param {number} [step=NUMERIC_ACCURACY]
 * @returns {number} Mixed second-order finite-difference approximation.
 *
 * @method static gradient
 * @param {function(Vector): number} func
 * @param {Vector} arg
 * @param {number} [step=NUMERIC_ACCURACY]
 * @returns {Vector} Numerical gradient vector of func at arg (uses partial).
 *
 * Errors and special notes:
 * - get/set throw Error when index is out of valid range.
 * - Many operations accept array-like/iterable operands and convert values with Number(...).
 * - Numeric comparisons use a global NUMERIC_ACCURACY tolerance; equality/removal/indexing
 *   rely on that tolerance.
 *
 * @throws {Error} If constructor requested size exceeds safe integer limits (Number.MAX_SAFE_INTEGER)
 */
class Vector {
 
  #data;
  #capacity;
  #filling;

  

 static #evalOperandsCase(lhs, rhs){
    var operandsCase = 0;
    operandsCase |= lhs instanceof Vector ? VECTOR_AS_LEFT_OPERAND : 0;
    operandsCase |= rhs instanceof Vector ? VECTOR_AS_RIGHT_OPERAND : 0;
    operandsCase |= isNumber(lhs) ? SCALAR_AS_LEFT_OPERAND : 0;
    operandsCase |= isNumber(rhs) ? SCALAR_AS_RIGHT_OPERAND : 0;
    return operandsCase;
 }

  /**
   * Ensures that the internal storage has at least the specified capacity.
   * If the current capacity is zero, initializes the storage with a default size.
   * If a new capacity is provided, sets the capacity to the maximum of 1 and newCapacity.
   * If the storage is full, increases the capacity by approximately doubling it.
   * Copies existing elements to the new storage.
   *
   * @param {number} [newCapacity=-1] - The desired minimum capacity. If -1, uses current logic to determine capacity.
   */
  reserve(newCapacity = -1) {
    
    if (this.#capacity == 0) {
      this.#capacity = 10;
      this.#data = new Array(this.#capacity);
      this.#data.fill(0.0);
    } else {
      var reallocate = false;
  
      if(newCapacity !== -1){
        reallocate =  true;
        this.#capacity = Math.max(1, newCapacity);
      }    
      
      if (this.#filling === this.#capacity){
        reallocate = true;
        this.#capacity = (this.#capacity >> 1) + this.#capacity;
      }
  
      if(reallocate){
        var data = new Array(this.#capacity);
        data.fill(0.0);
        for (var index = 0; index < this.#filling; ++index)
          data[index] = this.#data[index];
        this.#data = data;
      }
    }
  }

  /**
   * Checks if the provided index is within the valid range of the array.
   * Throws an error if the index is out of bounds.
   *
   * @private
   * @param {number} index - The index to validate.
   * @throws {Error} If the index is less than 0 or greater than the current filling.
   */
  #accessGuard(index) {
    if (index < 0 || index > this.#filling)
      throw new Error(`Index ${index} is out of array indices...`);
  }
  
  
  /**
   * Applies a scalar operation to each element of the vector, using the provided operator type and left operand.
   *
   * @param {number} left - The scalar value to use as the left operand in the operation.
   * @param {string} operatorType - The type of operation to perform. Supported values are:
   *   - OPERATOR_ADD: Addition
   *   - OPERATOR_SUB: Subtraction
   *   - OPERATOR_MUL: Multiplication
   *   - OPERATOR_DIV: Division
   * @returns {this} The modified vector instance after applying the operation.
   * @throws {TypeError} If the operatorType is not supported.
   */
  rhsScalarOperator(rhs, operatorType) {
    switch (operatorType) {
      case OPERATOR_ADD:
        this.forEach((val) => {
          return val + rhs;
        });
        break;
      case OPERATOR_SUB:
        this.forEach((val) => {
          return val - rhs;
        });
        break;
      case OPERATOR_MUL:
        this.forEach((val) => {
          return val * rhs;
        });
        break;
      case OPERATOR_DIV:
        rhs = 1.0 / rhs;
        this.forEach((val) => {
          return val * rhs;
        });
        break;
      default:
        vectorOperatorTypeError(typeof rhs, operatorType);
    }
    return this;
  }

  
  /**
   * Applies a scalar operation to each element of the vector, using the scalar `right` as the left operand.
   * Supported operations are addition, subtraction, multiplication, and division.
   * The operation is determined by `operatorType`.
   *
   * @param {number} lhs - The scalar value to use as the left operand in the operation.
   * @param {string} operatorType - The type of operation to perform. Should be one of OPERATOR_ADD, OPERATOR_SUB, OPERATOR_MUL, or OPERATOR_DIV.
   * @returns {this} Returns the modified vector instance.
   * @throws {TypeError} Throws if the operatorType is invalid.
   */
  lhsScalarOperator(lhs, operatorType) {
    switch (operatorType) {
      case OPERATOR_ADD:
        this.forEach((val) => {
          return lhs + val;
        });
        break;
      case OPERATOR_SUB:
        this.forEach((val) => {
          return lhs - val;
        });
        break;
      case OPERATOR_MUL:
        this.forEach((val) => {
          return lhs * val;
        });
        break;
      case OPERATOR_DIV:
        this.forEach((val) => {
          return lhs / val;
        });
        break;
      default:
        vectorOperatorTypeError(typeof lhs, operatorType);
    }
    return this;
  }

 
  /**
   * Applies a binary operator element-wise between the current vector and the `left` vector,
   * modifying the current vector in place. The operation is performed up to the minimum length
   * of the two vectors.
   *
   * @param {Vector} left - The left-hand side vector for the operation.
   * @param {number} operatorType - The type of operator to apply (e.g., OPERATOR_ADD, OPERATOR_SUB, OPERATOR_MUL, OPERATOR_DIV).
   * @returns {Vector} The modified current vector after applying the operation.
   * @throws {TypeError} If the operatorType is invalid.
   */
  rhsVectorOperator(rhs, operatorType) {
    let nItems = Math.min(this.#filling, rhs.#filling);
    var index = 0;
    switch (operatorType) {
      case OPERATOR_ADD:
        this.forEach((value) => {
          return index < nItems ? value + rhs.#data[index++] : value;
        });
        break;
      case OPERATOR_SUB:
        this.forEach((value) => {
          return index < nItems ? value - rhs.#data[index++] : value;
        });
        break;
      case OPERATOR_MUL:
        this.forEach((value) => {
          return index < nItems ? value * rhs.#data[index++] : value;
        });
        break;
      case OPERATOR_DIV:
        this.forEach((value) => {
          return index < nItems ? value / rhs.#data[index++] : value;
        });
        break;
      default:
        vectorOperatorTypeError(typeof rhs, operatorType);
    }
    return this;
  }

 
  /**
   * Applies a binary operator to two operands, which can be vectors or scalars.
   *
   * The operation performed depends on the types of the left-hand side (lhs) and right-hand side (rhs) operands:
   * - Vector-Vector: Applies the operator to two vectors.
   * - Vector-Scalar: Applies the operator to a vector and a scalar (scalar on the right).
   * - Scalar-Vector: Applies the operator to a scalar and a vector (scalar on the left).
   *
   * @static
   * @param {(Vector|number)} lhs - The left-hand side operand, either a Vector instance or a number.
   * @param {(Vector|number)} rhs - The right-hand side operand, either a Vector instance or a number.
   * @param {number} operatorType - The type of binary operator to apply.
   * @returns {Vector|undefined} The result of the operation as a new Vector, or undefined if the operand types are unsupported.
   */
  static binaryOperator(lhs, rhs, operatorType) {

    switch (Vector.#evalOperandsCase(lhs, rhs)) {
      case VECTOR_VECTOR_OPERATOR:
        var result = lhs.copy();
        return result.rhsVectorOperator(rhs, operatorType);

      case VECTOR_SCALAR_OPERATOR:
        var result = lhs.copy();
        return result.rhsScalarOperator(rhs, operatorType);
      
      case SCALAR_VECTOR_OPERATOR:
        var result = rhs.copy();
        return result.lhsScalarOperator(lhs, operatorType);
      
      default:
        console.error(
          new Error(
            `Vector math unsupported upon lhs of \"${typeof lhs}\" type and rhs of \"${typeof rhs}\" type...`
          )
        );
    }
  }

  /**
   * Applies a unary operator to the current vector instance with the provided operand.
   * If the operand is a Vector, delegates to leftVectorOperator.
   * If the operand is a number, delegates to rhsScalarOperator.
   * Throws an error if the operand type is unsupported.
   *
   * @param {Vector|number} other - The operand to apply the operator with.
   * @param {number} operatorType - The type of operator to apply [0 - addition, 1 - subtraction,
   *  2 - multiplication, 3 - division].
   * @returns {Vector} The result of the operation.
   * @throws {TypeError} If the operand type is not supported.
   */
  unaryOperator(other, operatorType) {
    if(other instanceof Vector)
      return this.rhsVectorOperator(other, operatorType);

    if(isNumber(other))
      return this.rhsScalarOperator(other, operatorType);

    vectorOperatorTypeError(typeof other, operatorType);
  }

  /// Public section
  /**
   * Creates a new Vector instance with the specified size.
   * Initializes the internal data array with zeros if size > 0.
   * Logs an error if the requested size exceeds Number.MAX_SAFE_INTEGER.
   *
   * @param {number} size - The desired size of the vector.
   */
  constructor(size ) {
    if (Number.MAX_SAFE_INTEGER < size)
      console.error(
        new Error(`Vector size ${size} is to big to allocate new one...`)
      );
    this.#filling = 0;
    this.#capacity = 0;
    this.#data = null;
    if (size > 0) {
      this.#filling = size;
      this.#capacity = size;
      this.#data = new Array(this.#capacity);
      this.#data.fill(0.0);
    }
  }

  [Symbol.iterator]() {
    let currentIndex = 0;
    const elements = this.#data; // Capture 'this.elements' for the iterator's scope
    const elementsCount = this.#filling;
    // Returns the iterator object
    return {
      // Implements the iterator protocol
      next() {
        if (currentIndex < elementsCount)
          return { value: elements[currentIndex++], done: false };
        else return { done: true };
      },
    };
  }

  /**
   * Returns a string representation of the vector, displaying its elements
   * formatted and separated by commas within square brackets.
   * 
   * @returns {string} The formatted string representation of the vector.
   */
  toString() {
    return `[ ${this.#data
      .slice(0, this.#filling)
      .map((v) => {
        return formatNumber(v);
      })
      .join(", ")} ]`;
  }

  /**
   * Gets the internal data of the vector.
   * @type {Array<number>}
   * @readonly
   */
  get data(){
    return this.#data;
  }

  /**
   * Gets the number of elements in the vector.
   * @type {number}
   * @readonly
   */
  get size() {
    return this.#filling;
  }

  /**
   * Calculates and returns the sum of the squares of the vector's elements.
   * This is equivalent to the squared length (magnitude) of the vector.
   * @returns {number} The sum of squares of the vector's elements.
   */
  get lengthSquared() {
    return this.#data
      .slice(0, this.#filling)
      .reduce((accumulator, curr) => accumulator + curr * curr, 0.0);
  }

  /**
   * Gets the magnitude (Euclidean length) of the vector.
   * @returns {number} The length of the vector.
   */
  get length() {
    return Math.sqrt(this.lengthSquared);
  }

  /**
   * Checks if the vector is empty (contains no elements).
   * @returns {boolean} True if the vector is empty, otherwise false.
   */
  get isEmpty() {
    return this.#filling == 0;
  }

  /**
   * Clears the vector by resetting its filling to zero.
   * @returns {this} The current instance for chaining.
   */
  clear()
  {
    this.#filling = 0;
    return this;
  }

  /**
   * Normalizes the vector to have a length of 1.
   * If the vector's length is close to zero (less than NUMERIC_ACCURACY), 
   * it logs a warning and returns the vector unchanged.
   * Otherwise, scales each component so that the vector's length becomes 1.
   * @returns {this} The normalized vector instance.
   */
  normalize() {
    var len = this.length;
    if (len < NUMERIC_ACCURACY) {
      console.log("Vector length is close to zero...");
      return this;
    }
    len = 1.0 / len;
    for (var index = 0; index < this.#filling; index++)
      this.#data[index] *= len;
    return this;
  }

  /**
   * Returns a normalized (unit length) copy of the current vector.
   * The original vector remains unchanged.
   * @returns {Vector} A new vector instance with unit length.
   */
  get normalized() {
    var copyOfThis = this.copy();
    return copyOfThis.normalize();
  }

  getUnchecked(index) {
    return this.#data[index];
  }

  setUnchecked(index, value) {
    this.#data[index] = Number(value);
  }

  get(index) {
    this.#accessGuard(index);
    return this.#data[index];
  }

  set(index, value) {
    this.#accessGuard(index);
    this.#data[index] = Number(value);
  }

  append(iterableSequence) {
    if (!isIterable(iterableSequence))
      console.error(new Error(`Argument \"${iterableSequence}\" is not iterable.`));
    else for (const item of iterableSequence) this.pushBack(item);
    return this;
  }

  pushBack(value) {
    this.reserve();
    this.#data[this.#filling++] = Number(value);
    return this;
  }

  insert(position, value) {
    if (position > this.#filling) return this.pushBack(value);
    this.reserve();
    for (var index = this.#filling; index > position; --index)
      this.#data[index] = this.#data[index - 1];
    this.#data[position] = Number(value);
    this.#filling++;
    return this;
  }

  removeAt(position) {
    if (position >= this.#filling) return false;
    for (var index = position; index < this.#filling - 1; ++index)
      this.#data[index] = this.#data[index + 1];
    this.#filling--;
    return true;
  }

  remove(value) {
    return this.removeAt(this.indexOf(value));
  }

  removeAll(value) {
    var deletions = 0;
    for (var index = 0; index < this.#filling; ++index)
      if (Math.abs(this.#data[index] - value) < NUMERIC_ACCURACY) {
        this.removeAt(index);
        deletions++;
      }
    return deletions;
  }

  indexOf(value) {
    var returnValue = -1;
    for (var index = 0; index < this.#filling; ++index)
      if (Math.abs(this.#data[index] - value) < NUMERIC_ACCURACY) {
        returnValue = index;
        break;
      }
    return returnValue;
  }

  forEach(callback) {
    this.#data.slice(0, this.#filling).forEach((val, idx) => {
      this.#data[idx] = callback(val);
    });
    return this;
  }
  
  /**
   * Create and return a shallow copy of this Vector instance.
   *
   * The new Vector will have the same capacity and filling count as the original,
   * and its internal storage will contain the same elements up to the current filling.
   * The returned instance is a separate object; mutating its storage will not affect
   * the original Vector's internal array reference.
   *
   * @returns {Vector} A new Vector instance containing copied elements.
   */
  copy() {
    var copyOfThis = new Vector(this.#capacity);
    copyOfThis.#filling = this.#filling;
    for (var index = 0; index < this.#filling; ++index)
      copyOfThis.#data[index] = this.#data[index];
    return copyOfThis;
  }

  /**
   * Swap internal storage with another Vector instance.
   *
   * Transfers ownership of this Vector's internal backing fields from the provided
   * Vector to this instance. After a successful swap, this Vector will contain the
   * data that previously belonged to `other`, and `other`'s internal fields are reset
   * (data set to null and numeric fields set to 0).
   *
   * The operation is only performed when `other` is a different instance of Vector.
   * Calling this method with a non-Vector or with the same instance is a no-op.
   * This method mutates both vectors and returns this instance to allow chaining.
   *
   * @param {Vector} other - The Vector to swap internal storage with.
   * @returns {this} The current Vector instance (after swap).
   */
  swap(other)
  {
    if( other instanceof Vector && other !== this) 
    {
        this.#data = other.#data; other.#data = null;
        this.#filling = other.#filling; other.#filling = 0;
        this.#capacity = other.#capacity; other.#capacity = 0;
    }
    return this;
  }

  /**
   * Assigns values from another Vector or any iterable to this Vector.
   *
   * If `other` is the same instance as this Vector, the operation is a no-op.
   * When `other` is a Vector or satisfies `isIterable`, this Vector is cleared
   * and each value from `other` is appended in iteration order (consumes the iterable).
   * If `other` is not a Vector or iterable, this Vector is left unchanged.
   *
   * @param {Vector|Iterable} other - Source of values to copy into this Vector.
   * @returns {this} The current Vector instance (for chaining).
   */
  assign(other)
  {
    if((other instanceof Vector && other !== this) || isIterable(other)) 
    {
      this.clear();
      for(var value of other)
        this.pushBack(value);
    }
    return this;
  }

 
  /**
   * Add a value to this vector in-place using the internal add operator.
   *
   * If `value` is a number, the scalar is added to every component.
   * If `value` is an array-like or another vector, performs element-wise addition.
   * The operation mutates and returns this vector to allow chaining.
   *
   * @param {number|Array<number>|Object} value - Scalar or vector/array to add.
   * @throws {TypeError} If `value` is not a number or array-like of numbers.
   * @throws {RangeError} If `value` is array-like and its length does not match this vector.
   * @returns {this} The modified vector (for chaining).
   */
  addEqual(value) {
    return this.unaryOperator(value, OPERATOR_ADD);
  }

 
  /**
   * Subtracts the given value from this vector in-place.
   *
   * Performs an element-wise subtraction of `value` from the current vector and
   * mutates the instance. Internally delegates to `unaryOperator` with the
   * subtraction operator (OPERATOR_SUB). Returns the instance to allow method
   * chaining.
   *
   * @param {number|Array<number>|Vector} value - The value to subtract. If a
   *        number is provided, it is subtracted from every component. If an
   *        array or another Vector is provided, subtraction is performed
   *        element-wise.
   * @returns {this} The modified vector instance (for chaining).
   */
  subEqual(value) {
    return this.unaryOperator(value, OPERATOR_SUB);
  }

  /**
   * Multiplies each element of the vector by the specified value and updates the vector in place.
   * 
   * If `value` is a number, each component of this vector is multiplied by that scalar.
   * If `value` is a Vector or an array of numbers, multiplication is performed element-wise.
   *
   * @param {number|Vector} value - The value or vector to multiply with each element of this vector.
   * @returns {Vector} The updated vector after multiplication.
   */
  mulEqual(value) {
    return this.unaryOperator(value, OPERATOR_MUL);
  }

  /**
   * Divides this vector by a given value and returns the modified vector.
   *
   * If `value` is a number, each component of this vector is divided by that scalar.
   * If `value` is a Vector or an array of numbers, division is performed element-wise.
   *
   * @param {number|Vector|Array<number>} value - Scalar or per-component divisor.
   * @returns {this} The vector instance after performing the division (mutated).
   * @throws {Error} If `value` has incompatible dimensions for element-wise division.
   * @remarks Behavior for division by zero (e.g. scalar `0` or containing `0` elements) depends on the underlying implementation and may result in Infinity/NaN or an error.
   */
  divEqual(value) {
    return this.unaryOperator(value, OPERATOR_DIV);
  }

  /**
   * Adds two vectors element-wise.
   *
   * @param {number|Vector|number[]} lhs - The left-hand side vector.
   * @param {number|Vector|number[]} rhs - The right-hand side vector.
   * @returns {Vector} The resulting vector after addition.
   */
  static add(lhs, rhs) {
    return Vector.binaryOperator(lhs, rhs, OPERATOR_ADD);
  }

  /**
   * Subtracts two vectors element-wise.
   *
   * @param {number|Vector|number[]} lhs - The left-hand side vector or array or number.
   * @param {number|Vector|number[]} rhs - The right-hand side vector or array or number.
   * @returns {Vector} The result of element-wise subtraction (lhs - rhs).
   */
  static sub(lhs, rhs) {
    return Vector.binaryOperator(lhs, rhs, OPERATOR_SUB);
  }

   /**
   * Multiplies two vectors element-wise.
   *
   * @param {number|Vector|number[]} lhs - The left-hand side vector or array or number.
   * @param {number|Vector|number[]} rhs - The right-hand side vector or array or number.
   * @returns {Vector} The result of element-wise multiplication (lhs * rhs).
   */
  static mul(lhs, rhs) {
    return Vector.binaryOperator(lhs, rhs, OPERATOR_MUL);
  }

   /**
   * Divides two vectors element-wise.
   *
   * @param {number|Vector|number[]} lhs - The left-hand side vector or array or number.
   * @param {number|Vector|number[]} rhs - The right-hand side vector or array or number.
   * @returns {Vector} The result of element-wise division (lhs / rhs).
   */
  static div(lhs, rhs) {
    return Vector.binaryOperator(lhs, rhs, OPERATOR_DIV);
  }

  static distance(lhs, rhs) {
    if(lhs instanceof Vector && rhs instanceof Vector)
      return Vector.sub(lhs, rhs).length;
    console.log(new Error(`Vector::distance::unsupported operands types. Type of lhs: \"${typeof lhs}\", type of rhs: \"${typeof lhs}\".`))
  }

  static direction(lhs, rhs) {
    if(lhs instanceof Vector && rhs instanceof Vector)
      return Vector.sub(lhs, rhs).normalize();
    console.log(new Error(`Vector::direction::unsupported operands types. Type of lhs: \"${typeof lhs}\", type of rhs: \"${typeof lhs}\".`))
  }

  static dot(lhs, rhs) {
    if(lhs instanceof Vector && rhs instanceof Vector)
    {
      var dotProduct = 0.0;
      for (var index = 0; index < Math.min(lhs.#filling, rhs.#filling); index++)
        dotProduct += lhs.#data[index] * rhs.#data[index];
      return dotProduct;
    }
    console.log(new Error(`Vector::dot::unsupported operands types. Type of lhs: \"${typeof lhs}\", type of rhs: \"${typeof lhs}\".`))
  }

  static partial(func, arg, index, step = NUMERIC_ACCURACY) {
    if(arg instanceof Vector){
      arg.setUnchecked(index, arg.getUnchecked(index) + step);
      var fr = func(arg);
      arg.setUnchecked(index, arg.getUnchecked(index) - 2.0 * step);
      var fl = func(arg);
      arg.setUnchecked(index, arg.getUnchecked(index) + step);
      return (fr - fl) / (step * 2.0);
    }
    console.log(new Error(`Vector::partial::unsupported operands types.
                           Type of func: \"${typeof func}\", 
                           arg: \"${typeof arg}\", 
                           index: \"${typeof index}\", 
                           step: \"${typeof step}\".`));
  }

  static partial2(func, arg, index1, index2, step = NUMERIC_ACCURACY) {
    if(arg instanceof Vector){
      arg.setUnchecked(index2, arg.getUnchecked(index2) + step);
      var fr = Vector.partial(func, arg, index1, step);
      arg.setUnchecked(index2, arg.getUnchecked(index2) - 2.0 * step);
      var fl = Vector.partial(func, arg, index1, step);
      arg.setUnchecked(index2, arg.getUnchecked(index2) + step);
      return (fr - fl) / step * 0.5;
    }
    console.log(new Error(`Vector::partial::unsupported operands types.
                           Type of func: \"${typeof func}\", 
                           arg: \"${typeof arg}\", 
                           index1: \"${typeof index1}\", 
                           index2: \"${typeof index2}\", 
                           step: \"${typeof step}\".`))
  }

  static gradient(func, arg, step = NUMERIC_ACCURACY) {
    if(arg instanceof Vector){
      var grad = new Vector(arg.size);
      for (var index = 0; index < arg.size; ++index)
        grad.setUnchecked(index, Vector.partial(func, arg, index, step));
      return grad;
    }
    console.log(new Error(`Vector::gradient::unsupported operands types.
                           Type of func: \"${typeof func}\", 
                           arg: \"${typeof arg}\", 
                           step: \"${typeof step}\".`))
  }
}

module.exports = { Vector };

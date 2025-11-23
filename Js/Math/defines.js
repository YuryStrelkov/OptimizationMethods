const VECTOR_TYPE_ID = 1;
const MATRIX_TYPE_ID = 2;

const NUMERIC_ACCURACY = 1e-12;
const OPERATOR_ADD = 0;
const OPERATOR_SUB = 1;
const OPERATOR_MUL = 2;
const OPERATOR_DIV = 3;

const VECTOR_AS_LEFT_OPERAND = 1;
const VECTOR_AS_RIGHT_OPERAND = 2;
const SCALAR_AS_LEFT_OPERAND = 4;
const SCALAR_AS_RIGHT_OPERAND = 8;
const MATRIX_AS_LEFT_OPERAND = 16;
const MATRIX_AS_RIGHT_OPERAND = 32;

const VECTOR_MATRIX_OPERATOR = VECTOR_AS_LEFT_OPERAND | MATRIX_AS_RIGHT_OPERAND;
const MATRIX_VECTOR_OPERATOR = MATRIX_AS_LEFT_OPERAND | VECTOR_AS_RIGHT_OPERAND;
const SCALAR_MATRIX_OPERATOR = SCALAR_AS_LEFT_OPERAND | MATRIX_AS_RIGHT_OPERAND;
const MATRIX_SCALAR_OPERATOR = MATRIX_AS_LEFT_OPERAND | SCALAR_AS_RIGHT_OPERAND;
const MATRIX_MATRIX_OPERATOR = MATRIX_AS_LEFT_OPERAND | MATRIX_AS_RIGHT_OPERAND;
const VECTOR_VECTOR_OPERATOR = VECTOR_AS_LEFT_OPERAND | VECTOR_AS_RIGHT_OPERAND;
const SCALAR_VECTOR_OPERATOR = SCALAR_AS_LEFT_OPERAND | VECTOR_AS_RIGHT_OPERAND;
const VECTOR_SCALAR_OPERATOR = VECTOR_AS_LEFT_OPERAND | SCALAR_AS_RIGHT_OPERAND;

const formatNumber = (
  number,
  alignment = 0,
  charactersCount = 12,
  decimalCharactersCount = 3,
  fillCharacter = " ",
  format = null
) => {
  let numberStr = format === null ? `${number.toFixed(decimalCharactersCount)}` : format(number);
  switch (alignment) {
    case 0:
      return fillCharacter
        .repeat(Math.max(charactersCount - numberStr.length, 0))
        .concat(numberStr);
    case 1:
      return numberStr.concat(
        fillCharacter.repeat(Math.max(charactersCount - numberStr.length, 0))
      );
    case 2:
      let delta = Math.max(charactersCount - numberStr.length, 0);
      if (delta % 2 == 0)
        return fillCharacter
          .repeat(delta >> 1)
          .concat(numberStr.concat(fillCharacter.repeat(delta >> 1)));
      else
        return fillCharacter
          .repeat((delta >> 1) + 1)
          .concat(numberStr.concat(fillCharacter.repeat(delta >> 1)));
    default:
      return numberStr;
  }
};

const operatorTypeError = (targetType, operandTypeName, operatorType) => {
  switch (operatorType) {
    case OPERATOR_ADD:
      console.error(
        new Error(
          `${targetType}::Addition::Type \"${operandTypeName}\" incompatible with \"${operandTypeName}\" type...`
        )
      );
      break;
    case OPERATOR_SUB:
      console.error(
        new Error(
          `${targetType}::Subtraction::Type \"${operandTypeName}\" incompatible with \"${operandTypeName}\" type...`
        )
      );
      break;
    case OPERATOR_MUL:
      console.error(
        new Error(
          `${targetType}::Multiplication::Type \"${operandTypeName}\" incompatible with \"${operandTypeName}\" type...`
        )
      );
      break;
    case OPERATOR_DIV:
      console.error(
        new Error(
          `${targetType}::Division::Type \"${operandTypeName}\" incompatible with \"${operandTypeName}\" type...`
        )
      );
      break;
    default:
      console.error(
        new Error(
          `${targetType}::Unknown operator type \"${operatorType}\" or type \"${operandTypeName}\" incompatible with \"${operandTypeName}\" type..`
        )
      );
  }
};

// const decimalToRational = (value, number) =>
// {
//   let m00 = 1.0;
//   let m01 = 0.0;
//   let m10 = 0.0;
//   let m11 = 1.0;
//   let ai, t, sign = value >= 0 ? 1 : -1;
//   let x = Math.abs(value);
// 
//   while ((m10 * (ai = x|0) + m11) <= 1000) {
//     t = m00 * ai + m01;
//     m01 = m00;
//     m00 = t;
//     t = m10 * ai + m11;
//     m11 = m10;
//     m10 = t;
//     if (x == ai) {
//       break;
//     }   // AF: division by zero
//     x = 1.0 / (x - ai);
//     if (x > 0x7FFFFFFF ) {
//       break;
//     }  // AF: representation failure
//   }
// 
//   if (!(number instanceof Array) || number.length != 3)
//     number = new Array(3);
// 
//   if ((number[0] = (m00 / m10) | 0) != 0) {
//     number[1] = (m00 - number[0] * m10) | 0;
//     number[0] *= sign;
//     number[2] = m10 | 0;
//   }
//   else 
//   {
//     number[1] = (sign * m00) | 0;
//     number[2] = m10 | 0;
//   }
// }
// 
// const rationalFormatter = (val) =>
// {
//   let number = null;
//   decimalToRational(val, number);
//   // number[0] | 0 | 1 | 0 | 0 | 1 | 0 | 1 | 1 |
//   // number[1] | 0 | 0 | 1 | 0 | 1 | 1 | 0 | 1 |
//   // number[2] | 0 | 0 | 0 | 1 | 0 | 1 | 1 | 1 |
//   //---------------------------------------------
//   // MASK       | 0 | 1 | 2 | 4 | 3 | 6 | 5 | 7 |
//   let _case = (number[0] !== 0 ? 1 : 0) | (number[1]  !== 0 ? 2 : 0) | (number[2]  !== 0 ? 4 : 0);
//   switch (_case)
//   {
//     case 0: return printf("%*ld", 13, 0);
//     case 1: return printf("%*ld", 13, number[0]);
//     case 2: return printf("%*s",  13, "nan");
//     case 3: return printf("%*ld", 13, number[0]);
//     case 4: return printf("%*ld", 13, 0);
//     case 5: return printf("%*ld", 13, number[0]);
//     case 6: return printf("    %*ld/%*ld", 4, number[1], 4, number[2]);
//     case 7: return printf("%*ld %*ld/%*ld", 3, number[0], 4, number[1], 4, number[2]);
//   }
//   return 0;
// }


const isIterable = (obj) => {
  return obj != null && typeof obj[Symbol.iterator] === "function";
};

const isNumber = (value) => {
  return value instanceof Number || typeof value === "number";
};

class Slice {
  #array;
  #start;
  #end;
  #step;
  #isInRange(index) {
    return 0 <= index && index < this.size;
  }
  constructor(array, start, end, step = 1) {
    //naive solution
    if (array instanceof Array) {
      this.#array = array;
      this.#start = start === undefined ? 0 : start;
      this.#end = end === undefined ? array.length : end;
      this.#step = step === undefined ? 1 : step;
    } else {
      this.#array = null;
      this.#start = 0;
      this.#end = 0;
      this.#step = 0;
    }
  }
  get size() {
    return ((this.#end - this.#start) / this.#step) | 0;
  }

  get length() {
    return this.size;
  }

  get(index) {
    if (this.#isInRange(index)) return this.getUnchecked(index);
    else console.log(new Error());
  }

  set(index, value) {
    if (this.#isInRange(index)) this.setUnchecked(index, value);
    else console.log(new Error());
  }

  setUnchecked(index, value) {
    this.#array[this.#start + index * this.#step] = value;
  }

  getUnchecked(index) {
    return this.#array[this.#start + index * this.#step];
  }

  setUnchecked(index, value) {
    this.#array[this.#start + index * this.#step] = value;
  }

  [Symbol.iterator]() {
    let currentIndex = -1;
    const elements = this.#array;
    const begin = this.#start;
    const step = this.#step;
    const size = this.size;
    return {
      next() {
        currentIndex++;
        if (currentIndex < size)
          return {
            value: elements[begin + currentIndex * step],
            done: false,
          };
        else return { done: true };
      },
    };
  }
  toString() {
    return `[ ${[...this].map((v) => {
        return formatNumber(v);
      })
      .join(", ")} ]`;
  }
}

module.exports = {
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
  Slice,
  formatNumber,
  isNumber,
  operatorTypeError,
  isIterable,
};

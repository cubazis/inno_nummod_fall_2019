#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define BUFF_SIZE 33

/**
 * Я потратила на эту штуку тонну времени, но тупо не успела к дедлайну. Там в мэйне лежат тесты на умножение и сложение
 * которое заиплеченченно. По сути не хватило некоторого времени на отладку, а главное на связку с твоими тестами.
 * Сейчас здесь 8 бит, что бы сделать 32 нужно просто написать 32 в sizeOfPosit и хера себе - все работает 
 */

typedef struct _posit32 {
    /** implement your code **/
    uint32_t val;

} posit;

int sizeOfPosit = 8;
int es = 2;// максимум 4 иначе не влезет в 32
int useed = 16;// 2 ^ (2 ^ es)

/**
 * 128 - 10000000
 * @param position from 0 till sizeOfPosit to get the bit on specific position
 * @param number posit type for which we want the bit
 * @return the bit for specified number
 */
uint32_t getBitOnPosition(int position, posit number) {
    return ((number.val << position) & (1 << sizeOfPosit - 1)) >> (sizeOfPosit - 1);
}

/**
 * Function that returns sign of the posit
 * the sign is the first bit of the number
 * @param num posit to work with
 * @return sign of the posit
 */
uint32_t getSign(posit num) {
    return num.val >> (sizeOfPosit - 1);
}

/**
 * Gets the firstbit of the regime which is on the 1 position of the number (from 0)
 * Iterates over every bit in the number till the next bit will not be equal to the current bit
 * @param num posit type that we want to work with
 * @return the size of the regime without the last bit
 */
uint32_t getRegimeSize(posit num) {
    uint32_t firstbit = getBitOnPosition(1, num);
    int currentShift = 2;
    int sizeOfRegime = 1;
    while (getBitOnPosition(currentShift++, num) == firstbit) {
        sizeOfRegime++;
    }
    return sizeOfRegime;
}

/**
 * es at most might be only 2, but in case if it is not true this function should be rewritten
 * es + 1 -> in case if it is 2 in binary it will become 11; in case if it is 1 it will be still only one 1
 * @return the mask for the exponent
 */
uint32_t getExponentMask() {
    int temp = es;
    if (es == 2) {
        temp++;
    }
    return (temp << (sizeOfPosit - es));
}

/**
 * Returns size of exponent
 * By now it is very silly function
 */
uint32_t getExponentSize() {
    return es;
}


/**
 * First gets the size of the regime then gets the first number of exponent
 * Shifts the number and make AND operation with the result of shift
 * Shit the result number to most right position to get the result number = exponent
 * @param num with what we will work
 * @return value of exponent
 */
uint32_t getExponent(posit num) {
    uint32_t regimeSize = getRegimeSize(num) + 1;

    uint32_t first_num = (num.val << (regimeSize + 1));

    return (first_num & getExponentMask()) >> (sizeOfPosit - getExponentSize());
}


/**
 * TODO а что если число regime начиналось на 0, тогда при сложении новых нулей не прибавится. Какой тогда должен быть размер
 * Нужно генерировать новый regime в таком случае?
 * @param number
 * @return
 */
int sizeOfNumberInBinary(int number) {
    int result = 0;
    while (number > 0) {
        number >>= 1;
        result++;
    }
    return result;
}

/**
 * Генерация побитово regime из переданного значения режима
 * TODO сделать для 0
 * @param number
 * @return
 */
uint32_t getRegimeFromValue(int number) {
    uint32_t result = 0;
    if (number >= 0) {
        result = 1;
        for (int i = number; i > 0; i--) {
            result = result << 1;
            result |= 1;
        }
        result = result << 1;
    }
    return result;
}

/**
 * Generating mask from int because it is very convenient in this case
 * example:
 * in: 3; out: 111;
 * in: 2; out: 11;
 * @param number - value to generate
 * @return mask in format 11
 */
uint32_t getMaskFromInt(int number) {
    uint32_t result = 0;
    for (int i = 0; i < number; i++) {
        result = result << 1;
        result |= 1;
    }
    return result;
}

posit encodePosit(int isNegative, int exp, unsigned int fraction) {
    char buffer[BUFF_SIZE];
    buffer[BUFF_SIZE - 1] = '\0';


    posit result;
    result.val = 0;
    int exp_initial = exp;
    exp = exp & 0b111;
    int regime;
    int regime_shifts;

    if (exp_initial >= 0) {
        regime = (((uint32_t)exp_initial) >> 3) + 1;

        regime_shifts = regime;

        for(int i = 0; i < regime; i++) {
            result.val |= 1 << (30-i);
        }
    } else {
        regime = ((uint32_t)-(exp_initial - exp)) >> 3;
        regime_shifts = regime;
        result.val |= 1 << (30-regime);
    }


    result.val |= exp << (30-regime_shifts-3);

    result.val |= fraction >> (1+regime_shifts+4);

    if ((fraction >> (4+regime_shifts))&1) {
        result.val += 1; // rounding
    }

    if (isNegative) {
        result.val = ~result.val + 1;
    }

    return result;
}

posit convertDoubleToPosit(double input) {

    if (input == 0.0) {
        posit result;
        result.val = 0;
        return result;
    }

    union {
        double input;
        uint64_t bits;
    } val;
    val.input = input;
    int exp = (int)((val.bits >> 52)&0x7FF) - 1023; // substract zero offset
    uint32_t fraction = (uint32_t)((val.bits >> 20)&0xFFFFFFFF); // fit fraction into 32 bit container
    return encodePosit(input < 0, exp, fraction);
}

uint32_t getFractionSize(posit num) {
    return sizeOfPosit - getExponentSize() - 1 /*size of sign*/ - getRegimeSize(num) - 1;
}

/**
 * Take all that left -> it will be fraction
 * @param num
 * @return
 */
uint32_t getFraction(posit num) {
    int sizeOfFraction = sizeOfPosit - getExponentSize() - 1 /*size of sign*/ - getRegimeSize(num) - 1;
    return num.val & getMaskFromInt(sizeOfFraction);
}

/**
 * Ам, ну я в итоге подключила math.h, но раз уж написала то пусть лежит. Просто возведение в степень
 * @param base
 * @param p
 * @return
 */
int intPow(int base, int p) {
    if (p != 0) {
        return intPow(base * base, p - 1);
    } else {
        return base;
    }
}


uint32_t getRegime(posit num) {
    uint32_t firstBit = getBitOnPosition(1, num);
    uint32_t regimeSize = getRegimeSize(num);

    if (firstBit == 1) {
        return regimeSize - 1;
    } else {//if 0
        return -regimeSize;
    }
}

/**
 * TODO проверить сортировку двух чисел по убыванию
 * @return
 */
posit addition(posit valueA, posit valueB) {
    posit a = valueA;
    posit b = valueB;
    if (valueA.val < valueB.val) {
        a = valueB;
        b = valueA;
    }

    uint32_t a_frac = (1 << getFractionSize(a)) | getFraction(a);
    uint32_t b_frac = (1 << getFractionSize(b)) | getFraction(b);

    uint32_t regimeLeft = getRegime(a);
    uint32_t regimeRight = getRegime(b);

    uint32_t expLeft = getExponent(a);
    uint32_t expRight = getExponent(b);
    uint32_t expResult = expLeft;
    uint32_t regimeResult = getRegimeFromValue(regimeLeft);

    uint32_t howMuchToShift = (regimeLeft - regimeRight) * intPow(2, es) + (expLeft - expRight);

    uint32_t shiftedFraction = b_frac >> howMuchToShift;

    uint32_t tempFraction = shiftedFraction + a_frac;

    if(sizeOfNumberInBinary(tempFraction) > sizeOfNumberInBinary(fmax(a_frac, b_frac))){
        expResult += 1;
        if (expResult >= pow(es, 2)) {
            regimeResult = getRegimeFromValue(regimeLeft + regimeRight + 1);
            expResult = expResult - pow(es, 2);
        }

        tempFraction = tempFraction >> 1;
    }

    uint32_t fractionResult = (tempFraction) & getMaskFromInt(sizeOfPosit - 1 - sizeOfNumberInBinary(regimeResult) - es);
    posit result = {
            regimeResult << (sizeOfPosit - 1 - sizeOfNumberInBinary(regimeResult)) |
            expResult << (sizeOfPosit - 1 - sizeOfNumberInBinary(regimeResult) - es) |
            fractionResult};

    //posit result = {(a.val & (getMaskFromInt(5) << 3)) | resultFraction};//TODO вообще не помню откуда вылезла 5
    return result;
}

int numberOfDigitsInNumber(int number) {
    int result = 0;
    while (number > 0) {
        number /= 10;
        result++;
    }
    return result;
}


/**
 * Здесь не нужно так много переменных, но мне так удобнее ориентироваться в происходящем
 * По выведенной формуле. Самое сложное - разобраться с fraction так как он точно перевалит и нужно
 * откинуть лишние биты
 *
 * @param valueA
 * @param valueB
 * @return
 */
posit multiplication(posit valueA, posit valueB) {
    posit a = valueA;
    posit b = valueB;
    if (valueA.val < valueB.val) {
        posit a = valueB;
        posit b = valueA;
    }

    uint32_t signLeft = getSign(a);
    uint32_t signRight = getSign(b);

    uint32_t regimeLeft = getRegime(a);
    uint32_t regimeRight = getRegime(b);
    uint32_t regimeSizeLeft = getRegimeSize(a);
    uint32_t regimeSizeRight = getRegimeSize(b);

    uint32_t expLeft = getExponent(a);
    uint32_t expRight = getExponent(b);

    uint32_t fractionLeft = getFraction(a);
    uint32_t fractionRight = getFraction(b);

    uint32_t fractionSizeLeft = getFractionSize(a);
    uint32_t fractionSizeRight = getFractionSize(b);

    uint32_t signResult = signLeft | signRight;
    uint32_t regimeResult = getRegimeFromValue(regimeLeft + regimeRight);
    uint32_t expResult = expLeft + expRight;
    if (expResult >= pow(es, 2)) {
        regimeResult = getRegimeFromValue(regimeLeft + regimeRight + 1);
        expResult = expResult - pow(es, 2);
    }

    uint32_t fracTemp1 = fractionLeft << fractionSizeRight;
    uint32_t fracTemp2 = fractionRight << fractionSizeLeft;
    uint32_t fracTemp3 = fractionLeft * fractionRight;
    uint32_t fractionResultTemp = fracTemp1 + fracTemp2 + fracTemp3;

    uint32_t sizeOfResultFraction =
            sizeOfPosit - 1 - sizeOfNumberInBinary(regimeResult) - sizeOfNumberInBinary(expResult);
    uint32_t howMuchToShift = 0;
    if (sizeOfNumberInBinary(fractionResultTemp) <= fractionSizeLeft + fractionSizeRight) {
        howMuchToShift = fmax(fractionSizeLeft, fractionSizeRight) +
                         (sizeOfNumberInBinary(regimeResult) - (fmax(regimeSizeLeft, regimeSizeRight) + 1));
    } else {
        howMuchToShift = sizeOfNumberInBinary(fractionResultTemp) - sizeOfResultFraction;
    }
    uint32_t fractionResult = fractionResultTemp
            >> howMuchToShift;/*(int) fabs(sizeOfNumberInBinary(fractionResultTemp) - sizeOfResultFraction);*/

    posit result = {
            regimeResult << (sizeOfPosit - 1 - sizeOfNumberInBinary(regimeResult)) |
            expResult << (sizeOfPosit - 1 - sizeOfNumberInBinary(regimeResult) - es) |
            (fractionResult)};

    return result;
}

posit powPosit(posit base, int p){
    if (p != 0) {
        return powPosit(multiplication(base, base), p - 1);
    } else {
        return base;
    }
}

/**
 * Util для более печати posit
 * @param number
 */
void printPosit(posit number) {
    uint32_t regime = getRegime(number);
    uint32_t exponent = getExponent(number);
    uint32_t fraction = getFraction(number);
    uint32_t fractionSize = getFractionSize(number);

    double useedPart = pow(useed, regime);
    double exponentPart = pow(2, exponent);
    double fractionPart = 1 + (fraction / (pow(2, fractionSize)));

    double result = useedPart * exponentPart * fractionPart;

    printf("%f", result);
}

/**
 * Чисто для тестов
 * @param number чиселка с которой работаем
 * @return дабл значение для чиселки
 */
double getDoubleFromPosit(posit number) {
    uint32_t regime = getRegime(number);
    uint32_t exponent = getExponent(number);
    uint32_t fraction = getFraction(number);
    uint32_t fractionSize = getFractionSize(number);

    double useedPart = pow(useed, regime);
    double exponentPart = pow(2, exponent);
    double fractionPart = 1 + (fraction / (pow(2, fractionSize)));

    double result = useedPart * exponentPart * fractionPart;

    return result;
}

void printAdditionPosit(posit a, posit b, posit result) {
    printf("Addition: ");
    printPosit(a);
    printf(" + ");
    printPosit(b);
    printf(" = ");
    printPosit(result);
    printf("\n");
}

void printMultiplicationPosit(posit a, posit b, posit result) {
    printf("Multiplication: ");
    printPosit(a);
    printf(" * ");
    printPosit(b);
    printf(" = ");
    printPosit(result);
    printf("\n");
}

/**
 * Есть ли ответ в возможных ответах
 * Примитивный поиск по массиву
 * @param answers все возможные ответы
 * @param size размер массива
 * @param answer наш ответ западу
 * @return 1 если возможен такой ответ и 0 если невозможет
 */
int isAvailableInAnswers(double answers[], int size, double answer) {
    for (int i = 0; i < size; i++) {
        if (answers[i] == answer) {
            return 1;
        }
    }
    return 0;
}

/**
 * Тесты для проверки качества работы перемножения
 * Генерирует все возможные значения posit
 * Перемножает
 * Проверяет возможен ли вообще правильный ответ так как может точность быть меньше чем нужно
 * Считает сколько правильно а сколько неправильно
 * TODO 6 тестов все еще не проходит
 */
void multiplyTests() {
    int rightAnswers = 0;
    int totalTests = 0;

    double possibleAnswers[49];
    int index = 0;
    for (int i = 64; i < 113; i++) {
        posit a = {i};
        possibleAnswers[index] = getDoubleFromPosit(a);
        index++;
    }


    for (int i = 64; i < 87; i++) {
        for (int j = 64; j < 87; j++) {
            posit a = {i};
            posit b = {j};

            posit result = multiplication(a, b);

            double dResult = getDoubleFromPosit(result);
            double rightAnswer = getDoubleFromPosit(a) * getDoubleFromPosit(b);

            if (dResult == rightAnswer) {
                rightAnswers++;
                totalTests++;
            } else {
                printf("-------------------- WRONG --------------------\n");
                int isAvailable = isAvailableInAnswers(possibleAnswers, 49, rightAnswer);
                printf("IS AVAILABLE %d\n", isAvailable);
                if (isAvailable == 1) {
                    totalTests++;
                }
            }


            if (getExponent(a) > 1 && getExponent(b) > 1) {
                printf("\nBIG EXPONENT: %d\n", getExponent(a));
            }

            printf("Test for : %d, %d, %d\n", i, j, result.val);
            printMultiplicationPosit(a, b, result);
            printf("\n\n");
        }
    }

    printf("Test result: %d/%d\n", rightAnswers, totalTests);

}

void additionTests() {
    int rightAnswers = 0;
    int totalTests = 0;

    double possibleAnswers[49];
    int index = 0;
    for (int i = 64; i < 113; i++) {
        posit a = {i};
        possibleAnswers[index] = getDoubleFromPosit(a);
        index++;
    }


    for (int i = 64; i < 87; i++) {
        for (int j = 64; j < 87; j++) {
            posit a = {i};
            posit b = {j};

            posit result = addition(a, b);

            double dResult = getDoubleFromPosit(result);
            double rightAnswer = getDoubleFromPosit(a) + getDoubleFromPosit(b);

            if (dResult == rightAnswer) {
                rightAnswers++;
                totalTests++;
            } else {
                printf("-------------------- WRONG --------------------\n");
                int isAvailable = isAvailableInAnswers(possibleAnswers, 49, rightAnswer);
                printf("IS AVAILABLE %d\n", isAvailable);
                if (isAvailable == 1) {
                    totalTests++;
                }
            }


            printf("Test for : %d, %d, %d\n", i, j, result.val);
            printAdditionPosit(a, b, result);
            printf("\n\n");
        }
    }

    printf("Test result: %d/%d\n", rightAnswers, totalTests);
}

int main() {
    multiplyTests();
    additionTests();
  //  posit a = {68};
//    posit b = {80};
//
//    posit result = addition(a, b);
//
//    printf("%d\n", result.val);
//
//    printAdditionPosit(a, b, result);

//    posit a = {72};
//    posit b = {113};
//
//    printPosit(a);
//    printPosit(b);
//
//    posit result = convertDoubleToPosit(1.5);
//    printPosit(result);

//    printPosit(a);
//    printf("\n");
//
//    printPosit(powPosit(a, 1));
    posit result = convertDoubleToPosit(1.5);

    return 0;
}
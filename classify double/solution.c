#include <stdio.h>

#include <stdbool.h>

#include <stdint.h>

#include <stdlib.h>

/**
 * Structure for storing sign, exponent and fraction indexes.
 */

struct doubleBounds {
  uint8_t fracStart;
  uint8_t fracEnd;
  uint8_t expStart;
  uint8_t expEnd;
  uint8_t signInd;
};

struct doubleBounds bounds = {
  0,
  51,
  52,
  62,
  63
};

/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64(double number) {
    return *((uint64_t * )( & number));
}

bool getBit(const uint64_t number, const uint8_t index) {
    uint64_t mask = 1ull << index;
    uint64_t temp = number & mask;
    temp >>= index;
    return temp == 1;
}

/**
 * Helper functions:
 */

bool checkForZeroSign(uint64_t number) {
    return getBit(number, bounds.signInd) == 0;
}

uint64_t getRangeValue(uint64_t number, uint8_t start, uint8_t end) {
    uint8_t leftSteps = bounds.signInd - end;
    uint8_t rightSteps = leftSteps + start;
    return (number << leftSteps) >> rightSteps;
}

bool checkRangeForAllZeroValues(uint64_t number, uint8_t start, uint8_t end) {
    uint64_t value = getRangeValue(number, start, end);
    return value == 0;
}

bool checkRangeForAllOneValues(uint64_t number, uint8_t start, uint8_t end) {
    uint64_t allOneValue = (1ull << (end - start + 1)) - 1;
    uint64_t value = getRangeValue(number, start, end);
    return value == allOneValue;
}

bool checkRangeForIntermediateValues(uint64_t number, uint8_t start, uint8_t end) {
    uint8_t allZeroValue = 0;
    uint64_t allOneValue = (1ull << (end - start + 1)) - 1;
    uint64_t value = getRangeValue(number, start, end);
    return (allZeroValue < value) & (value < allOneValue);
}

bool checkForZeroExponent(uint64_t number) {
    return checkRangeForAllZeroValues(number, bounds.expStart, bounds.expEnd);
}

bool checkForAllOneExponent(uint64_t number) {
    return checkRangeForAllOneValues(number, bounds.expStart, bounds.expEnd);
}

bool checkForZeroFraction(uint64_t number) {
    return checkRangeForAllZeroValues(number, bounds.fracStart, bounds.fracEnd);
}

bool checkForAllOneFraction(uint64_t number) {
    return checkRangeForAllOneValues(number, bounds.fracStart, bounds.fracEnd);
}

bool checkExpForIntermediateValues(uint64_t number) {
    return checkRangeForIntermediateValues(number, bounds.expStart, bounds.expEnd);
}

bool checkFracForIntermediateValues(uint64_t number) {
    return checkRangeForIntermediateValues(number, bounds.fracStart, bounds.fracEnd);
}

/**
 * Checkers here:
 */

bool checkForPlusZero(uint64_t number) {
    return number == 0;
}

bool checkForMinusZero(uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf(uint64_t number) {
    return checkForZeroSign(number) &
        checkForAllOneExponent(number) &
        checkForZeroFraction(number);
}

bool checkForMinusInf(uint64_t number) {
    return (!checkForZeroSign(number)) &
        checkForAllOneExponent(number) &
        checkForZeroFraction(number);
}

bool checkForPlusNormal(uint64_t number) {
    return checkForZeroSign(number) &
        checkExpForIntermediateValues(number);
}

bool checkForMinusNormal(uint64_t number) {
    return (!checkForZeroSign(number)) &
        checkExpForIntermediateValues(number);
}

bool checkForPlusDenormal(uint64_t number) {
    return checkForZeroSign(number) &
        checkForZeroExponent(number) &
        (!checkForZeroFraction(number));
}

bool checkForMinusDenormal(uint64_t number) {
    return (!checkForZeroSign(number)) &
        checkForZeroExponent(number) &
        (!checkForZeroFraction(number));
}

bool checkForSignalingNan(uint64_t number) {
    return checkForAllOneExponent(number) & (!getBit(number, bounds.fracEnd)) &
        (!checkRangeForAllZeroValues(number, bounds.fracStart, bounds.fracEnd - 1));
}

bool checkForQuietNan(uint64_t number) {
    return checkForAllOneExponent(number) &
        getBit(number, bounds.fracEnd);
}

void classify(double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    } else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    } else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    } else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    } else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    } else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    } else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    } else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    } else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    } else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    } else {
        printf("Error.\n");
    }
}

void classifyInt(double number) {
    if (checkForPlusZero(number)) {
        printf("Plus zero\n");
    } else if (checkForMinusZero(number)) {
        printf("Minus zero\n");
    } else if (checkForPlusInf(number)) {
        printf("Plus inf\n");
    } else if (checkForMinusInf(number)) {
        printf("Minus inf\n");
    } else if (checkForPlusNormal(number)) {
        printf("Plus normal\n");
    } else if (checkForMinusNormal(number)) {
        printf("Minus normal\n");
    } else if (checkForPlusDenormal(number)) {
        printf("Plus Denormal\n");
    } else if (checkForMinusDenormal(number)) {
        printf("Minus Denormal\n");
    } else if (checkForSignalingNan(number)) {
        printf("Signailing NaN\n");
    } else if (checkForQuietNan(number)) {
        printf("Quiet NaN\n");
    } else {
        printf("Error.\n");
    }
}

// 0 00000000000 0000000000000000000000000000000000000000000000000000
// 0 11111111111 0000000000000000000000000000000000000000000000000000
int main() {
    double test;
    scanf("%lf", & test);
    classify(test);
    // uint64_t test = 0b1111111111110000000000000000000000000000000000000000000000000000;
    // classifyInt(test);
    // uint8_t ans = checkRangeForIntermediateValues(test, 60, 62);
    // uint8_t ans = checkRangeForAllOneValues(test, 60, 62);
    // uint8_t ans = checkRangeForAllZeroValues(test, 60, 62);
    // printf("%d\n", ans);
    return 0;
}
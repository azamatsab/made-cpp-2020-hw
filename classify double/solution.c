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
  uint8_t exptEnd;
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
    uint64_t one = 1;
    uint64_t mask = one << index;
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

bool checkRangeForValue(uint64_t number, uint8_t start, uint8_t end, uint8_t value) {
    for (int i = start; i < end + 1; i++) {
        if (getBit(number, i) != value) {
            return false;
        }
    }
    return true;
}

bool checkForZeroExponent(uint64_t number) {
    return checkRangeForValue(number, bounds.expStart, bounds.exptEnd, 0);
}

bool checkForAllOneExponent(uint64_t number) {
    return checkRangeForValue(number, bounds.expStart, bounds.exptEnd, 1);
}

bool checkForZeroFraction(uint64_t number) {
    return checkRangeForValue(number, bounds.fracStart, bounds.fracEnd, 0);
}

bool checkForAllOneFraction(uint64_t number) {
    return checkRangeForValue(number, bounds.fracStart, bounds.fracEnd, 1);
}

bool checkExpForIntermediateValues(uint64_t number) {
    return !(checkRangeForValue(number, bounds.expStart, bounds.exptEnd, 1) |
        checkRangeForValue(number, bounds.expStart, bounds.exptEnd, 0));
}

bool checkFracForIntermediateValues(uint64_t number) {
    return !(checkRangeForValue(number, bounds.fracStart, bounds.fracEnd, 1) |
        checkRangeForValue(number, bounds.fracStart, bounds.fracEnd, 0));
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
        (!checkRangeForValue(number, bounds.fracStart, bounds.fracEnd - 1, 0));
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

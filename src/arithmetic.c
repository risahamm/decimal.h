#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  if (result == NULL) {
    return (res = 1);
  }
  int sign1 = get_bit(value_1.bits[3], 31);
  int sign2 = get_bit(value_2.bits[3], 31);
  if (sign1 ^ sign2) {
    null_bit(&value_1.bits[3], 31);
    null_bit(&value_2.bits[3], 31);
    if (sign1 == 0) {
      res = s21_sub(value_1, value_2, result);
    } else {
      res = s21_sub(value_2, value_1, result);
    }
  } else {
    big_decimal big_value_1 = decimal_to_big(value_1);
    big_decimal big_value_2 = decimal_to_big(value_2);
    big_decimal big_result = decimal_to_big(*result);
    normalise(&big_value_1, &big_value_2);
    big_result = summ(big_value_1, big_value_2);
    set_big_scale(get_big_scale(big_value_1), &big_result);
    if (sign1 == 1 && sign2 == 1) {
      set_bit(&big_result.bits[6], 31);
    }
    res = big_to_decimal(result, big_result);
  }
  if (res == 1 || res == 2) {
    null_decimal(result);
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  if (result == NULL) {
    return (res = 1);
  }
  int sign1 = get_bit(value_1.bits[3], 31);
  int sign2 = get_bit(value_2.bits[3], 31);
  if (sign1 ^ sign2) {
    if (sign1 == 0) {
      null_bit(&value_2.bits[3], 31);
    } else if (sign1 == 1) {
      set_bit(&value_2.bits[3], 31);
    }
    res = s21_add(value_1, value_2, result);
  } else {
    big_decimal big_value_1 = decimal_to_big(value_1);
    big_decimal big_value_2 = decimal_to_big(value_2);
    big_decimal big_result = decimal_to_big(*result);
    normalise(&big_value_1, &big_value_2);
    null_bit(&big_value_1.bits[6], 31);
    null_bit(&big_value_2.bits[6], 31);
    if (greater_or_equal(big_value_1, big_value_2)) {
      big_result = subtract(big_value_1, big_value_2);
    } else {
      big_result = subtract(big_value_2, big_value_1);
    }
    if (sign1 == 1) {
      sign2 = greater_or_equal(big_value_2, big_value_1) ? 0 : 1;
    } else {
      if (greater_or_not(big_value_2, big_value_1)) {
        sign2 = 1;
      } else if (greater_or_not(big_value_1, big_value_2)) {
        sign2 = 0;
      } else if (equal_or_not(big_value_1, big_value_2)) {
        sign2 = 0;
      }
    }
    set_big_scale(get_big_scale(big_value_1), &big_result);
    if (sign2 == 1) {
      set_bit(&big_result.bits[6], 31);
    } else if (sign2 == 0) {
      null_bit(&big_result.bits[6], 31);
    }
    res = big_to_decimal(result, big_result);
  }
  if (res == 1 || res == 2) {
    null_decimal(result);
  }
  return res;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  if (result == NULL) {
    return (res = 1);
  }
  int sign1 = get_bit(value_1.bits[3], 31);
  int sign2 = get_bit(value_2.bits[3], 31);
  big_decimal big_value_1 = decimal_to_big(value_1);
  big_decimal big_value_2 = decimal_to_big(value_2);
  big_decimal big_result = decimal_to_big(*result);
  big_result = multiplication(big_value_1, big_value_2);
  set_big_scale((get_big_scale(big_value_1) + get_big_scale(big_value_2)),
                &big_result);
  if (sign1 ^ sign2) {
    set_bit(&big_result.bits[6], 31);
  } else {
    null_bit(&big_result.bits[6], 31);
  }
  res = big_to_decimal(result, big_result);
  if (res == 1 || res == 2) {
    null_decimal(result);
  }
  return res;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  s21_decimal zero = {{0, 0, 0, 0}};
  if (result == NULL) {
    return (res = 1);
  }
  if (s21_is_equal(value_2, zero)) {
    return (res = 3);
  }
  float temp_1 = 0;
  s21_from_decimal_to_float(value_1, &temp_1);
  float temp_2 = 0;
  s21_from_decimal_to_float(value_2, &temp_2);
  float temp_result = temp_1 / temp_2;
  res = s21_from_float_to_decimal(temp_result, result);
  if (res == 1) {
    null_decimal(result);
  }
  return res;
}

big_decimal summ(big_decimal value_1, big_decimal value_2) {
  big_decimal result = {{0}};
  big_decimal temp_1 = {{0}};
  big_decimal temp_2 = {{0}};
  while (decimal_not_equal_zero(value_1) != 0) {
    temp_1 = bitwise_and(value_1, value_2);
    shift_left(&temp_1, 1);
    temp_2 = bitwise_xor(value_1, value_2);
    value_1 = temp_1;
    value_2 = temp_2;
  }
  if (decimal_not_equal_zero(value_1) == 0) {
    result = value_2;
  }
  return result;
}

big_decimal subtract(big_decimal value_1, big_decimal value_2) {
  big_decimal temp = {{0}};
  while (decimal_not_equal_zero(value_2) != 0) {
    temp = bitwise_and(bitwise_not(value_1), value_2);
    value_1 = bitwise_xor(value_1, value_2);
    shift_left(&temp, 1);
    value_2 = temp;
  }
  return value_1;
}

big_decimal multiplication(big_decimal value_1, big_decimal value_2) {
  big_decimal result = {{0}};
  while (decimal_not_equal_zero(value_2) != 0) {
    if (get_bit(value_2.bits[0], 0) == 1) {
      result = summ(result, value_1);
    }
    shift_left(&value_1, 1);
    shift_right(&value_2, 1);
  }
  return result;
}
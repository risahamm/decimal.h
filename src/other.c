#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int return_code = 0;
  big_decimal big_result = decimal_to_big(value);
  int scale = get_big_scale(big_result);
  if (scale != 0) {
    while (scale > 0) {
      divide_by_10(&big_result);
      scale--;
      set_big_scale((scale), &big_result);
    }
  }
  int res = big_to_decimal(result, big_result);
  if (res == 1 || res == 2) {
    return_code = 1;
    null_decimal(result);
  }
  return return_code;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int return_code = 0;
  invert_bit(&value.bits[3], 31);
  *result = value;
  return return_code;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int return_code = 0;
  int sign = get_bit(value.bits[3], 31);
  big_decimal big_result = decimal_to_big(value);
  int scale = get_big_scale(big_result);
  s21_decimal temp1 = {{0}};
  big_decimal temp2 = {{0}};
  big_decimal remainder = {{0}};
  big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
  big_decimal half = {{5, 0, 0, 0, 0, 0, 0}};
  if (scale != 0) {
    while (scale > 1) {
      divide_by_10(&big_result);
      scale--;
      set_big_scale((scale), &big_result);
      return_code = big_to_decimal(result, big_result);
    }
    if (scale == 1) {
      big_to_decimal(&value, big_result);
      s21_truncate(value, &temp1);
      temp2 = decimal_to_big(temp1);
      multiply_by_10(&temp2);
      remainder = subtract(big_result, temp2);
      divide_by_10(&big_result);
      scale--;
      set_big_scale((scale), &big_result);
      return_code = big_to_decimal(result, big_result);
      set_spec_bit(&result->bits[3], 31, sign);
    }
    if (greater_or_equal(remainder, half) == 1) {
      summ(big_result, one);
      return_code = big_to_decimal(result, big_result);
      set_spec_bit(&result->bits[3], 31, sign);
    }
  } else {
    *result = value;
  }
  if (return_code == 1 || return_code == 2) {
    return_code = 1;
    null_decimal(result);
  }
  return return_code;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int return_code = 0;
  int sign = get_bit(value.bits[3], 31);
  int exp = get_scale(value);
  if (!sign) {
    return_code = s21_truncate(value, result);
  } else {
    if (exp == 0) {
      return_code = 0;
      *result = value;
    } else {
      return_code = s21_truncate(value, result);
      if (return_code == 0) {
        big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
        big_decimal big_result = decimal_to_big(*result);
        big_result = summ(big_result, one);
        return_code = big_to_decimal(result, big_result);
        set_sign(result);
      }
    }
  }
  if (return_code == 1 || return_code == 2) {
    return_code = 1;
    null_decimal(result);
  }
  return return_code;
}

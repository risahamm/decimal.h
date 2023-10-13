#include "s21_decimal.h"

//возвращает 0 если true и 1 если false

int s21_is_greater_or_equal(s21_decimal value1, s21_decimal value2) {
  int return_code = 0;
  big_decimal val_1 = decimal_to_big(value1);
  big_decimal val_2 = decimal_to_big(value2);
  return_code = greater_or_equal(val_1, val_2);
  return return_code;
}

int s21_is_greater(s21_decimal value1, s21_decimal value2) {
  int return_code = 0;
  big_decimal val_1 = decimal_to_big(value1);
  big_decimal val_2 = decimal_to_big(value2);
  return_code = greater_or_not(val_1, val_2);
  return return_code;
}

int s21_is_less_or_equal(s21_decimal value1, s21_decimal value2) {
  int return_code = 0;
  big_decimal val_1 = decimal_to_big(value1);
  big_decimal val_2 = decimal_to_big(value2);
  return_code = less_or_equal(val_1, val_2);
  return return_code;
}

int s21_is_less(s21_decimal value1, s21_decimal value2) {
  int return_code = 0;
  big_decimal val_1 = decimal_to_big(value1);
  big_decimal val_2 = decimal_to_big(value2);
  return_code = less_or_not(val_1, val_2);
  return return_code;
}

int s21_is_equal(s21_decimal value1, s21_decimal value2) {
  int return_code = 0;
  big_decimal val_1 = decimal_to_big(value1);
  big_decimal val_2 = decimal_to_big(value2);
  return_code = equal_or_not(val_1, val_2);
  return return_code;
}

int s21_is_not_equal(s21_decimal value1, s21_decimal value2) {
  int return_code = 0;
  big_decimal val_1 = decimal_to_big(value1);
  big_decimal val_2 = decimal_to_big(value2);
  return_code = !equal_or_not(val_1, val_2);
  return return_code;
}

#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEGATIVE_INT 2147483648

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[7];
} big_decimal;

// arithmetic
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// comparison
int s21_is_less(s21_decimal value1, s21_decimal value2);
int s21_is_less_or_equal(s21_decimal value1, s21_decimal value2);
int s21_is_greater(s21_decimal value1, s21_decimal value2);
int s21_is_greater_or_equal(s21_decimal value1, s21_decimal value2);
int s21_is_equal(s21_decimal value1, s21_decimal value2);
int s21_is_not_equal(s21_decimal value1, s21_decimal value2);

// conversion
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// round
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// additional //

// bit operations
int get_bit(unsigned int num, int index);
void invert_bit(unsigned int *num, int index);
void set_bit(unsigned int *num, int index);  //устанавливает бит на 1
void set_spec_bit(unsigned int *num, int index, int bit);
void null_bit(unsigned int *num, int index);  //устанав бит на 0

// decimal operations
void null_decimal(s21_decimal *decimal);  //зануляет децимал
void null_big_decimal(big_decimal *value);
int get_scale(s21_decimal decimal);  //читает экспоненту
int get_big_scale(big_decimal decimal);
void set_scale(int scale, s21_decimal *decimal);  //устанавливает экспоненту
void set_big_scale(int scale, big_decimal *decimal);
void set_sign(s21_decimal *decimal);  //делает число отрицательным

// arithmetics
void normalise(big_decimal *decimal_1, big_decimal *decimal_2);
void multiply_by_10(big_decimal *decimal);
void divide_by_10(big_decimal *decimal);
big_decimal summ(big_decimal value_1, big_decimal value_2);
big_decimal subtract(big_decimal value_1, big_decimal value_2);
big_decimal multiplication(big_decimal value_1, big_decimal value_2);
void shift_left(big_decimal *decimal, int step);
void shift_right(big_decimal *decimal, int step);

// comparison
int decimal_not_equal_zero(big_decimal decimal);  //пустой или нет
int big_3_bits_check(big_decimal value);  //проверяет 3,4,5 биты big_decimal
int greater_or_equal(big_decimal value_1, big_decimal value_2);
int less_or_equal(big_decimal value_1, big_decimal value_2);
int equal_or_not(big_decimal value_1, big_decimal value_2);
int less_or_not(big_decimal value_1, big_decimal value_2);
int greater_or_not(big_decimal value_1, big_decimal value_2);

// bitwise operations
big_decimal bitwise_and(big_decimal value_1, big_decimal value_2);
big_decimal bitwise_xor(big_decimal value_1, big_decimal value_2);
big_decimal bitwise_not(big_decimal value);

// converters
big_decimal decimal_to_big(s21_decimal value);
int big_to_decimal(s21_decimal *normal, big_decimal big);

// prints
void print_decimal(s21_decimal decimal);
void print_big_decimal(big_decimal decimal);

#endif  // S21_DECIMAL_H_

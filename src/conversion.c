#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int return_code = 0;
  if (dst == NULL) {
    return (return_code = 1);
  } else {
    null_decimal(dst);
    if (src < 0) {
      set_bit(&dst->bits[3], 31);
      src = -src;
    }
    dst->bits[0] = src;
    return_code = 0;
  }
  return return_code;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int return_code = 0;
  int sign = get_bit(src.bits[3], 31);
  if (dst == NULL) {
    return (return_code = 1);
  }
  if ((get_scale(src) == 0 || sign) && src.bits[1] == 0 && src.bits[2] == 0) {
    if ((src.bits[0] > (unsigned)INT32_MIN && sign) ||
        (src.bits[0] > (unsigned)INT32_MAX && !sign))
      return_code = 1;
    else {
      *dst = src.bits[0];
      if (sign) {
        *dst = -(*dst);
      }
    }
  } else
    return_code = 1;
  if (get_scale(src) > 0) {
    s21_decimal truncated = {{0}};
    return_code = s21_truncate(src, &truncated);
    if (truncated.bits[1] != 0 || truncated.bits[2] != 0 ||
        (((truncated.bits[0] > (unsigned)INT32_MIN && sign) ||
          (truncated.bits[0] > (unsigned)INT32_MAX && !sign)))) {
      return_code = 1;
    } else {
      *dst = truncated.bits[0];
      return_code = 0;
      if (sign) *dst = -(*dst);
    }
  }
  return return_code;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int return_code = 0;
  if (src == NAN || src == INFINITY || src == -INFINITY || dst == NULL) {
    return (return_code = 1);
  }
  char buffer[100] = {0};
  char integer[100] = {0};
  int length_before_point = 0, length_after_point = 0;
  int scale = 0;
  int negative = 0;
  if (src < 0) {
    src = -src;
    negative = 1;
  }
  null_decimal(dst);
  unsigned int max_float_value = 9999999;
  sprintf(buffer, "%f\n", src);
  for (int i = 0, j = 0; i < 7 && buffer[i] != 46; i++) {
    integer[j] = buffer[i];
    j++;
  }
  length_before_point = strlen(integer);
  unsigned int number = atoi(integer);
  if (number > max_float_value) {
    dst->bits[0] = max_float_value;
  }
  float remainder = src - (float)number;
  if (remainder > 0 && length_before_point < 8) {
    length_after_point = 7 - length_before_point;
    for (int i = (strlen(integer)), j = 0;
         j < length_after_point && buffer[i] != '\0'; i++) {
      integer[i] = buffer[i + 1];
      j++;
      scale++;
    }
    for (int i = 6; i > length_before_point; i--) {
      if (integer[i] == 48) {
        integer[i] = '\0';
        scale--;
      } else
        break;
    }
    number = atoi(integer);
    dst->bits[0] = number;
    set_scale(scale, dst);
    if (negative == 1) set_sign(dst);
  } else if (remainder == 0) {
    dst->bits[0] = number;
    set_scale(scale, dst);
    if (negative == 1) set_sign(dst);
  }
  return return_code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int return_code = 0;
  if (get_scale(src) > 28 || dst == NULL) {
    return (return_code = 1);
  }
  unsigned long long int sum = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 32; j++) {
      sum += get_bit(src.bits[i], j) * pow(2, j);
    }
  }
  *dst = sum * (1 / pow(10, get_scale(src)));
  if (get_bit(src.bits[3], 31) == 1) *dst = -(*dst);

  return return_code;
}

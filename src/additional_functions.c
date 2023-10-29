#include "s21_decimal.h"

int get_bit(unsigned int num, int index) {
  unsigned int result = 1;
  result = num & (result << index);
  if (result > 0)
    result = 1;
  else
    result = 0;
  return result;
}

void set_bit(unsigned int *num, int index) {  // устанавливает бит на 1
  unsigned int result = 1;
  *num = *num | (result << index);
}

void set_spec_bit(unsigned int *num, int index, int bit) {
  if (bit) {
    set_bit(num, index);
  } else {
    null_bit(num, index);
    
  }
}

void null_bit(unsigned int *num, int index) {  // устанавливает бит на 0
  unsigned int mask = 1;
  mask = ~(mask << index);
  *num = *num & mask;
}

void invert_bit(unsigned int *num, int index) {
  unsigned int mask = 1;
  *num = *num ^ (mask << index);
}

void print_decimal(s21_decimal decimal) {
  printf("decimal: |");
  for (int i = 0; i < 4; i++) {
    printf("%u|", decimal.bits[i]);
  }
  printf("\n");
  printf("binary: | ");
  for (int i = 3; i >= 0; i--) {
    for (int b = 31; b >= 0; b--) {
      printf("%d", get_bit(decimal.bits[i], b));
    }
    printf(" | ");
  }
  printf("\n");
}

void null_decimal(s21_decimal *decimal) {
  for (int i = 0; i < 4; i++) {
    decimal->bits[i] = 0;
  }
}

int get_scale(s21_decimal decimal) {  //считывает экспоненту
  int scale = 0;
  int mask = 16711680;
  scale = (decimal.bits[3] & mask) >> 16;
  return scale;
}

int get_big_scale(big_decimal decimal) {  //считывает экспоненту
  int scale = 0;
  int mask = 16711680;
  scale = (decimal.bits[6] & mask) >> 16;
  return scale;
}

void set_scale(int scale, s21_decimal *decimal) {
  int bit = 0;
  for (int j = 16; j < 24; j++) {
    null_bit(&decimal->bits[3], j);
  }
  for (int i = 0, j = 16; i < 8 && j < 24; i++) {
    if ((bit = get_bit(scale, i)) == 1) {
      set_bit(&decimal->bits[3], j);
    }
    j++;
  }
}

void set_big_scale(int scale, big_decimal *decimal) {
  int bit = 0;
  for (int j = 16; j < 24; j++) {
    null_bit(&decimal->bits[6], j);
  }
  for (int i = 0, j = 16; i < 8 && j < 24; i++) {
    if ((bit = get_bit(scale, i)) == 1) {
      set_bit(&decimal->bits[6], j);
    }
    j++;
  }
}

void set_sign(s21_decimal *decimal) { set_bit(&decimal->bits[3], 31); }

void normalise(big_decimal *decimal_1, big_decimal *decimal_2) {
  int scale_1 = get_big_scale(*decimal_1);
  int scale_2 = get_big_scale(*decimal_2);
  int scale = 0;
  if (scale_1 > scale_2) {
    scale = scale_1 - scale_2;
    for (int i = 0; i < scale; i++) {
      multiply_by_10(decimal_2);
    }
    set_big_scale(scale_1, decimal_2);
  } else if (scale_2 > scale_1) {
    scale = scale_2 - scale_1;
    for (int i = 0; i < scale; i++) {
      multiply_by_10(decimal_1);
    }
    set_big_scale(scale_2, decimal_1);
  }
}

void multiply_by_10(big_decimal *decimal) {
  big_decimal temporary_1 = *decimal;
  big_decimal temporary_2 = *decimal;
  shift_left(&temporary_1, 1);
  shift_left(&temporary_2, 3);
  *decimal = summ(temporary_1, temporary_2);
}

void divide_by_10(big_decimal *decimal) {
  unsigned long long int last = decimal->bits[5];
  unsigned int remainder = 0;
  for (int i = 5; i >= 0; i--) {
    remainder = last % 10;
    decimal->bits[i] = last / 10;
    last = remainder * (4294967296) + decimal->bits[i - 1];
  }
}

void shift_left(big_decimal *decimal, int step) {
  for (int j = 0; j < step; j++) {
    int bit_last0 = get_bit(decimal->bits[0], 31);
    int bit_last1 = get_bit(decimal->bits[1], 31);
    int bit_last2 = get_bit(decimal->bits[2], 31);
    int bit_last3 = get_bit(decimal->bits[3], 31);
    int bit_last4 = get_bit(decimal->bits[4], 31);
    for (int i = 0; i < 6; i++) {
      decimal->bits[i] = decimal->bits[i] << 1;
    }
    set_spec_bit(&decimal->bits[1], 0, bit_last0);
    set_spec_bit(&decimal->bits[2], 0, bit_last1);
    set_spec_bit(&decimal->bits[3], 0, bit_last2);
    set_spec_bit(&decimal->bits[4], 0, bit_last3);
    set_spec_bit(&decimal->bits[5], 0, bit_last4);
  }
}

void shift_right(big_decimal *decimal, int step) {
  for (int j = 0; j < step; j++) {
    int bit_last5 = get_bit(decimal->bits[5], 0);
    int bit_last4 = get_bit(decimal->bits[4], 0);
    int bit_last3 = get_bit(decimal->bits[3], 0);
    int bit_last2 = get_bit(decimal->bits[2], 0);
    int bit_last1 = get_bit(decimal->bits[1], 0);
    for (int i = 0; i < 6; i++) {
      decimal->bits[i] = decimal->bits[i] >> 1;
    }
    set_spec_bit(&decimal->bits[4], 31, bit_last5);
    set_spec_bit(&decimal->bits[3], 31, bit_last4);
    set_spec_bit(&decimal->bits[2], 31, bit_last3);
    set_spec_bit(&decimal->bits[1], 31, bit_last2);
    set_spec_bit(&decimal->bits[0], 31, bit_last1);
  }
}

//возвращает 0 если децимал пустой или 1 если не равен нулю
int decimal_not_equal_zero(big_decimal decimal) {
  int result = 0;
  for (int i = 0; i < 6; i++) {
    if (decimal.bits[i] == 0) {
      result = 0;
    } else {
      result = 1;
      break;
    }
  }
  return result;
}

// Natasha:

big_decimal bitwise_and(big_decimal value_1, big_decimal value_2) {
  big_decimal result = {{0}};
  result.bits[0] = value_1.bits[0] & value_2.bits[0];
  result.bits[1] = value_1.bits[1] & value_2.bits[1];
  result.bits[2] = value_1.bits[2] & value_2.bits[2];
  result.bits[3] = value_1.bits[3] & value_2.bits[3];
  result.bits[4] = value_1.bits[4] & value_2.bits[4];
  result.bits[5] = value_1.bits[5] & value_2.bits[5];
  return result;
}

big_decimal bitwise_xor(big_decimal value_1, big_decimal value_2) {
  big_decimal result = {{0}};
  result.bits[0] = value_1.bits[0] ^ value_2.bits[0];
  result.bits[1] = value_1.bits[1] ^ value_2.bits[1];
  result.bits[2] = value_1.bits[2] ^ value_2.bits[2];
  result.bits[3] = value_1.bits[3] ^ value_2.bits[3];
  result.bits[4] = value_1.bits[4] ^ value_2.bits[4];
  result.bits[5] = value_1.bits[5] ^ value_2.bits[5];
  return result;
}

big_decimal bitwise_not(big_decimal value) {
  big_decimal result = {{0}};
  result.bits[0] = ~value.bits[0];
  result.bits[1] = ~value.bits[1];
  result.bits[2] = ~value.bits[2];
  result.bits[3] = ~value.bits[3];
  result.bits[4] = ~value.bits[4];
  result.bits[5] = ~value.bits[5];
  return result;
}

void null_big_decimal(big_decimal *value) {
  for (int i = 0; i < 7; i++) {
    value->bits[i] = 0;
  }
}

big_decimal decimal_to_big(s21_decimal value) {
  big_decimal result = {{0}};
  result.bits[0] = value.bits[0];
  result.bits[1] = value.bits[1];
  result.bits[2] = value.bits[2];
  result.bits[3] = 0;
  result.bits[4] = 0;
  result.bits[5] = 0;
  result.bits[6] = value.bits[3];
  return result;
}

int big_to_decimal(s21_decimal *normal, big_decimal big) {
  int return_code = 0;
  int sign = get_bit(big.bits[6], 31);
  int exp = get_big_scale(big);
  if (decimal_not_equal_zero(big) == 0) {
    return_code = 0;  // to fix
  } else {
    while (big_3_bits_check(big) != 0 && exp > 0) {
      divide_by_10(&big);
      exp--;
    }
    while (exp > 28) {
      divide_by_10(&big);
      exp--;
    }
    set_big_scale(exp, &big);
    if (sign && big.bits[3] != 0) {
      return_code = 2;  // number is too small
    }
    if (!sign && big.bits[3] != 0) {
      return_code = 1;  // number is too large;
    }
  }
  if (return_code == 0) {
    for (int i = 0; i < 4; i++) {
      normal->bits[i] = big.bits[i];
    }
    normal->bits[3] = big.bits[6];
  }
  return return_code;
}

void print_big_decimal(big_decimal decimal) {
  printf("decimal: |");
  for (int i = 0; i < 7; i++) {
    printf("%u|", decimal.bits[i]);
  }
  printf("\n");
  printf("binary: | ");
  for (int i = 6; i >= 0; i--) {
    for (int b = 31; b >= 0; b--) {
      printf("%d", get_bit(decimal.bits[i], b));
    }
    printf(" | ");
  }
  printf("\n");
}

//проверяет равны ли битс 3, 4, 5 нулю
//возвращает 0 если равны нулю и 1 если не равны
int big_3_bits_check(big_decimal value) {
  int result = 1;
  if (value.bits[3] == 0 && value.bits[4] == 0 && value.bits[5] == 0) {
    result = 0;
  }
  return result;
}

//возвращает 1 если true и 0 если false
int greater_or_equal(big_decimal value_1, big_decimal value_2) {
  int return_code = 0;
  for (int i = 0; i < 6; i++) {
    if (value_1.bits[i] == value_2.bits[i] && value_1.bits[i] == 0) {
      return (return_code = 1);
    } else
      break;
  }
  int sign_1 = get_bit(value_1.bits[6], 31);
  int sign_2 = get_bit(value_2.bits[6], 31);
  int exp_1 = get_big_scale(value_1);
  int exp_2 = get_big_scale(value_2);
  int flag = 0;
  if ((sign_1 ^ sign_2) && sign_1) {
    return_code = 0;  // false
  } else if ((sign_1 ^ sign_2) && sign_2) {
    return_code = 1;  // true
  } else {
    if (exp_1 != exp_2) {
      normalise(&value_1, &value_2);
    }
    for (int i = 5; i >= 0 && !flag; i--) {
      if (!flag && ((value_1.bits[i] == value_2.bits[i] && !sign_1) ||
                    (value_2.bits[i] == value_1.bits[i] && sign_1))) {
        return_code = 1;
      } else if (!flag && ((value_1.bits[i] > value_2.bits[i] && !sign_1) ||
                           (value_2.bits[i] > value_1.bits[i] && sign_1))) {
        return_code = 1;
        flag = 1;
      } else {
        return_code = 0;
        flag = 1;
      }
    }
  }
  return return_code;
}

int less_or_equal(big_decimal value_1, big_decimal value_2) {
  int return_code = 0;
  for (int i = 0; i < 6; i++) {
    if (value_1.bits[i] == value_2.bits[i] && value_1.bits[i] == 0) {
      return (return_code = 1);
    } else
      break;
  }
  int sign_1 = get_bit(value_1.bits[6], 31);
  int sign_2 = get_bit(value_2.bits[6], 31);
  int exp_1 = get_big_scale(value_1);
  int exp_2 = get_big_scale(value_2);
  int flag = 0;
  if ((sign_1 ^ sign_2) && sign_1) {
    return_code = 1;  // true
  } else if ((sign_1 ^ sign_2) && sign_2) {
    return_code = 0;  // false
  } else {
    if (exp_1 != exp_2) {
      normalise(&value_1, &value_2);
    }
    for (int i = 5; i >= 0 && !flag; i--) {
      if (!flag && ((value_1.bits[i] == value_2.bits[i] && !sign_1) ||
                    (value_2.bits[i] == value_1.bits[i] && sign_1))) {
        return_code = 1;
      } else if (!flag && ((value_1.bits[i] < value_2.bits[i] && !sign_1) ||
                           (value_2.bits[i] < value_1.bits[i] && sign_1))) {
        return_code = 1;
        flag = 1;
      } else {
        return_code = 0;
        flag = 1;
      }
    }
  }
  return return_code;
}

//возвращает 1 если равны, 0 если не равны
int equal_or_not(big_decimal value_1, big_decimal value_2) {
  int return_code = 0;
  for (int i = 0; i < 6; i++) {
    if (value_1.bits[i] == value_2.bits[i] && value_1.bits[i] == 0) {
      return (return_code = 1);
    } else
      break;
  }
  int sign_1 = get_bit(value_1.bits[6], 31);
  int sign_2 = get_bit(value_2.bits[6], 31);
  int exp_1 = get_big_scale(value_1);
  int exp_2 = get_big_scale(value_2);
  if (sign_1 ^ sign_2) {
    return_code = 0;  // false
  } else {
    if (exp_1 != exp_2) {
      normalise(&value_1, &value_2);
    }
    for (int i = 5; i >= 0; i--) {
      if (value_1.bits[i] == value_2.bits[i]) {
        return_code = 1;
      } else {
        return_code = 0;
        break;
      }
    }
  }
  return return_code;
}

int less_or_not(big_decimal value_1, big_decimal value_2) {
  int return_code = 0;
  int sign_1 = get_bit(value_1.bits[6], 31);
  int sign_2 = get_bit(value_2.bits[6], 31);
  int exp_1 = get_big_scale(value_1);
  int exp_2 = get_big_scale(value_2);
  int flag = 0;
  if ((sign_1 ^ sign_2) && sign_1) {
    return_code = 1;  // true
  } else if ((sign_1 ^ sign_2) && sign_2) {
    return_code = 0;  // false
  } else {
    if (exp_1 != exp_2) {
      normalise(&value_1, &value_2);
    }
    for (int i = 5; i >= 0 && !flag; i--) {
      if (!flag && i == 0 &&
          ((value_1.bits[i] < value_2.bits[i] && !sign_1) ||
           (value_2.bits[i] < value_1.bits[i] && sign_1))) {
        return_code = 1;
        flag = 1;
      } else if (!flag && i != 0 &&
                 ((value_1.bits[i] == value_2.bits[i] && !sign_1) ||
                  (value_2.bits[i] == value_1.bits[i] && sign_1))) {
        return_code = 1;
      } else if (!flag && i != 0 &&
                 ((value_1.bits[i] < value_2.bits[i] && !sign_1) ||
                  (value_2.bits[i] < value_1.bits[i] && sign_1))) {
        return_code = 1;
        flag = 1;
      } else {
        return_code = 0;
        flag = 1;
      }
    }
  }
  return return_code;
}

int greater_or_not(big_decimal value_1, big_decimal value_2) {
  int return_code = 0;
  int sign_1 = get_bit(value_1.bits[6], 31);
  int sign_2 = get_bit(value_2.bits[6], 31);
  int exp_1 = get_big_scale(value_1);
  int exp_2 = get_big_scale(value_2);
  int flag = 0;
  if ((sign_1 ^ sign_2) && sign_1) {
    return_code = 0;  // false
  } else if ((sign_1 ^ sign_2) && sign_2) {
    return_code = 1;  // true
  } else {
    if (exp_1 != exp_2) {
      normalise(&value_1, &value_2);
    }
    for (int i = 5; i >= 0 && !flag; i--) {
      if (!flag && i == 0 &&
          ((value_1.bits[i] > value_2.bits[i] && !sign_1) ||
           (value_2.bits[i] > value_1.bits[i] && sign_1))) {
        return_code = 1;
        flag = 1;
      } else if (!flag && i != 0 &&
                 ((value_1.bits[i] == value_2.bits[i] && !sign_1) ||
                  (value_2.bits[i] == value_1.bits[i] && sign_1))) {
        return_code = 1;
      } else if (!flag && i != 0 &&
                 ((value_1.bits[i] > value_2.bits[i] && !sign_1) ||
                  (value_2.bits[i] > value_1.bits[i] && sign_1))) {
        return_code = 1;
        flag = 1;
      } else {
        return_code = 0;
        flag = 1;
      }
    }
  }
  return return_code;
}

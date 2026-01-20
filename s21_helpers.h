#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_is_matrix_size_equal(matrix_t* A, matrix_t* B);

int s21_is_correct_matrix(matrix_t* A);

int s21_is_square_matrix(matrix_t* A);

void s21_create_minor_matrix(int ind_rows, int ind_columns, matrix_t* A,
                             matrix_t* result);

#endif

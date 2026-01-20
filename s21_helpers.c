#include "s21_helpers.h"

int s21_is_correct_matrix(matrix_t* A) {
  int is_correct = 1;
  if (A == NULL || A->matrix == NULL || A->rows < 1 || A->columns < 1) {
    is_correct = 0;
  }
  return is_correct;
}

int s21_is_square_matrix(matrix_t* A) {
  int is_square = 1;
  if (A->rows != A->columns) {
    is_square = 0;
  }
  return is_square;
}

int s21_is_matrix_size_equal(matrix_t* A, matrix_t* B) {
  int size_equal = 0;
  if (A->rows == B->rows && A->columns == B->columns) {
    size_equal = 1;
  }
  return size_equal;
}

void s21_create_minor_matrix(int ind_rows, int ind_columns, matrix_t* A,
                             matrix_t* result) {
  for (int i = 0, j = 0; i < A->rows; i++) {
    if (ind_rows == i) continue;
    for (int m = 0, n = 0; m < A->columns; m++) {
      if (ind_columns == m) continue;
      result->matrix[j][n] = A->matrix[i][m];
      n++;
    }
    j++;
  }
}

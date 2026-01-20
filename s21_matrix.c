#include "s21_helpers.c"

int s21_create_matrix(int rows, int columns, matrix_t* result) {
  if (rows < 1 || columns < 1 || result == NULL) {
    return INCORRECT_MATRIX;
  }
  int alloc_error = 0;
  result->rows = rows;
  result->columns = columns;
  result->matrix = calloc(rows, sizeof(double*));
  if (result->matrix == NULL)
    alloc_error = INCORRECT_MATRIX;
  else {
    for (int i = 0; i < rows && !alloc_error; i++) {
      result->matrix[i] = calloc(columns, sizeof(double));
      if (result->matrix[i] == NULL) {
        s21_remove_matrix(result);
        alloc_error = INCORRECT_MATRIX;
      }
    }
  }
  return alloc_error;
}

void s21_remove_matrix(matrix_t* A) {
  if (s21_is_correct_matrix(A)) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
      A->matrix[i] = NULL;
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t* A, matrix_t* B) {
  int result = FAILURE;
  if (s21_is_correct_matrix(A) && s21_is_correct_matrix(B) &&
      s21_is_matrix_size_equal(A, B)) {
    result = SUCCESS;
    for (int i = 0; i < A->rows && result == SUCCESS; i++) {
      for (int j = 0; j < A->columns && result == SUCCESS; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > PROXIMITY) {
          result = FAILURE;
        }
      }
    }
  }
  return result;
}

int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int status = OK;
  if (s21_is_correct_matrix(A) && s21_is_correct_matrix(B)) {
    if (!s21_is_matrix_size_equal(A, B)) {
      status = CALCULATION_ERROR;
    } else if (s21_create_matrix(A->rows, A->columns, result) == OK) {
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
      }
    } else {
      status = INCORRECT_MATRIX;
    }
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int status = OK;
  if (s21_is_correct_matrix(A) && s21_is_correct_matrix(B)) {
    if (!s21_is_matrix_size_equal(A, B)) {
      status = CALCULATION_ERROR;
    } else if (s21_create_matrix(A->rows, A->columns, result) == OK) {
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
      }
    } else {
      status = INCORRECT_MATRIX;
    }
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

int s21_mult_number(matrix_t* A, double number, matrix_t* result) {
  int status = OK;
  if (s21_is_correct_matrix(A)) {
    if (s21_create_matrix(A->rows, A->columns, result) == OK) {
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] * number;
        }
      }
    } else
      status = INCORRECT_MATRIX;
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int status = OK;
  if (s21_is_correct_matrix(A) && s21_is_correct_matrix(B)) {
    if (A->columns != B->rows) {
      status = CALCULATION_ERROR;
    } else if (s21_create_matrix(A->rows, B->columns, result) == OK) {
      int k = A->columns;
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++) {
          for (int m = 0; m < k; m++) {
            result->matrix[i][j] += A->matrix[i][m] * B->matrix[m][j];
          }
        }
      }
    } else
      status = INCORRECT_MATRIX;
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

int s21_transpose(matrix_t* A, matrix_t* result) {
  int status = OK;
  if (s21_is_correct_matrix(A)) {
    if (s21_create_matrix(A->columns, A->rows, result) == OK) {
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++) {
          result->matrix[i][j] = A->matrix[j][i];
        }
      }
    } else {
      s21_remove_matrix(result);
      status = INCORRECT_MATRIX;
    }
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

int s21_calc_complements(matrix_t* A, matrix_t* result) {
  int status = OK;
  matrix_t M = {0};
  if (s21_is_correct_matrix(A) && !s21_is_square_matrix(A)) {
    status = CALCULATION_ERROR;
  } else if (s21_is_correct_matrix(A) && A->rows == 1 &&
             s21_create_matrix(A->rows, A->columns, result) == OK) {
    result->matrix[0][0] = 1;
  } else if (s21_is_correct_matrix(A) &&
             s21_create_matrix(A->rows - 1, A->columns - 1, &M) == OK &&
             s21_create_matrix(A->rows, A->columns, result) == OK) {
    for (int i = 0; i < result->rows && status == OK; i++) {
      for (int j = 0; j < result->columns; j++) {
        double det = 0.0;
        s21_create_minor_matrix(i, j, A, &M);
        status = s21_determinant(&M, &det);
        result->matrix[i][j] = pow(-1, i + j) * det;
      }
    }
    s21_remove_matrix(&M);
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

int s21_determinant(matrix_t* A, double* result) {
  int status = OK;
  double temp_result = 0.0;
  matrix_t M = {0};
  if (s21_is_correct_matrix(A) && !s21_is_square_matrix(A)) {
    status = CALCULATION_ERROR;
  } else if (s21_is_correct_matrix(A) && A->rows == 1) {
    temp_result = A->matrix[0][0];
  } else if (s21_is_correct_matrix(A) && A->rows == 2) {
    temp_result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else if (s21_is_correct_matrix(A) &&
             s21_create_matrix(A->rows - 1, A->columns - 1, &M) == OK) {
    for (int j = 0; j < A->columns && status == OK; j++) {
      double det = 0.0;
      s21_create_minor_matrix(0, j, A, &M);
      status = s21_determinant(&M, &det);
      if (status == OK) {
        temp_result += pow(-1, j) * A->matrix[0][j] * det;
      }
    }
    s21_remove_matrix(&M);
  } else {
    status = INCORRECT_MATRIX;
  }
  if (status == OK) {
    *result = temp_result;
  }
  return status;
}

int s21_inverse_matrix(matrix_t* A, matrix_t* result) {
  int status = OK;
  double det = 0.0;
  matrix_t M = {0}, TM = {0};
  if (s21_is_correct_matrix(A) && !s21_is_square_matrix(A)) {
    status = CALCULATION_ERROR;
  } else if (s21_is_correct_matrix(A)) {
    status = s21_determinant(A, &det);
    if (status == CALCULATION_ERROR || fabs(det) < PROXIMITY) {
      status = CALCULATION_ERROR;
    } else if (status == OK) {
      status = s21_calc_complements(A, &M);
      if (status == OK) {
        status = s21_transpose(&M, &TM);
      }
      if (status == OK) {
        status = s21_mult_number(&TM, 1.0 / det, result);
      }
      s21_remove_matrix(&M);
      s21_remove_matrix(&TM);
    } else {
      status = INCORRECT_MATRIX;
    }
  } else {
    status = INCORRECT_MATRIX;
  }
  return status;
}

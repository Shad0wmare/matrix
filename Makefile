CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -std=c11 -lm
TEST_FLAGS = -lcheck -lpthread -lrt -lsubunit
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LIB_SRC=s21_matrix.c
LIB_OBJ=s21_matrix.o
TESTS_SRC=s21_tests.c
EXE = test_out

all: s21_matrix.a test gcov_report

clean:
	rm -f *.o *.out *.gcno *.gcda *.info *.a $(EXE)
	rm -rf report/

rebuild: clean all

test:
	$(CC) $(LIB_SRC) $(TESTS_SRC) $(CFLAGS) $(TEST_FLAGS) -o $(EXE)
	./$(EXE)

valgrind:
	$(CC) $(LIB_SRC) $(TESTS_SRC) $(CFLAGS) $(TEST_FLAGS) -o $(EXE)
	@CK_FORK=no valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXE)

s21_matrix.a:
	$(CC) -c $(LIB_SRC) -o $(LIB_OBJ)
	ar rcs s21_matrix.a s21_matrix.o
	ranlib $@

gcov_report:
	$(CC) $(LIB_SRC) $(TESTS_SRC) $(CFLAGS) $(TEST_FLAGS) $(GCOV_FLAGS) -o $(EXE)
	./$(EXE)
	lcov -t "Report" -c -d . --output-file coverage.info
	genhtml coverage.info --output-directory report/
	open report/index.html

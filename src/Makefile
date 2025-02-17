CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -pedantic
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
LDFLAGS=-lcheck -lm -lsubunit -lpthread -lrt

SOURCES=$(filter-out test.c, $(wildcard *.c))
OBJ=$(patsubst %.c,%.o,$(SOURCES))


all: s21_string.a

s21_string.a: $(OBJ)
	@ar rcs $@ $^
	@ranlib $@

$(OBJ): $(SOURCES)
	@$(CC) $(CFLAGS) -c $^


test_compose.check:
	@cat test_*.check > test_compose.check
test.c: test_compose.check
	@checkmk clean_mode=1 test_compose.check > $@
	@clang-format -i -style=Google *.h *.c
test: test.c s21_string.a 
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o test
	./test


gcov_report: clean test.c
	$(CC) $(GCOV_FLAGS) $(SOURCES) test.c -o s21_test $(LDFLAGS)
	./s21_test
	lcov -t "s21_test" --rc lcov_branch_coverage=1 -o coverage.info --directory . --capture --exclude 'test*'
	genhtml --branch-coverage --output-directory gcov_report --legend coverage.info

open:
	open gcov_report/index.html

check: test
	valgrind --tool=memcheck --leak-check=yes ./test

clang:
	clang-format -i -style=Google *.h *.c
	
clean:
	@rm -rf s21_string.a test *.o *.gc* gcov_report test.c *.info test_compose.check s21_test
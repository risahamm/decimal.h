CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
OS = $(shell uname)
FUNCTIONS = conversion.c comparison.c other.c arithmetic.c additional_functions.c

ifeq ($(OS),Linux)
    det_OS = -lcheck -lm -lrt -lpthread -lsubunit
else
    det_OS = -lcheck -lm -lpthread
endif

all: clean s21_decimal.a test gcov_report

s21_decimal.a: s21_decimal.o	
	ar rcs s21_decimal.a *.o 
	ranlib s21_decimal.a

s21_decimal.o: $(FUNCTIONS) s21_decimal.h
	$(CC) $(CFLAGS) $(FUNCTIONS) -c

test: clean
	$(CC) $(CFLAGS) $(FUNCTIONS) --coverage test.c -o testresult $(det_OS)
	./testresult

gcov_report: test
	lcov -t "s21_decimal_test" -o decimal.info -c -d .
	genhtml -o report/ decimal.info
	open report/index.html
	rm -rf *.gcno *gcda *.gcov

clang:
	clang-format -style=Google -n *.c *.h

style:
	clang-format -style=Google -i *.c *.h

clean:
	rm -rf testresult *.gcda *.gcno *.o *.info *.a tests/*.gcno tests/report test.dSYM

install_brew:
	cd ~
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	
install_lcov:
	brew install lcov

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all make test
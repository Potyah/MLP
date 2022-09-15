.PHONY: all clean check tests gcov_report dvi install uninstall dist
XX=g++
APP=$(BD)MLP.app

FILES= model/cvsReader.cpp \
	   model/graphNet.cpp \
	   model/matrixNet.cpp \
	   model/neuraltransformations.cpp \
	   model/neuron.cpp \
	   model/s21_matrix_oop.cpp \
	   model/layer.cpp \
	   controller/controller.cpp

DIRS= view \
	  model \
	  controller \
	  doc \
	  letters \
	  weights

LDFLAGS= -Wall -Wextra -Werror -std=c++17 -g
OS := $(shell uname -s)
TEST_FLAGS= -lgtest -lgtest_main -lpthread
CFLAGS= -fprofile-arcs -ftest-coverage
BD= build/

ifeq ($(OS), Darwin)
	CC += -D OS_MAC
else
	CC += -D OS_LINUX
endif

all: build install tests

build: clean copy
	cd $(BD); qmake MLP.pro
	make -C $(BD)

copy:
	@mkdir build
	cp *.pro *.cpp $(BD)
	cp *.png common.h $(BD)
	rsync -rupE $(DIRS) $(BD)

install: uninstall build
	@mkdir $(HOME)/Applications/MLP/ 2>/dev/null
	cp -rf $(APP) $(HOME)/Applications/MLP/
	make clean

uninstall: 
	rm -rf $(HOME)/Applications/MLP

dist:
	rm -rf MLP
	mkdir MLP
	cp Makefile *.pro *.cpp MLP/
	rsync -rupE $(DIRS)  MLP/
	tar -cvzf $(HOME)/Desktop/MLP.tgz MLP/
	rm -rf MLP/

dvi:
	open doc/index.html

tests: clean
	$(XX) $(LDFLAGS) $(FLAGS) tests/tests.cpp $(FILES) -lstdc++ $(TEST_FLAGS) -o test
	./test

clean:
	rm -rf build test *.a *.o *.cfg *.gcda *.gcno *.html *.info *.dSYM report

check: 
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	cp ../materials/linters/suppressions.cfg suppressions.cfg
	python3 ../materials/linters/cpplint.py --extension=cpp model/*.cpp controller/*.cpp view/*.cpp model/*.h controller/*.h view/*.h
	cppcheck -q --std=c++17 --enable=all --language=c++ --suppressions-list=suppressions.cfg model/*.cpp controller/*.cpp view/*.cpp model/*.h controller/*.h view/*.h

gcov_report: clean
	$(XX) $(FLAGS) $(CFLAGS) $(FILES) tests/tests.cpp -lstdc++ $(TEST_FLAGS) -o test
	./test
	lcov --no-external -c -d . -o tests.info
	genhtml tests.info -o report
ifeq ($(OS), Darwin)
	open -a "Google Chrome" report/index.html
else
	xdg-open report/index.html
endif

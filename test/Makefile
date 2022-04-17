CC=g++
CXXARGS=-c -Wall -Wextra -pedantic -I../inc/ -g -fprofile-arcs -ftest-coverage
LDARGS=-fprofile-arcs -ftest-coverage

SOURCES=$(shell find . -name '*.cpp')
OBJECTS=$(SOURCES:./src/%.cpp=./obj/%.o)

all: runtest

coverage: runtest
	lcov --directory "obj/" --output-file coverage.info -c \
		--exclude '*doctest*' --exclude '/usr/*'
	genhtml coverage.info --output-directory "html/"
	firefox html/index.html

runtest: ./test
	./test

dep/conanbuildinfo.mak: conanfile.txt
	cd dep && conan install ..

-include dep/conanbuildinfo.mak
CXXADD=$(CONAN_CXXFLAGS) $(CONAN_INCLUDE_DIRS:%=-I%)
LDADD=$(CONAN_LIB_DIRS:%=-L%) $(CONAN_LIBS:%=-l%) $(CONAN_SYSTEM_LIBS:%=-l%)

obj/%.o: src/%.cpp Makefile ../inc/event.hpp ../inc/property.hpp
	$(CC) $(CXXARGS) $(CXXADD) $< -o $@

./test: dep/conanbuildinfo.mak $(OBJECTS)
	$(CC) $(LDARGS) $(LDADD) $(OBJECTS) -o $@

clean:
	rm -f dep/*
	rm -f obj/*
	rm -f ./test

.PHONY: all runtest clean

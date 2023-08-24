APP ?= ./test
LIB = ./lib/libmemory.so
LOG_DIR = ./log

all: compile_lib compile_test rmdir_log mkdir_log

compile_lib: libmemory
compile_test: test

# Compile simple test file(test.c)
test: test.c
	gcc -g -O0 -o test test.c


# Compile library
libmemory: ./lib/*
	gcc -fPIC -shared -lpthread ./lib/*.c -o $(LIB) -ldl

# Using libmemory.so to run app
run: 
	LD_PRELOAD=$(LIB) $(APP)

# Using libmemory.so to run app in GDB
gdb:
	gdb -q -ex 'set exec-wrapper env LD_PRELOAD=$(LIB)' $(APP) | tee gdb.log

clean:
	rm ./test $(LIB)
	if test -d $(LOG_DIR); then rm -r ./log; fi

mkdir_log:
	mkdir $(LOG_DIR)

rmdir_log:
	if test -d $(LOG_DIR); then rm -r ./log; fi
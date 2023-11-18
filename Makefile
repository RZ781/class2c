CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror

all: class2c java/HelloWorld.class;
class2c: src/*.c include/*.h
	$(CC) $(CFLAGS) -Iinclude src/*.c -o class2c
%.class: %.java
	javac $<

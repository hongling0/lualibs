CC=gcc
CFLAG=-std=gnu99 -O3 -Wall -Wextra

SOURCE:=$(wildcard *.c)
TARGET:=$(patsubst %.c,%.so,$(SOURCE))

all:$(TARGET)

%.so:%.c
	$(CC) $(CFLAG) -fPIC --shared -o $@ $^

list:
	@echo $(SOURCE)

test:
	lua uniq_test.lua

clean:
	rm $(TARGET)


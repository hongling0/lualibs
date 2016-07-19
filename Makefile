CC=gcc
CFLAG=-std=gnu99 -O3 -Wall -Wextra -g

SOURCE:=$(wildcard *.c)
TARGET:=$(patsubst %.c,%.so,$(SOURCE))

all:$(TARGET)

%.so:%.c
	$(CC) $(CFLAG) -fPIC --shared -o $@ $^

list:
	@echo $(SOURCE)

clean:
	rm $(TARGET)


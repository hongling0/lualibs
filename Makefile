CC=gcc
CFLAG=-std=gnu99 -O2 -Wall -Wextra -DLUA_COMPAT_5_2 -DLUA_USE_LINUX

SOURCE:=$(wildcard *.c)
TARGET:=$(patsubst %.c,%.so,$(SOURCE))

all:$(TARGET)

%.so:%.c
	$(CC) $(CFLAG) -fPIC --shared -o $@ $^

list:
	@echo $(SOURCE)

clean:
	rm $(TARGET)


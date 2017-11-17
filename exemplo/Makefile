
CC=gcc
LIB_DIR=../lib
FLAGS = -L$(LIB_DIR) -lt2fs -Wall -m32 -std=c99

all: t2fstst

t2fstst: t2fstst.c $(LIB_DIR)/libt2fs.a
	$(CC) -o t2fstst t2fstst.c $(FLAGS)

clean:
	rm -rf t2fstst
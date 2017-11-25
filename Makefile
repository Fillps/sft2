
CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TST_DIR=./teste

all: regra1 regra2

regra1: $(SRC_DIR)/*.c
	$(CC) -c $(SRC_DIR)/*.c -L$(LIB_DIR) -I$(INC_DIR) -Wall -m32
	mv *.o $(BIN_DIR)

regra2:  $(BIN_DIR)/t2fs.o $(LIB_DIR)/apidisk.o
	ar crs $(LIB_DIR)/libt2fs.a $(BIN_DIR)/*.o $(LIB_DIR)/apidisk.o

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o




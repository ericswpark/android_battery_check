CC=gcc
CFLAGS=-Wall -Werror  -g
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))

LIB=android_battery_check

all: $(LIB)

$(LIB): $(OBJ) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJ)/%.o: $(SRC)/%.cpp $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir $@

.PHONY: clean

clean:
	$(RM) -r $(OBJ) $(TESTBINS) $(LIB)

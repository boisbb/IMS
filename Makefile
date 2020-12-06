CC=g++
CFLAGS=-std=c++11

all: model

model: model.cpp
	$(CC) model.cpp $(CFLAGS) -o model

run_study:
	./model --study-results

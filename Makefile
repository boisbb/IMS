CC=g++
CFLAGS=-std=c++11

all: model

model: src/model.cpp
	$(CC) src/model.cpp $(CFLAGS) -o model

clean:
	rm model

run:
	./model $(ARGS)

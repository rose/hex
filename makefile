CC=g++
CPPFLAGS= -Wall -g -std=c++11

SRC= main.cpp hex.cpp gameboard.cpp board.cpp ufind.cpp boardsim.cpp player.cpp

OBJ=$(SRC:.cpp=.o)

prog: $(OBJ) $(HDR) 
	$(CC) -o hex $(CPPFLAGS) $^ 

test: 
	make clean && make prog && ./hex && make clean

%.o: %.cpp $(HDR)
	$(CC) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean

clean:
	rm -f *.o hex

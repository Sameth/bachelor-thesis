all: common.h graph.h graph.cpp superstring.cpp
	g++ -Wall -Wextra -O2 -DNDEBUG -I ~/include -L ~/lib  -std=c++11 -o superstring.bin superstring.cpp graph.cpp -lsdsl -ldivsufsort -ldivsufsort64

debug: common.h graph.h graph.cpp superstring.cpp
	g++ -Wall -Wextra -O2 -DNDEBUG -I ~/include -L ~/lib -g -std=c++11 -o superstring.bin superstring.cpp graph.cpp -lsdsl -ldivsufsort -ldivsufsort64

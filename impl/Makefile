all: common.h graph.h graph.cpp sr-index.cpp
	g++ -Wall -Wextra -O2 -DNDEBUG -I ~/include -L ~/lib  -std=c++11 -o sr-index.bin sr-index.cpp graph.cpp util.cpp -lsdsl -ldivsufsort -ldivsufsort64 -lboost_system -lboost_filesystem

debug: common.h graph.h graph.cpp sr-index.cpp
	g++ -Wall -Wextra -O2 -DNDEBUG -I ~/include -L ~/lib -g -std=c++11 -o sr-index.bin sr-index.cpp graph.cpp util.cpp -lsdsl -ldivsufsort -ldivsufsort64 -lboost_system -lboost_filesystem

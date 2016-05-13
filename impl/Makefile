all: common.h graph.h graph.cpp sr-index.cpp
	g++ -Wall -Wextra -O2 -DNDEBUG -I ~/include -L ~/lib  -std=c++11 -o sr-index.bin sr-index.cpp graph.cpp util.cpp benchmark/test.cpp -lsdsl -ldivsufsort -ldivsufsort64 -lboost_system -lboost_filesystem

debug: common.h graph.h graph.cpp sr-index.cpp
	g++ -Wall -Wextra -O2 -DNDEBUG -I ~/include -L ~/lib -g -std=c++11 -o sr-index.bin sr-index.cpp graph.cpp util.cpp benchmark/test.cpp -lsdsl -ldivsufsort -ldivsufsort64 -lboost_system -lboost_filesystem

benchmarks: graph.cpp sr-index.cpp benchmark/query.cpp
	g++ -Wall -Wextra -O2 -DNDEBUG -I ~/include -L ~/lib -g -std=c++11 -o benchmark/query.bin sr-index.cpp graph.cpp util.cpp benchmark/query.cpp -lsdsl -ldivsufsort -ldivsufsort64 -lboost_system -lboost_filesystem
	g++ -Wall -Wextra -O2 -DNDEBUG -I ~/include -L ~/lib -g -std=c++11 -o benchmark/test.bin sr-index.cpp graph.cpp util.cpp benchmark/test.cpp -lsdsl -ldivsufsort -ldivsufsort64 -lboost_system -lboost_filesystem

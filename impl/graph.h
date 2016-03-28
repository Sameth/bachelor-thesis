#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include "common.h"

using namespace std;

class Graph {
    map <int_t, int_t> label_compress;
    vector <int_t> label_decompress;
    vector <vector <int_t> > edges;
    static int number_of_vertices = 0;
    public:
        void add_edge(int_t, int_t);
};

#endif //GRAPH_H

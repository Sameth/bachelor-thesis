#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <set>
#include "common.h"

using namespace std;

class Graph {
    map <int_t, int_t> label_compress;
    vector <int_t> label_decompress;
    vector <set <int_t> > edges;
    vector <vector <int_t> > edges_for_euler;
    int number_of_vertices = 0;
    void construct_edges_for_euler();
    public:
        void add_edge(int_t, int_t);
};

#endif //GRAPH_H

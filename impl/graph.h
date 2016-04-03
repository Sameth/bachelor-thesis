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
    int k;
    void construct_edges_for_euler();
    void random_assignment(vector <pair <int_t, bool> >&);
    int_t count_distance(int_t, int_t);
    int_t count_score(vector <pair <int_t, bool> >&);
    public:
        void add_edge(int_t, int_t);
        Graph(int kk): k(kk){}
};

#endif //GRAPH_H

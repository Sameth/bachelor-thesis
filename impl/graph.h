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
    vector <map <int_t, int> > edges;
    vector <vector <int_t> > edges_for_euler;
    vector <vector <int_t> > reverse_edges;
    int number_of_vertices = 0;
    int k;
    void search(int_t, vector <bool>&, vector <pair<int_t, bool> >&);
    void connect_components();
    void construct_edges_for_euler();
    void random_assignment(vector <pair <int_t, bool> >&);
    int_t count_distance(int_t, int_t);
    int_t count_score(vector <pair <int_t, bool> >&);
    void euler_recursive(int_t, vector <int_t>&);
    public:
        void add_edge(int_t, int_t);
        Graph(int kk): k(kk){}
        vector <int_t> euler_path();
};

#endif //GRAPH_H

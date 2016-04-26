#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <set>
#include "common.h"

using namespace std;

class Graph {
    vector <int_t> label_decompress;
    vector <vector <pair <int_t, int> > > edges_for_euler;
    vector <int> result_counts;
    int_t number_of_vertices = 0;
    int k;
    void connect_components();
    void construct_edges_for_euler();
    void random_assignment(vector <pair <int_t, bool> >&);
    int_t count_distance(int_t, int_t);
    int_t count_score(vector <pair <int_t, bool> >&);
    void euler_recursive(int_t, vector <int_t>&, int);
    public:
        void add_edge(int_t, int_t, vector <map <int_t, int> >&, map<int_t, int_t>&);
        Graph(int kk): k(kk){}
        vector <int_t> euler_path();
        vector <int> path_counts();
        void load_edges(const string&);
};

#endif //GRAPH_H

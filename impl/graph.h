#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <set>
#include "common.h"

using namespace std;

class Graph {
    vector <int_t> label_decompress;
    vector <vector <int> > edges_for_euler;
    // Primitive edges
    vector <int> edge_count;
    vector <int> edge_dest;
    // Composite edges
    vector <vector <int> > contained_edges;

    vector <int> result_edges;
    int_t number_of_vertices = 0;
    int primitive_edges = 0;
    int k;
    void connect_components();
    void construct_edges_for_euler();
    void random_assignment(vector <pair <int_t, bool> >&);
    int_t count_distance(int_t, int_t);
    int_t count_score(vector <pair <int_t, bool> >&);
    void euler_recursive(int_t, vector <int>&, int);
    int find_edge(int, int);
    void remove_edge(int, int);
    int edge_destination(int);
    vector <int> list_edges(int);
    public:
        void add_edge(int_t, int_t, vector <map <int_t, int> >&, map<int_t, int_t>&);
        Graph(int kk): k(kk){}
        vector <int_t> euler_path();
        vector <int> path_counts();
        void load_edges(const string&);
        void adjoin_edges(const string&);
};

#endif //GRAPH_H

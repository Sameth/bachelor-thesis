#include "graph.h"

using namespace std;

void Graph::add_edge(int_t from, int_t to) {
    if (label_compress.count(from) == 0) {

        // Add vertex
        label_compress [from] = number_of_vertices;
        label_decompress.push_back(from);
        edges.push_back(vector <int_t>());
        number_of_vertices ++;
    }
    if (label_compress.count(to) == 0) {

        // Add vertex
        label_compress [to] = number_of_vertices;
        label_decompress.push_back(from);
        edges.push_back(vector <int_t>());
        number_of_vertices ++;
    }

    // Add edge
    edges [label_compress [from]].push_back(label_compress [to]);
}

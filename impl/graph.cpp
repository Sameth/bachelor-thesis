#include "graph.h"

#ifndef RETRIES
#define RETRIES 1000
#endif

using namespace std;

void Graph::add_edge(int_t from, int_t to) {
    if (label_compress.count(from) == 0) {

        // Add vertex
        label_compress [from] = number_of_vertices;
        label_decompress.push_back(from);
        edges.push_back(set <int_t>());
        number_of_vertices ++;
    }
    if (label_compress.count(to) == 0) {

        // Add vertex
        label_compress [to] = number_of_vertices;
        label_decompress.push_back(from);
        edges.push_back(set <int_t>());
        number_of_vertices ++;
    }

    // Add edge
    edges [label_compress [from]].insert(label_compress [to]);
}

void Graph::construct_edges_for_euler() {

    // First, collapse set into a vector (we might need duplicates).
    edges_for_euler.resize(number_of_vertices);
    for (int i = 0; i < number_of_vertices; i++) {
        for (int_v v : edges [i]) {
            edges_for_euler[i].push_back(v);
        }
    }

    // Next, count bad vertices (outdegree != indegree);
    // second value is true iff outdegree > indegree 
    vector <pair <int_t, bool> > bad_vertices;
    vector <int> degreecount (number_of_vertices, 0);
    for (int i = 0; i < number_of_vertices; i++) {
        degreecount [i] += edges_for_euler [i].size();
        for (int_v v : edges_for_euler [i]) {
            degreecount [v] --;
        }
    }

    for (int i = 0; i < number_of_vertices; i++) {
        if (degreecount[i] > 0) {
            for(int j = 0; j < degreecount [i]; j++) {
                bad_vertices.push_back(i, true);
            }
        }
        else {
            for (int j = degreecount [i]; j < 0; j++) {
                bad_vertices.push_back(j, false);
            }
        }
    }
    
    
}

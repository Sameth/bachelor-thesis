#include "graph.h"
#include <algorithm>
#include <ctime>
#include <unistd.h>
#include <cassert>

#ifndef RETRIES
#define RETRIES 1000
#endif

#ifndef VERTICES_THRESHOLD
#define VERTICES_THRESHOLD 200
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

/**
 * Count the length of oriented edge from v1 to v2.
 */
int_t Graph::count_distance(int_t v1, int_t v2) {
    int_t label1 = label_decompress [v1], label2 = label_decompress [v2];
    int_t scope = (1 << (2*k - 2)) - 1;

    for (int i = 0; i < k+3; i++) {
        if (label1 == label2) return i;
        scope >>= 2;
        label1 &= scope;
        label2 >>= 2;
    }
    assert(false);
}

int_t Graph::count_score(vector <pair <int_t, bool> >& assignment) {
    int_t score = 0;
    unsigned int false_ = 0, true_ = 0;
    while (false_ < assignment.size()) {
        while (false_ < assignment.size() && assignment [false_].second) false_ ++;
        while (true_ < assignment.size() && !assignment [true_].second) true_ ++;
        if (false_ < assignment.size()) {
            score += this -> count_distance (assignment [false_].first, assignment [true_].first);
        }
    }
    return score;
}

void Graph::random_assignment(vector <pair <int_t, bool> >& bad_vertices) {
    vector <pair <int_t, bool> > best = bad_vertices;
    int_t score = this -> count_score (best);
    srand (time(NULL) + getpid());

    for (int i = 0; i < RETRIES; i++) {
        random_shuffle (bad_vertices.begin(), bad_vertices.end());
        int_t newscore = this -> count_score (bad_vertices);
        if (newscore < score) {
            score = newscore;
            best = bad_vertices;
        }
    }
    
    unsigned int false_ = 0, true_ = 0;
    while (false_ < best.size()) {
        while (false_ < best.size() && best [false_].second) false_ ++;
        while (true_ < best.size() && !best [true_].second) true_ ++;
        if (false_ < best.size()) {
            this -> edges_for_euler [best [false_].first].push_back(best [true_].first);
        }
    }
}

void Graph::construct_edges_for_euler() {

    // First, collapse set into a vector (we might need duplicates).
    this -> edges_for_euler.resize(number_of_vertices);
    for (int i = 0; i < number_of_vertices; i++) {
        for (int_t v : edges [i]) {
            this -> edges_for_euler[i].push_back(v);
        }
    }

    // Next, count bad vertices (outdegree != indegree);
    // second value is true iff outdegree > indegree
    // In other terms, second value is true iff we need to add inbound edges to the vertex
    vector <pair <int_t, bool> > bad_vertices;
    vector <int> degreecount (number_of_vertices, 0);
    for (int i = 0; i < number_of_vertices; i++) {
        degreecount [i] += this -> edges_for_euler [i].size();
        for (int_t v : this -> edges_for_euler [i]) {
            degreecount [v] --;
        }
    }

    for (int i = 0; i < number_of_vertices; i++) {
        if (degreecount[i] > 0) {
            for(int j = 0; j < degreecount [i]; j++) {
                bad_vertices.push_back(make_pair(i, true));
            }
        }
        else {
            for (int j = degreecount [i]; j < 0; j++) {
                bad_vertices.push_back(make_pair(j, false));
            }
        }
    }
    
    // Add edges so that the graph is eulerian.
    this->random_assignment(bad_vertices);
}

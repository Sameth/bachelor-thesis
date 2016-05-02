#include "graph.h"
#include <algorithm>
#include <ctime>
#include <unistd.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <stack>

#ifndef RETRIES
#define RETRIES 100
#endif

#ifndef VERTICES_THRESHOLD
#define VERTICES_THRESHOLD 200
#endif

using namespace std;


map<char, int_t> base_to_int = {{'A',0}, {'C',1}, {'G',2}, {'T',3}};


vector <int_t> encode (string& s, int k) {
    vector <int_t> result;
    if ((int) s.size() < k) return result;
    int_t roll = 0;
    for (int i = 0; i < k - 1; i++) {
        roll <<= 2;
        roll += base_to_int [s [i]];
    }
    for (int i = k - 1; i < (int) s.size(); i++) {
        roll <<= 2;
        roll += base_to_int [s [i]];
        roll &= (1LL << (2*k)) - 1;
        result.push_back(roll);
    }
    return result;
}


void Graph::add_edge(int_t from, int_t to, vector <map <int_t, int> >& edges, map<int_t, int_t>& label_compress) {
    if (label_compress.count(from) == 0) {

        // Add vertex
        label_compress [from] = number_of_vertices;
        label_decompress.push_back(from);
        edges.push_back(map <int_t, int>());
        number_of_vertices ++;
    }
    if (label_compress.count(to) == 0) {

        // Add vertex
        label_compress [to] = number_of_vertices;
        label_decompress.push_back(to);
        edges.push_back(map <int_t, int>());
        number_of_vertices ++;
    }

    // Add edge
    edges [label_compress [from]] [label_compress [to]] ++;
}

void Graph::load_edges(const string& fasta_file) {
    vector <map <int_t, int> > edges;
    map <int_t, int_t> label_compress;
    cerr << "opening " + fasta_file << endl;
    ifstream file_in(fasta_file, ifstream::in);
    string seq;
    int_t n = 0;
    while(file_in >> seq) {
        n ++;
        file_in >> seq;
        vector <int_t> encoded = encode(seq , k - 1);
        for (int j = 0; j < (int) encoded.size() - 1; j++) {
            add_edge(encoded [j], encoded [j + 1], edges, label_compress);
        }
    }
    cerr << "read " << n << " reads" << endl;
    file_in.close();
    this -> edges_for_euler.clear();
    this -> edges_for_euler.resize(number_of_vertices);
    for (int_t i = 0; i < number_of_vertices; i++) {
        for (auto v : edges [i]) {
            this -> edges_for_euler[i].push_back(make_pair(v.first, v.second));
        }
    }
}

/**
 * Count the length of oriented edge from v1 to v2.
 */
int_t Graph::count_distance(int_t v1, int_t v2) {
    int_t label1 = label_decompress [v1], label2 = label_decompress [v2];
    int_t scope = (1LL << (2*k - 4)) - 1;
    label1 &= scope;
    label2 >>= 2;

    for (int i = 1; i < k+3; i++) {
        if (label1 == label2) return i;
        scope >>= 2;
        label1 &= scope;
        label2 >>= 2;
    }
    assert(false);
    return -1;
}

/**
 * Count the price of adding edges from first "false" vertex to
 * first "true", second to second, etc. etc.
 */
int_t Graph::count_score(vector <pair <int_t, bool> >& assignment) {
    int_t score = 0;
    unsigned int false_ = 0, true_ = 0;
//    if (count_if(assignment.begin(), assignment.end(), is_true) != count_if(assignment.begin(), assignment.end(), is_false)) cout << "WAT!\n";
    while (false_ < assignment.size()) {
        while (false_ < assignment.size() && assignment [false_].second) false_ ++;
        while (true_ < assignment.size() && !assignment [true_].second) true_ ++;
        if (false_ < assignment.size()) {
            score += this -> count_distance (assignment [false_].first, assignment [true_].first);
            false_ ++;
            true_ ++;
        }
    }
    return score;
}

/**
 * Tries RETRIES random assignments and picks the best one.
 * Adds picked edges to the graph, to make it eulerian.
 */
void Graph::random_assignment(vector <pair <int_t, bool> >& bad_vertices) {
    cerr << "begin asignment" << endl;
    if (bad_vertices.empty()) return;
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
            this -> edges_for_euler [best [false_].first].push_back(make_pair(best [true_].first, 0));
            false_ ++;
            true_ ++;
        }
    }
}

/**
 * Depth-first search using both orientations of edges. Stores vertices with outdegree != indegree in unsatisfied.
 */
/*
void Graph::search(int_t v, vector <bool>& visited, vector <pair <int_t, bool> >& unsatisfied) {
    visited [v] = true;
    if (edges_for_euler [v].size() != reverse_edges [v].size()) unsatisfied.push_back(make_pair(v, edges_for_euler [v].size() > reverse_edges [v].size()));
    for (pair<int_t, int> edge : edges_for_euler [v]) {
        if (!visited [edge.first]) {
            search(edge.first, visited, unsatisfied);
        }
    }
    for (int_t new_v : reverse_edges [v]) {
        if (!visited [new_v]) {
            search(new_v, visited, unsatisfied);
        }
    }
} */

// TODO: Do something more intelligent (like: match vertex with outdegree > indegree
// with a vertex with outdegree < indegree if possible)
void Graph::connect_components() {
    cerr << "begin connecting" << endl;

    // Some necessary structures
//    vector <pair <int_t, bool> > unsatisfied_vertices;
    vector <bool> visited(number_of_vertices, false);
    srand (time(NULL) + getpid());
    vector <vector <int_t> > reverse_edges(number_of_vertices);

    // Construct reverse edges for the dfs
    for (int_t i = 0; i < number_of_vertices; i++) {
        for(pair <int_t, int> edge : edges_for_euler [i]) {
            reverse_edges [edge.first].push_back(i);
        }
    }

    int_t last_begin = -1;
    for (int_t i = 0; i < number_of_vertices; i++) {
        if (!visited [i]) {
//            vector <pair <int_t, bool> > new_unsatisfied_vertices;

            stack<int_t> buffer;
            visited [i] = true;
            buffer.push(i);
            
            // DFS
            while (!buffer.empty()) {
                int_t v = buffer.top();
                buffer.pop();

                // Normal edges
                for (pair <int_t, int> edge : edges_for_euler [v]) {
                    if (!visited [edge.first]) {
                        visited [edge.first] = true;
                        buffer.push(edge.first);
                    }
                }

                // Reverse edges
                for (int_t new_v : reverse_edges [v]) {
                    if (!visited [new_v]) {
                        visited [new_v] = true;
                        buffer.push(new_v);
                    }
                }
            }
            
            // Connect
            if (last_begin != -1ull) edges_for_euler [last_begin].push_back(make_pair(i, 0));
            last_begin = i;
        }
    }
}

void Graph::construct_edges_for_euler() {

    // First, collapse set into a vector (we might need duplicates).

    // Connect all the components (in unoriented sense)
    this -> connect_components();

    // Next, count bad vertices (outdegree != indegree);
    // second value is true iff outdegree > indegree
    // In other terms, second value is true iff we need to add inbound edges to the vertex
    vector <pair <int_t, bool> > bad_vertices;
    vector <int> degreecount (number_of_vertices, 0);
    for (int_t i = 0; i < number_of_vertices; i++) {
        degreecount [i] += this -> edges_for_euler [i].size();
        for (pair<int_t, int> edge : this -> edges_for_euler [i]) {
            degreecount [edge.first] --;
        }
    }

    for (int_t i = 0; i < number_of_vertices; i++) {
        if (degreecount[i] > 0) {
            for(int j = 0; j < degreecount [i]; j++) {
                bad_vertices.push_back(make_pair(i, true));
            }
        }
        else {
            for (int j = degreecount [i]; j < 0; j++) {
                bad_vertices.push_back(make_pair(i, false));
            }
        }
    }
    
    cerr << "assignment problem size: " << bad_vertices.size() << endl;
    // Add edges so that the graph is eulerian.
    this->random_assignment(bad_vertices);
}

vector <int> Graph::path_counts() {
    return result_counts;
}

void Graph::euler_recursive(int_t v, vector <int_t>& result, int previous_count) {
    while (!edges_for_euler[v].empty()) {
        pair<int_t, int> next = edges_for_euler [v].back();
        edges_for_euler[v].pop_back();
        euler_recursive(next.first, result, next.second);
    }
    result.push_back(v);
    this->result_counts.push_back(previous_count);
}

vector <int_t> Graph::euler_path() {
    this -> construct_edges_for_euler();
    vector <int_t> result;

    cerr << "begin recursive euler" << endl;
    euler_recursive(0, result, -1);
    reverse(result.begin(), result.end());
    vector <int_t> decompressed_result;

    for (int_t v : result) {
        decompressed_result.push_back(label_decompress [v]);
    }
    cerr << "finish\n";
    return decompressed_result;
}


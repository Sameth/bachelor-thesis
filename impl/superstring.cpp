#include <bits/stdc++.h>
#include "common.h"
#include "graph.h"
#include <sdsl/suffix_arrays.hpp>

using namespace std;
using namespace sdsl;

map<int_t, char> int_to_base = {{0,'A'}, {1,'C'}, {2,'G'}, {3,'T'}};

string decode_two(int_t v1, int_t v2, int k, int count, vector <int>& string_counts) {
    int_t cv1 = v1, cv2 = v2;
    int_t add = 1, scope = (1LL << (2*k - 4)) - 1;
    cv1 &= scope;
    cv2 >>= 2;
    while (cv1 != cv2) {
        add ++;
        cv2 >>= 2;
        scope >>= 2;
        cv1 &= scope;
    }

    cv2 = v2;
    string result;
    for (int i = 0; i < add; i++) {
        result.push_back (int_to_base [cv2 & 3]);
        cv2 >>= 2;
        string_counts.push_back(count);
    }
    reverse(result.begin(), result.end());
    return result;
}

string decode(vector <int_t> superstring, int k, vector <int> path_counts, vector <int>& string_counts) {
    string result;
    string_counts.clear();
    int_t label1 = superstring [0], label2;
    for (int i = 0; i < k - 1; i++) {
        result.push_back(int_to_base [label1 & 3]);
        string_counts.push_back(1);
        label1 >>= 2;
    }
    reverse(result.begin(), result.end());

    for (int i = 0; i < (int) superstring.size() - 1; i++) {
        result += decode_two(superstring [i], superstring [i+1], k, path_counts[i+1], string_counts);
    }
    return result;
}

string construct_superstring(int k, char fasta_file[], vector <int>& string_counts) {
    Graph g(k);
    g.load_edges(fasta_file);

    vector <int_t> result_ints = g.euler_path();
    vector <int> result_counts = g.path_counts();

    return decode(result_ints, k, result_counts, string_counts);
}

int main (const int argc, char* argv[]) {
    string usage = string(argv [0]) + " <k> <fasta_file>";
    if (argc < 3) {
        cout << usage << endl;
        return 1;
    }
    int k = atol(argv [1]);
    vector <int> string_counts;
    string superstring = construct_superstring(k, argv[2], string_counts);
    csa_wt<> fm_index;
    construct_im(fm_index, superstring, 1);
    cout << size_in_mega_bytes(fm_index) << endl;

}

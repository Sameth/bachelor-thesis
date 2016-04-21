#include <bits/stdc++.h>
#include "common.h"
#include "graph.h"
#include <sdsl/suffix_arrays.hpp>

using namespace std;
using namespace sdsl;

map<char, int_t> base_to_int = {{'A',0}, {'C',1}, {'G',2}, {'T',3}};
map<int_t, char> int_to_base = {{0,'A'}, {1,'C'}, {2,'G'}, {3,'T'}};

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
        roll &= (1 << (2*k)) - 1;
        result.push_back(roll);
    }
    return result;
}

string decode_two(int_t v1, int_t v2, int k) {
    int_t cv1 = v1, cv2 = v2;
    int add = 1, scope = (1 << (2*k - 4)) - 1;
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
    }
    reverse(result.begin(), result.end());
    return result;
}

string decode(vector <int_t> superstring, int k, vector <int> path_counts, vector <int>& string_counts) {
    string result;
    int_t label1 = superstring [0], label2;
    for (int i = 0; i < k - 1; i++) {
        result.push_back(int_to_base [label1 & 3]);
        label1 >>= 2;
    }
    reverse(result.begin(), result.end());

    for (int i = 0; i < (int) superstring.size() - 1; i++) {
        result += decode_two(superstring [i], superstring [i+1], k);
    }
    return result;
}

int main () {
    int n, k;
    scanf("%d %d", &n, &k);
    Graph g(k);
    vector <string> seq(n);
    for (int i = 0; i < n; i++) {
        cin >> seq[i];
        vector <int_t> encoded = encode(seq [i], k - 1);
        for (int j = 0; j < (int) encoded.size() - 1; j++) {
            g.add_edge(encoded [j], encoded [j + 1]);
        }
    }

    vector <int_t> result_ints = g.euler_path();
    vector <int> result_counts = g.path_counts();
/*    for (int i = 0; i < result_ints.size(); i++) {
        cout << result_ints [i] << ' ';
    }
    cout << endl;
    cout << decode(result_ints, k) << endl; */
    string result = decode(result_ints, k);
    cout << result << endl;
    
    csa_wt<> fm_index;
    construct_im(fm_index, result, 1);

            
}

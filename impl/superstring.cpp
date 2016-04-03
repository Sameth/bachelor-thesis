#include <bits/stdc++.h>
#include "common.h"
#include "graph.h"

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
    for (int i = k; i < (int) s.size(); i++) {
        roll <<= 2;
        roll += base_to_int [s [i]];
        roll &= (1 << (2*k)) - 1;
        result.push_back(roll);
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
}

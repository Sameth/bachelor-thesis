#include "sr-index.hpp"

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
        string_counts.push_back(count + 1);
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
        string_counts.push_back(2);
        label1 >>= 2;
    }
    reverse(result.begin(), result.end());

    for (int i = 0; i < (int) superstring.size() - 1; i++) {
        result += decode_two(superstring [i], superstring [i+1], k, path_counts[i+1], string_counts);
    }
    return result;
}

void SR_index::construct_superstring(const string& fasta_file) {
    Graph g(this -> k);
    g.load_edges(fasta_file);

    vector <int_t> result_ints = g.euler_path();
    vector <int> result_counts = g.path_counts();
    vector <int> string_counts;
    
    cerr << "Construct FM-index\n";
    construct_im(this -> fm_index, decode(result_ints, this -> k, result_counts, string_counts), 1);
    this -> counts = vlc_vector<>(string_counts);
}

void SR_index::construct(const string& fasta_file) {
    construct_superstring(fasta_file);
    cerr << "start processing intervals" << endl;
    
    ifstream file_in(fasta_file, ifstream::in);
    string seq;
    vector <int> intervals;
    vector <bool> starts;
    int k = this -> k;
    while (file_in >> seq) {
        file_in >> seq;
        vector <int> positions;
        for (int i = 0; i <= (int) seq.size() - k; i++) {
            auto occs = locate(fm_index, seq.substr(i, k));
            for (int pos : occs) {
                if (counts [pos + k - 1] > 1) {
                    positions.push_back(pos);
                    break;
                }
                assert(false);
            }
        }
        sort (positions.begin(), positions.end());
        positions.resize((int) (unique(positions.begin(), positions.end()) - positions.begin()));
        intervals.push_back(positions [0]);
        intervals.push_back(positions [0]);
        starts.push_back(true);
        starts.push_back(false);
        for (int i = 1; i < positions.size(); i++) {
            if (intervals.back() + 1 == positions [i]) intervals.back()++;
            else {
                intervals.push_back(positions [i]);
                intervals.push_back(positions [i]);
                starts.push_back(false);
                starts.push_back(false);
            }
        }
    }

    file_in.close();

    for (int i = 0; i < intervals.size(); i++) {
        intervals [i] += k - 1;
    }
    
    cerr << "intervals vlc: " << size_in_mega_bytes(vlc_vector<>(intervals)) << endl;
    bit_vector starts_b(starts.size());
    for (unsigned int i = 0; i < starts.size(); i++) starts_b [i] = starts [i];
    cerr << "starts sd_vector: " << size_in_mega_bytes(sd_vector<>(starts_b)) << endl;
}

int main (const int argc, char* argv[]) {
    string usage = string(argv [0]) + " <k> <fasta_file>";
    if (argc < 3) {
        cout << usage << endl;
        return 1;
    }
    struct rlimit stack_limit;
    getrlimit(RLIMIT_STACK, &stack_limit);
    stack_limit.rlim_cur = RLIM_INFINITY;
    if (setrlimit(RLIMIT_STACK, &stack_limit) != 0) {
        cerr << "Unlimiting stack size failed, might crash.\n";
    }
    int k = atol(argv [1]);
    boost::filesystem::path orig_file = boost::filesystem::path(argv [2]);
    SR_index index(k);
    index.construct(orig_file.string());

}

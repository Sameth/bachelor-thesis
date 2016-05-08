#include "sr-index.hpp"

using namespace std;
using namespace sdsl;

map<int_t, char> int_to_base = {{0,'A'}, {1,'C'}, {2,'G'}, {3,'T'}};

string decode_two(int_t v1, int_t v2, int k, int count, vector <int>& string_counts) {
    int_t cv1 = v1, cv2 = v2;
    int_t add = 1, scope = (1LL << (2*k - 4)) - 1;
    cv1 &= scope;
    cv2 >>= 2;
    vector <int> magic;
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
    g.adjoin_edges(fasta_file);

    vector <int_t> result_ints = g.euler_path();
    vector <int> result_counts = g.path_counts();
    vector <int> string_counts;
    
    cerr << "Construct FM-index\n";
    construct_im(this -> fm_index, decode(result_ints, this -> k, result_counts, string_counts), 1);
    
    this -> counts = vlc_vector<>(string_counts);
    cerr << fm_index.size() << ' ' << counts.size() << endl;
    cerr << "FM index size in mb: " << size_in_mega_bytes(fm_index) << endl;
    cerr << "counts vector size in mb: " << size_in_mega_bytes(counts) << endl;
/*    cerr << "superstring: " << extract(fm_index, 0, fm_index.size() - 1) << endl;
    for (int v : string_counts) {
        cerr << v << ' ';
    }
    cerr << endl; */
}

void SR_index::construct(const string& fasta_file) {
    construct_superstring(fasta_file);
    cerr << "start processing intervals" << endl;
    
    ifstream file_in(fasta_file, ifstream::in);
    string seq;
    vector <int> start_indices;
    vector <bool> starts, valid_positions;
    int k = this -> k;
    int max_read = 100;
    while (file_in >> seq) {
        file_in >> seq;
        max_read = max(max_read, (int) seq.size());
        vector <pair <int, int> > positions;
        for (int i = 0; i <= (int) seq.size() - k; i++) {
            auto occs = locate(fm_index, seq.substr(i, k));
            bool found = false;
            for (int pos : occs) {
                if (counts [pos + k - 1] > 1) {
                    positions.push_back(make_pair(pos - i, i));
                    found = true;
                    break;
                }
            }
            if (!found) {
                cerr << "error" << ' ' << seq.substr(i, k) << endl;
                for (int pos : occs) {
                    cerr << pos << endl;
                }
                exit(1);
            }
        }
        vector <bool> validinread(max_read + 1, false); 
        sort (positions.begin(), positions.end());
        starts.push_back(true);
        start_indices.push_back(positions[0].first);
        for (int i = positions [0].second; i < positions [0].second + k; i++) validinread [i] = true;
        for (int i = 1; i < positions.size(); i++) {
            if (start_indices.back() != positions [i].first) {
                start_indices.push_back(positions [i].first);
                starts.push_back(false);
                bool last = false;
                for (int i = 0; i <= max_read; i++) {
                    if (validinread [i] != last) valid_positions.push_back(true);
                    else valid_positions.push_back(false);
                    last = validinread [i];
                    validinread [i] = false;
                }
            }
            for (int j = positions [i].second; j < positions [i].second + k; j++) validinread [j] = true;
        }
    }

    for (int i = 0; i < start_indices.size(); i++) {
        start_indices [i] += max_read;
    }

    file_in.close();

    cerr << "start_intervals number of elements: " << start_indices.size() << endl;
    cerr << "start_intervals vlc: " << size_in_mega_bytes(vlc_vector<>(start_indices)) << endl;
    bit_vector starts_b(starts.size()), valid_positions_b(valid_positions.size());
    for (unsigned int i = 0; i < starts.size(); i++) starts_b [i] = starts [i];
    for (unsigned int i = 0; i < valid_positions.size(); i++) valid_positions_b [i] = valid_positions [i];
    cerr << "starts sd_vector: " << size_in_mega_bytes(sd_vector<>(starts_b)) << endl;
    cerr << "valid in read sd_vector: " << size_in_mega_bytes(sd_vector<>(valid_positions_b)) << endl;
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

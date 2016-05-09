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
    while (file_in >> seq) {
        file_in >> seq;
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
        vector <bool> validinread(max_read_length + 1, false); 
        sort (positions.begin(), positions.end());
        starts.push_back(true);
        start_indices.push_back(positions[0].first);
        for (int i = positions [0].second; i < positions [0].second + k; i++) validinread [i] = true;
        for (int i = 1; i < positions.size(); i++) {
            if (start_indices.back() != positions [i].first) {
                start_indices.push_back(positions [i].first);
                starts.push_back(false);
                bool last = false;
                for (int j = 0; j <= max_read_length; j++) {
                    if (validinread [j] != last) valid_positions.push_back(true);
                    else valid_positions.push_back(false);
                    last = validinread [j];
                    validinread [j] = false;
                }
            }
            for (int j = positions [i].second; j < positions [i].second + k; j++) validinread [j] = true;
        }
    }

    vector <int> start_indices_permutation(start_indices.size());
    for (int i = 0; i < start_indices.size(); i++) {
        start_indices [i] += max_read_length;
        start_indices_permutation [i] = i;
    }
    sort (start_indices_permutation.begin(), start_indices_permutation.end(), [&start_indices](int i1, int i2) {return start_indices [i1] < start_indices [i2];});

    for (auto e : start_indices) {
        cerr << e << ' ';
    }
    cerr << endl;

    for (auto e : start_indices_permutation) {
        cerr << e << ' ';
    }
    cerr << endl;

    file_in.close();
    cerr << "start_indices number of elements: " << start_indices.size() << endl;
    this -> start_indices = vlc_vector<>(start_indices);
    cerr << "start_indices vlc: " << size_in_mega_bytes(this -> start_indices) << endl;
    this -> start_indices_permutation = vlc_vector<>(start_indices_permutation);
    cerr << "start_indices_permutation vlc " << size_in_mega_bytes(this->start_indices_permutation) << endl;
    bit_vector starts_b(starts.size()), valid_positions_b(valid_positions.size());
    for (unsigned int i = 0; i < starts.size(); i++) starts_b [i] = starts [i];
    for (unsigned int i = 0; i < valid_positions.size(); i++) valid_positions_b [i] = valid_positions [i];
    this -> new_read_start = sd_vector<>(starts_b);
    this -> read_start_rank = sd_vector<>::rank_1_type(&(this -> new_read_start));
    cerr << "starts sd_vector: " << size_in_mega_bytes(this -> new_read_start) << endl;
    this -> valid_in_read = sd_vector<>(valid_positions_b);
    this -> valid_in_read_rank = sd_vector<>::rank_1_type(&(this -> valid_in_read));
    cerr << "valid in read sd_vector: " << size_in_mega_bytes(this -> valid_in_read) << endl;
}

vector<int> SR_index::find_reads(const string& query) {
    if (query.size() > k) {
        cerr << "Query longer than k\n";
        exit(1);
    }
    int the_position = -1;
    vector <int> result;
    for (auto pos : locate(fm_index, query)) {
        if (counts [pos + query.size() - 1] > 1) {
            the_position = pos;
            break;
        }
    }

    if (the_position == -1) {
        cerr << "not in superstring\n";
        return result;
    }

    int lower = -1, upper = start_indices_permutation.size();
    while (upper - lower > 1) {
        int middle = (upper + lower) / 2;
        if (start_indices[start_indices_permutation[middle]]  < the_position + query.size() - 1) lower = middle;
        else upper = middle;
    }
    cerr << upper << endl;
    int current = upper;
    set <int> results;
    while (current < start_indices_permutation.size() && start_indices[start_indices_permutation[current]] - max_read_length <= the_position) {
        int curstart = start_indices [start_indices_permutation[current]] - max_read_length;
        int valid_in_read_offset = (max_read_length + 1) * start_indices_permutation [current];
        cerr << start_indices_permutation [current] << ' ' << start_indices [start_indices_permutation [current]] << endl;

        if ((valid_in_read_rank(valid_in_read_offset + the_position - curstart + 1) % 2) == 0 && (valid_in_read_rank(valid_in_read_offset + the_position - curstart + query.size() + 1) == valid_in_read_rank(valid_in_read_offset + the_position - curstart + 1))) {
            results.insert(read_start_rank(start_indices_permutation [current] + 1));
        }

        current ++;
    }

    for (auto x : results) {
        result.push_back(x);
    }
    return result;

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
    SR_index index(k, 100);
    index.construct(orig_file.string());
    string query;
    while (cin >> query) {
        for (auto x : index.find_reads(query)) {
            cout << x << ' ';
        }
        cout << endl;
    }
}

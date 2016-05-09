#ifndef SR_INDEX__
#define SR_INDEX__

#include <algorithm>
#include "common.h"
#include "graph.h"
#include "util.hpp"
#include <sdsl/suffix_arrays.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sys/resource.h>

class SR_index {
    private:
        int k, max_read_length;
        sdsl::csa_wt<> fm_index;
        sdsl::vlc_vector<> counts;
        sdsl::vlc_vector<> start_indices;
        sdsl::vlc_vector<> start_indices_permutation;
        sdsl::sd_vector<> new_read_start;
        sdsl::sd_vector<>::rank_1_type read_start_rank;
        sdsl::sd_vector<> valid_in_read;
        sdsl::sd_vector<>::rank_1_type valid_in_read_rank;

    public:
        void construct(const string&);
        void construct_superstring(const string&);
        SR_index(int kk, int max_read): k(kk), max_read_length(max_read){}
        vector <int> find_reads(const string&, bool);
};

#endif //SR_INDEX__

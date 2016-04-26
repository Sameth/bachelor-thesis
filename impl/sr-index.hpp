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
        int k;
        sdsl::csa_wt<> fm_index;
        sdsl::vlc_vector<> counts;
        sdsl::dac_vector<> position_intervals;
        sdsl::sd_vector<> read_starts;
        sdsl::sd_vector<>::select_1_type read_start_select;
    public:
        void construct(const string&);
        void construct_superstring(const string&);
        SR_index(int kk): k(kk) {}
};

#endif //SR_INDEX__

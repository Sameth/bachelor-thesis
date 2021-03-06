#include "util.hpp"

using namespace std;

    // TODO: modify this when taking into account reads with uNknowns etc. (R, ... )
    bool check_read(const string& r) {
        for (size_t i = 0; i < r.size(); i++) {
            if ((r[i] != 'A') &&
                    (r[i] != 'C') &&
                    (r[i] != 'T') &&
                    (r[i] != 'G')) {
                return false;
            }
        }

        return true;
    }

/*    // TODO: modify this when taking into account reads with uNknowns etc. (R, ... )
    bool check_contig(const string& c) {
        for (size_t i = 0; i < c.size(); i++) {
            if ((c[i] != 'A') &&
                    (c[i] != 'C') &&
                    (c[i] != 'T') &&
                    (c[i] != 'G')) {
                return false;
            }
        }

        return true;
    }

    string rev_compl(const string& s) {
        string rc = "";
        for (size_t i = 0; i < s.length(); i++) {
            switch (s[i]) {
            case 'A':
                rc += 'T';
                break;
            case 'T':
                rc += 'A';
                break;
            case 'C':
                rc += 'G';
                break;
            case 'G':
                rc += 'C';
                break;
            default:
                rc += s[i];
                break;
            }
        }
        reverse(rc.begin(), rc.end());
        return rc;
    }

    string load_contigs(const string& contigs_path) {
        string retval = "";
        ifstream f(contigs_path);

        if (!f) {
            throw runtime_error("Error opening file '" + contigs_path + "'");
        }

        string l = "";
        int i = 0;
        int skipped = 0;
        while (getline(f, l)) {
            if (i++ % 2 != 1) {
                continue;
            }
            boost::algorithm::trim(l);
            if (check_contig(l)) {
                retval += l;
            } else {
                skipped++;
            }
        }

        f.close();

        return retval;
    }*/

    string execute_command(const string &command) {
        cerr << "go " << command << endl;
        string output;
        const redi::pstreams::pmode mode = redi::pstreams::pstdout |
                redi::pstreams::pstderr;

        redi::ipstream ips(command, mode);
        string l;
        while (getline(ips, l)) {
            output += "  " + l + '\n';
        }
        ips.close();
        int exit_code = ips.rdbuf()->status();
        if (exit_code != 0) {
            throw runtime_error("Error while executing '" + command +
                    "'. Exit code: " + to_string(exit_code));
        }

        return output;
    }

    boost::filesystem::path create_tmpdir() {
        //boost::filesystem::path tmpdir = boost::filesystem::temp_directory_path();
        boost::filesystem::path tmpdir = boost::filesystem::current_path();
        srand(time(NULL));
        tmpdir /= to_string(rand() % 1000000);

        if (!boost::filesystem::create_directory(tmpdir)) {
            throw runtime_error("failed to create temporary directory");
        }

        return tmpdir;
    }

    void check_path_existence(boost::filesystem::path path) {
        if (!boost::filesystem::exists(path)) {
            throw runtime_error("Error. File '" + path.string() + "' not found");
        }
    }


#include "parse_opts.h"
#include <iostream>
using namespace std;

Options *Global_Opts;

void print_usage() {
    cerr << "Usage: comp4133 [-b | -n | -v] [-tf <tf function>] " << endl;
    cerr << "                [-idf <idf function>] [-nf <nf function> <slope>]" << endl;
    cerr << "                queryfile" << endl;
    cerr << "Options:" << endl;
    cerr << endl;
    cerr << "Select retrieval model" << endl;
    cerr << "   -b        Boolean model" << endl;
    cerr << "   -n        Boolean model(NLP) - Require file produced by NLP model" << endl;
    cerr << "   -v        Vector space model(VSM)" << endl;
    cerr << endl;
    cerr << "Select tf function (for VSM)" << endl;
    cerr << "   -tf 1     Binary" << endl;
    cerr << "   -tf 2     Raw frequency" << endl;
    cerr << "   -tf 3     Log normalization" << endl;
    cerr << "   -tf 4     Double normalization 0.5" << endl;
    cerr << "   -tf 5     Double normalization 0.4 0.6 (default)" << endl;
    cerr << endl;
    cerr << "Select idf function (for VSM)" << endl;
    cerr << "   -idf 1     Unary" << endl;
    cerr << "   -idf 2     Inverse document frequency" << endl;
    cerr << "   -idf 3     Inverse document frequency smooth" << endl;
    cerr << "   -idf 4     Inverse document frequency max" << endl;
    cerr << "   -idf 5     probabilistic inverse document frequency" << endl;
    cerr << endl;
    cerr << "Select normalization function (for VSM)" << endl;
    cerr << "   -nf 1            Cosine Similarity - vector lengths (default)" << endl;
    cerr << "   -nf 2 <slope>    Pivoted cosine normalization" << endl;
    cerr << "   -nf 3 <slope>    Pivoted unique normalization" << endl;
    cerr << endl;
}

struct Options* parse_opts(int argc, char** argv) {
    Options opts;
    string token;
    // ./comp4133 -b queryT
    if (argc<3)
        return 0;

    // default values
    opts.tf_function = 2;
    opts.idf_function = 2;
    opts.nf_function = 1;

    // Retrieval model [-b | -v | -n]
    token = string(argv[1]);
    if(token=="-b") {
        opts.retrieval_model = Options::RetrievalModel::Boolean;
    } else if (token=="-n") {
        opts.retrieval_model = Options::RetrievalModel::Boolean_NLP;
    } else if (token=="-v") {
        opts.retrieval_model = Options::RetrievalModel::VSM;
    } else {
        return 0;
    }

    for (int i = 2; i < argc-1; i++) {
        token = string(argv[i]);
        if(token == "-tf") {
            opts.tf_function = stoi(string(argv[i+1]));
            i++;
            continue;
        } else if(token == "-idf") {
            opts.idf_function = stoi(string(argv[i+1]));
            i++;
            continue;
        } else if(token == "-nf") {
            opts.nf_function = stoi(string(argv[i+1]));
            if(opts.nf_function == 2) {
                // Get slope for pivoted algorithm
                opts.pivoted_slope = stod(string(argv[i+2]));
                i++;
            } else if(opts.nf_function == 3) {
                // Get slope for pivoted algorithm
                opts.pivoted_slope = stod(string(argv[i+2]));
                i++;
                //Override tf,idf settings
                opts.tf_function = 5;
                opts.idf_function = 1;
            }
            i++;
            continue;
        } else {
            return 0;
        }
    }

    opts.query_file = argv[argc-1];
    return new Options(opts);
}

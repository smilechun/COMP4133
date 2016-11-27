#include "parse_opts.h"
#include <iostream>
using namespace std;

void print_usage() {
    cerr << "Usage: comp4133 [-b | -n | -v] [-tf <tf function>] " << endl;
    cerr << "                [-idf <idf function>] queryfile" << endl;
    cerr << "Options:" << endl;
    cerr << endl;
    cerr << "Select retrieval model" << endl;
    cerr << "   -b    Boolean model" << endl;
    cerr << "   -n    Boolean model(NLP)" << endl;
    cerr << "   -v    Vector space model(VSM)" << endl;
    cerr << endl;
    cerr << "Select tf function (for VSM)" << endl;
    cerr << "   1     Binary" << endl;
    cerr << "   2     Raw frequency" << endl;
    cerr << "   3     Log normalization" << endl;
    cerr << "   4     double normalization 0.5" << endl;
    cerr << "   5     double normalization 0.4 0.6 (default)" << endl;
    cerr << endl;
    cerr << "Select idf function (for VSM)" << endl;
    cerr << "   1     Unary" << endl;
    cerr << "   2     Inverse document frequency" << endl;
    cerr << "   3     Inverse document frequency smooth" << endl;
    cerr << "   4     Inverse document frequency max" << endl;
    cerr << "   5     probabilistic inverse document frequency" << endl;
}

struct Options* parse_opts(int argc, char** argv) {
    Options opts;
    string token;
    // ./comp4133 -b queryT
    if (argc<3)
        return 0;

    // default values
    opts.tf_function = 5;
    opts.idf_function = 2;

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
        } else {
            return 0;
        }
    }

    opts.query_file = argv[argc-1];
    return new Options(opts);
}

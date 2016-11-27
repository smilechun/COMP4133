#ifndef PARSE_OPTS
#define PARSE_OPTS
#include <string>
using namespace std;

void print_usage();


struct Options {
enum RetrievalModel {
    Boolean,
    Boolean_NLP,
    VSM
};
    RetrievalModel retrieval_model;
    int tf_function;
    int idf_function;
    string query_file;
};

struct Options* parse_opts(int argc, char** argv);
#endif

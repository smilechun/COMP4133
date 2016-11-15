#ifndef QUERY_SET
#define QUERY_SET

#include <vector>
#include <string>
#include <utility>
#include <functional>
#include "InvFile.h"

using namespace std;
using Query = pair<int, string>;

class QuerySet {
public:
    QuerySet(string filename);
    template <class fn>
    void IterateQueries(fn func);
private:
    int long_query;
    vector<Query> queries;
};

vector<string> split(string str, char c = ' ');
static inline std::string &rtrim(std::string &s);

class BooleanModel {
public:
    BooleanModel(InvFile *invFile);
    void operator() (Query q);
private:
    InvFile *invFile;
};

template<class fn>
void QuerySet::IterateQueries(fn func) {
    for(auto i: queries) {
        func(i);
    }
}
#endif

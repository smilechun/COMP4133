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
    void IterateQueries(function<void(Query)> fn);
private:
    int long_query;
    vector<Query> queries;
};

vector<string> split(string str, char c = ' ');
static inline std::string &rtrim(std::string &s);

// A functor
class BooleanModel {
public:
    BooleanModel(InvFile *invFile);
    void operator() (Query q);
private:
    InvFile *invFile;
};

class VSM {
public:
    VSM(InvFile *invFile);
    void operator() (Query q);
private:
    InvFile *invFile;
};

class BooleanModelEnhanced {
public:
    BooleanModelEnhanced(InvFile *invfile);
    void operator() (Query q);
private:
    InvFile *invFile;
};

#endif

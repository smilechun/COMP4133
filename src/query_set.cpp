#include "query_set.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

int LONG_QUERY_THRESHOLD = 50;

QuerySet::QuerySet(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        abort();
    }
    cerr << "Prasing queries." << endl;
    string line;
    while(getline(file, line)) {
        //docnO 0 1
        size_t space1 = line.find(" ");
        int queryID = stoi(line.substr(0, space1));
        string query = line.substr(space1+1, line.length()-space1-1);
        queries.push_back(pair<int, string>(queryID, query));
    }

    // Judge whether the query is long or short
    long long avg_line_width = 0;
    for (auto i: queries) {
        avg_line_width += i.second.length();
    }
    avg_line_width /= queries.size();
    if (avg_line_width<=LONG_QUERY_THRESHOLD) {
        long_query = 0;
    } else {
        long_query = 1;
    }
    //cerr << "!!QUERYQUERY " << long_query << endl;
}

void QuerySet::IterateQueries(function<void(Query)> fn) {
    cerr << "Processing queries." << endl;
    for(auto i: queries) {
        fn(i);
    }
}

static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                                std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
}

vector<string> split(string mystr, char c)
{
    vector<string> result;
    const char *str = mystr.c_str();

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        if(begin!=str)
            result.push_back(string(begin, str));
    } while (0 != *str++);
    return result;
}

//===================================
// BooleanModel
//===================================
BooleanModel::BooleanModel(InvFile *invFile, int longQuery): invFile(invFile), longQuery(longQuery) {
}

void BooleanModel::operator() (Query q) {
    vector<string> tokens = split(rtrim(q.second));
    stringstream ss;
    for(int i=0; i<tokens.size()-1; i++) {
        if(longQuery)
            ss << tokens[i] << " | ";
        else
            ss << tokens[i] << " & ";
    }
    ss << tokens[tokens.size()-1];

    invFile->RetrievalBoolean(ss.str()).Print(q.first);
    //cerr << ss.str() << endl;
}

//===================================
// VSM
//===================================
VSM::VSM(InvFile *invFile): invFile(invFile) {
}

void VSM::operator() (Query q) {
    invFile->RetrievalVSM(q.second).PrintVSM(q.first);
}

//===================================
// VSM
//===================================
BooleanModelEnhanced::BooleanModelEnhanced(InvFile *invFile): invFile(invFile) {
}

void BooleanModelEnhanced::operator() (Query q) {
    vector<string> tokens = split(rtrim(q.second));
    stringstream ss;
    for(int i=0; i<tokens.size()-1; i++) {
        ss << tokens[i] << " ";
    }
    ss << tokens[tokens.size()-1];

    invFile->RetrievalBoolean(ss.str()).Print(q.first);
}

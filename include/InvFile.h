#ifndef INV_FILE
#define INV_FILE
#include <string>
#include <unordered_map>
#include <list>
#include <utility>
#include <map>
#include <vector>
#include "tools.h"

using namespace std;
typedef int DocID;
typedef double Score;

using Post = pair<int, int>;
using HNode = list<Post>;

class RetrievalDoc {
public:
    DocID docID;
    Score score;
};

class RetrievalResult {
public:
    void Add(DocID docID, Score score);
    void Print();
private:
    vector<RetrievalDoc> result;
};

class InvFile {
public:
    InvFile();
    ~InvFile();
    
    // Init functions
    void Build(string filename);
    void Add(string s, DocID docid);

    // Retrieval functions
    size_t GetDF(string stem_word);
    RetrievalResult RetrievalBoolean(vector<string> query);
    
    // Debug
    void DebugPrint();
private:
    using _InvFile = unordered_map<string, HNode>;
    _InvFile inv_file;
    unsigned int doc_count;

    HNode* RetrievalList(string stem_word);
};

#endif

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
typedef int Offset;
typedef double Score;

using Posts = list<Offset>;
using HNode = map<DocID, Posts>;

class RetrievalDoc {
public:
    DocID docID;
    Score score;
};

class RetrievalResult {
public:
    void Add(DocID docID, Score score);
    size_t Size();
    void Print(int queryID);
    void Union(RetrievalResult r2);
    void Intersect(RetrievalResult r2);
    void Complement(RetrievalResult r2);
private:
    map<DocID, RetrievalDoc> result;
};

class InvFile {
public:
    InvFile();
    ~InvFile();
    
    // Init functions
    void Build(string filename);
	void Add(string stem_word, DocID docid, Offset offset);

    // Retrieval functions
    size_t GetDF(string stem_word);
    double GetIDF(string stem_word);
    RetrievalResult RetrievalBoolean(string query);
    RetrievalResult RetrieveExist(string query);
    
    // Debug
    void DebugPrint();
private:
    using _InvFile = unordered_map<string, HNode>;
    _InvFile inv_file;
    unsigned int doc_count;

    HNode* RetrievalList(string stem_word);
};

#endif

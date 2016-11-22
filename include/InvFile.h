#ifndef INV_FILE
#define INV_FILE
#include <string>
#include <unordered_map>
#include <list>
#include <utility>
#include <map>
#include <vector>
#include <functional>
#include "tools.h"
#include "document.h"

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
    void PrintVSM(int queryID);
    //Boolean model
    void Union(RetrievalResult r2);
    void Intersect(RetrievalResult r2);
    void Complement(RetrievalResult r2);

    // VSM model
    void JoinVSM(RetrievalResult r2);
    void NormalizeVSM(DocumentList *docList);

private:
    map<DocID, RetrievalDoc> result;
};

typedef function<double(int raw_freq, int max_freq)> CAL_TF;
typedef function<double(int N, int df, int max_df)> CAL_IDF;

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

    // Vector space query
    RetrievalResult RetrievalVSM(string query);
    
    // Cal tf-idf
    void BuildDocument(CAL_TF, CAL_IDF);

    // Debug
    void DebugPrint();
private:
    using _InvFile = unordered_map<string, HNode>;
    _InvFile inv_file;
    unsigned int doc_count;

    HNode* RetrievalList(string stem_word);
    //VSM
    DocumentList *docList;
    CAL_TF cal_tf;
    CAL_IDF cal_idf;
};

// TF functions
double TF_binary(int raw_freq, int max_freq);
double TF_raw_freq(int raw_freq, int max_freq);
double TF_log_norm(int raw_freq, int max_freq);
double TF_double_norm(int raw_freq, int max_freq);
double TF_double_norm_K(int raw_freq, int max_freq);

// IDF functions
double IDF_unary(int N, int df, int max_df);
double IDF_idf(int N, int df, int max_df);
double IDF_idf_smooth(int N, int df, int max_df);
double IDF_idf_max(int N, int df, int max_df);
double IDF_prob_idf(int N, int df, int max_df);
#endif

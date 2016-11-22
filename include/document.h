#ifndef DOCUMENT
#define DOCUMENT
#include <vector>
using namespace std;

class Document {
public:
    int max_tf;
    int sum_tf;
    int max_df;
    int unique_terms;
    double doc_len;
    double cumulative_weight_sqrt;
};

class DocumentList {
public:
    DocumentList(int docCount);
    // Individual document
    void DocAddTF(int docID, int tf);
    void DocAddDF(int docID, int df);
    void DocAddWeight(int docID, double weight);
    int DocGetMaxTF(int docID);
    int DocGetMaxDF(int docID);
    double DocGetLen(int docID);

    // Avg values
    void cal_avg();
    double get_avg_doc_len();
    double get_avg_unique_term();

private:
    DocumentList();
    double avg_doc_normalization;
    vector<Document> documents;

    double avg_doc_len;
    double avg_unique_term;
};

#endif

#include "document.h"

#include <cmath>
#include <iostream>
using namespace std;

DocumentList::DocumentList(int docCount): documents(docCount) {
    avg_doc_len = 0;
    avg_unique_term = 0;
}
void DocumentList::printDEBUG() {
    for(int i=0; i<documents.size(); i++) {
        cerr << "DOC: " << i << endl;
        cerr << "maxtf:" << documents[i].max_tf << endl;
        cerr << "sumtf:" << documents[i].sum_tf << endl;
        cerr << "maxtf:" << documents[i].max_tf << endl;
        cerr << "unique_terms:" << documents[i].unique_terms << endl;
        cerr << "doc_len:" << documents[i].doc_len << endl;
        cerr << "cumulative_weight_sqrt:" << documents[i].cumulative_weight_sqrt<< endl;
    }
}

void DocumentList::DocAddTF(int docID, int tf) {
    if( tf > documents[docID].max_tf )
        documents[docID].max_tf = tf;
    documents[docID].sum_tf += tf;
    documents[docID].unique_terms++;
}

void DocumentList::DocAddDF(int docID, int df) {
    if( df > documents[docID].max_df )
        documents[docID].max_df = df;
}

void DocumentList::DocAddWeight(int docID, double weight) {
    documents[docID].cumulative_weight_sqrt += weight*weight;
}

int DocumentList::DocGetMaxTF(int docID) {
    return documents[docID].max_tf;
}

int DocumentList::DocGetMaxDF(int docID) {
    return documents[docID].max_df;
}

double DocumentList::DocGetLen(int docID) {
    return documents[docID].doc_len;
}

double DocumentList::DocGetUniqTerms(int docID) {
    return documents[docID].unique_terms;
}

#include <iostream>
void DocumentList::cal_avg() {
    int count = 0;
    avg_doc_len = 0;
    avg_unique_term = 0;
    for(auto &i: documents) {
        i.doc_len = sqrt(i.cumulative_weight_sqrt);
        //Avg
        avg_unique_term += i.cumulative_weight_sqrt;
        avg_doc_len += i.doc_len;
    }
    avg_unique_term /= documents.size();
    avg_doc_len /= documents.size();
}

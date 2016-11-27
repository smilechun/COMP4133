#include "InvFile.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stack>
#include "boolean.h"
#include <cstdlib>
#include <cmath>
#include <sstream>

#define HASH_INIT_SIZE 330000

void RetrievalResult::Add(DocID docID, Score score) {
    RetrievalDoc doc;
    doc.docID = docID;
    doc.score = score;
    result[docID] = doc;
}
size_t RetrievalResult::Size() {
    return result.size();
}
void RetrievalResult::Print(int queryID) {
    int count = 0;
    for(auto i: result) {
        cout << queryID << " 0 " << Global_Tools->get_trec_map(i.first) << " "
             << count << " " << 1000 - count << " HKPU-1" << endl;
        count++;
        if(count>=1000)
            break;
    }
}
bool VSM_cmp (RetrievalDoc d1, RetrievalDoc d2) {
    return d1.score>d2.score;
}
void RetrievalResult::PrintVSM(int queryID) {
    // Sort result
    vector<RetrievalDoc> docs;
    for (auto i: result) {
        docs.push_back(i.second);
    }
    sort(docs.begin(), docs.end(), VSM_cmp);

    // Print result
    int count = 0;
    for(auto i: docs) {
        cout << queryID << " 0 " << Global_Tools->get_trec_map(i.docID) << " "
             << count << " " << i.score << " HKPU-1" << endl;
        count++;
        if(count>=1000)
            break;
    }
}
void RetrievalResult::Union(RetrievalResult r2) {
    result.insert(r2.result.begin(), r2.result.end());
}
void RetrievalResult::Intersect(RetrievalResult r2) {
    map<DocID, RetrievalDoc>::const_iterator left = result.begin();
    map<DocID, RetrievalDoc>::const_iterator right = r2.result.begin();
    while(left!=result.end() && right!=r2.result.end()) {
        if(left->first < right->first) {
            left = result.erase(left);
        } else if(left->first > right->first) {
            right++;
        } else {
            left++;
            right++;
        }
    }
    while(left!=result.end()) {
        left = result.erase(left);
    }
}

void RetrievalResult::Complement(RetrievalResult r2) {
    map<DocID, RetrievalDoc>::const_iterator left = result.begin();
    map<DocID, RetrievalDoc>::const_iterator right = r2.result.begin();
    while(left!=result.end() && right!=r2.result.end()) {
        if(left->first < right->first) {
            left++;
        } else if(left->first > right->first) {
            right++;
        } else {
            left = result.erase(left);
            right++;
        }
    }
}

void RetrievalResult::JoinVSM(RetrievalResult r2) {
    for(auto i: r2.result) {
        auto got = result.find(i.first);
        if (got==result.end()) {
            result.insert(i);
        } else {
            got->second.score += i.second.score;
        }
    }
}

void RetrievalResult::NormalizeVSM(DocumentList *docList) {
    for(auto &i: result) {
        //i.second.score /= docList->DocGetLen(i.first);

        /*
        double s = 0.75;
        i.second.score /= (1-s)+(s)*(docList->DocGetLen(i.first)/docList->avg_doc_len);
        */

        /*
        double s = 0.85;
        i.second.score /= (1-s)+(s)*(docList->DocGetLen(i.first)/docList->avg_doc_len);
        */


        double s = 0.4;
        i.second.score /= (1-s)+(s)*(docList->DocGetUniqTerms(i.first));
    }
}

InvFile::InvFile(): inv_file(HASH_INIT_SIZE) {
    doc_count = 0;
    docList = 0;
}

InvFile::~InvFile() {
}

void InvFile::BuildDocument(CAL_TF cal_tf, CAL_IDF cal_idf) {
    if (docList != 0)
        delete docList; 
    cerr << "Build document length" << endl;
    docList = new DocumentList(doc_count);
    this->cal_tf = cal_tf;
    this->cal_idf = cal_idf;
    // ===================================
    //  Step 1:
    //  Build max_tf, sum_tf, unique_terms
    // ===================================
    // Loop through all hnodes
    for(auto i = inv_file.cbegin(); i != inv_file.cend(); i++) {
        auto hnode = i->second;
        // Loop through all post in a hnode
        for(auto j = hnode.cbegin(); j != hnode.cend(); j++) {
            // j->first = docID
            // j->second = term freq
            docList->DocAddTF(j->first, j->second.size());
            docList->DocAddDF(j->first, hnode.size());
        }
    }
    // ===================================
    //  Step 2:
    //  Build document length
    // ===================================
    // Loop through all hnodes
    for(auto i = inv_file.cbegin(); i != inv_file.cend(); i++) {
        auto hnode = i->second;
        // Loop through all post in a hnode
        for(auto j = hnode.cbegin(); j != hnode.cend(); j++) {
            // j->first = docID
            // j->second = term freq
            double tf = cal_tf(j->second.size(), docList->DocGetMaxTF(j->first));
            double idf = cal_idf(doc_count, hnode.size(), docList->DocGetMaxDF(j->first));
            double weight = tf*idf;
            //cout << "WEIGHT" << tf << ":" << idf << ":" << weight << endl;
            docList->DocAddWeight(j->first, weight);
        }
    }
    docList->cal_avg();
}

int InvFile::Build(string filename) {
    int count = 0;
    DocID docID;
	Offset offset;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return -1;
    }
    cerr << "Build inverse file." << endl;
    string line;
    while(getline(file, line)) {
        //docnO 0 1
        size_t space1 = line.find(" ");
		size_t space2 = line.find(" ", space1 + 1);
        if (space1==string::npos || space2==string::npos || space1==space2)
            cerr << "WTF IS THIS" << docID << endl;
		docID = stoi(line.substr(space1 + 1, space2 - space1));
		offset = stoi(line.substr(space2 + 1));
        Add(line.substr(0, space1), docID, offset);
    }
    doc_count = docID+1;    //docID start from 0
    return 0;
}

void InvFile::Add(string stem_word, DocID docid, Offset offset) {
	auto iter = inv_file.find(stem_word);
	if (iter == inv_file.end()) {
		// insert new word
		HNode hnode;
		hnode[docid] = Posts({ offset });
		inv_file[stem_word] = hnode;
	} else {
        // insert into existing word
        // No need a new hnode, just add a new Post into hnode
		auto posts = iter->second.find(docid);
		if (posts == iter->second.end()) {
			// insert a new document
			Posts p({ offset });
			iter->second.insert(HNode::value_type(docid, p));
		} else {
			// document exists, insert a new offset into list
			posts->second.push_back(offset);
		}
    }
}

size_t InvFile::GetDF(string stem_word) {
    auto iter = inv_file.find(stem_word);
    if (iter == inv_file.end()) {
        return 0;
    } else {
        return iter->second.size();
    }
}

double InvFile::GetIDF(string stem_word) {
    int df = GetDF(stem_word);
    if(df==0)
        return 0;
    return log2(doc_count*1.0/df);
}

void removeCharsFromString( string &str, string charsToRemove ) {
    for ( unsigned int i = 0; i < charsToRemove.length(); ++i ) {
        str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
    }
}
// query = "a & ( b | c)"
RetrievalResult InvFile::RetrievalBoolean(string query) {
    stack<RetrievalResult> mystack;
    Boolean b(query);
    for(auto i: b.GetPostfixExp()) {
        if(!Boolean::IsOperator(i)) {
            string tmp(i);
            removeCharsFromString(tmp, "(),.'\"/`?!><{}*&^%$#@~;:|[]=-_+");
            if(tmp.empty() || Global_Tools->is_stop(tmp)) {
                mystack.push(RetrievalResult()); // empty result for stop words
            } else {
                string temp = Global_Tools->stem(tmp);
                mystack.push(RetrieveExist(temp));
            }
        } else {
            if(mystack.size() < 2) {
                cerr << "Bad Query: " << query << endl;
                return RetrievalResult();
            }
            RetrievalResult r1 = mystack.top(); mystack.pop();
            RetrievalResult r2 = mystack.top(); mystack.pop();
            if(i=="&") {
                r2.Intersect(r1);
            } else if (i=="|") {
                r2.Union(r1);
            } else if (i=="^") {
                r2.Complement(r1);
            }
            mystack.push(r2);
        }
    }
    if(mystack.empty()) {
        cerr << "Bad Query: " << query << endl;
        return RetrievalResult();
    }
    return mystack.top();
}

RetrievalResult InvFile::RetrieveExist(string query) {
    RetrievalResult result;
    //if(query.empty() || Global_Tools->is_stop(query))
        //return result;
    _InvFile::iterator ret = inv_file.find(query);
    if (ret == inv_file.end()) {
        return result;
    } else {
        for(auto i:ret->second) {
            result.Add(i.first, 1);
        }
    }
    return result;
}

RetrievalResult InvFile::RetrievalVSM(string query) {
    istringstream iss(query);
    string token;
    RetrievalResult result;
    while(std::getline(iss, token, ' ')) {
        if(token.empty() || Global_Tools->is_stop(token))
            continue;
        // Find all relevant documents
        token = Global_Tools->stem(token);
        auto got = inv_file.find(token);
        if(got==inv_file.end()) {
            // This term does not exist in any of the docs
            continue;
        }

        // Process the documents that have df > 1 for this token
        RetrievalResult tmp_result;
        HNode hnode = got->second;
        for(auto i: hnode) {
            double idf = cal_idf(doc_count, hnode.size(), docList->DocGetMaxDF(i.first));
            double tf = cal_tf(i.second.size(), docList->DocGetMaxTF(i.first));
            tmp_result.Add(i.first, tf*idf);
        }
        result.JoinVSM(tmp_result);
    }

    // Normalize VSM final score
    result.NormalizeVSM(docList);
    return result;
}

void InvFile::DebugPrint() {
    cout <<"DEBUG" <<inv_file.size() << endl;
    cout <<"DEBUG" <<inv_file.bucket_count() << endl;

    /*
	std::cout << "inv_file buckets contain:\n";
	for ( unsigned i = 0; i < inv_file.bucket_count(); ++i) {
		std::cout << "bucket #" << i << " contains:";
		for ( auto local_it = inv_file.begin(i); local_it!= inv_file.end(i); ++local_it )
			std::cout << " " << local_it->first; // << ":" << local_it->second;
		std::cout << std::endl;
	}
    */
}

HNode* InvFile::RetrievalList(string stem_word) {
    auto iter = inv_file.find(stem_word);
    if (iter != inv_file.end()) {
        return &iter->second;
    };
    return 0;
}

//===============================================
// TF functions
//===============================================
double TF_binary(int raw_freq, int max_freq) {
    return raw_freq > 0;
}

double TF_raw_freq(int raw_freq, int max_freq) {
    return raw_freq;
}

double TF_log_norm(int raw_freq, int max_freq) {
    return 1 + log(raw_freq);
}

double TF_double_norm(int raw_freq, int max_freq) {
    return raw_freq * 1.0 / max_freq;
}

double TF_double_norm_K(int raw_freq, int max_freq) {
    return 0.4 + 0.6 * (1+log(raw_freq))/(1+log(max_freq));
}

double IDF_unary(int N, int df, int max_df) {
    return 1;
}

double IDF_idf(int N, int df, int max_df) {
    return log(N * 1.0 / df);
}

double IDF_idf_smooth(int N, int df, int max_df) {
    return log(1 + df * 1.0 / df);
}

double IDF_idf_max(int N, int df, int max_df) {
    return log(1 + max_df * 1.0 / df);
}

double IDF_prob_idf(int N, int df, int max_df) {
    return log((N * 1.0 - df) / df);
}

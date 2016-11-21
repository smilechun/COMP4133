#include "InvFile.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stack>
#include "boolean.h"
#include <cstdlib>
#include <cmath>

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

InvFile::InvFile(): inv_file(HASH_INIT_SIZE) {
    doc_count = 0;
}

InvFile::~InvFile() {
}

void InvFile::Build(string filename) {
    int count = 0;
    DocID docID;
	Offset offset;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        abort();
    }
    string line;
    while(getline(file, line)) {
        //docnO 0 1
        size_t space1 = line.find(" ");
		size_t space2 = line.find(" ", space1 + 1);
		docID = stoi(line.substr(space1 + 1, space2 - space1));
		offset = stoi(line.substr(space2 + 1));
        Add(line.substr(0, space1), docID, offset);
    }
    doc_count = docID+1;    //docID start from 0
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
			iter->second.insert(posts, HNode::value_type(docid, p));
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

// query = "a & ( b | c)"
RetrievalResult InvFile::RetrievalBoolean(string query) {
    stack<RetrievalResult> mystack;
    Boolean b(query);
    for(auto i: b.GetPostfixExp()) {
        if(!Boolean::IsOperator(i)) {
            string temp = Global_Tools->stem(i);
            mystack.push(RetrieveExist(temp));
        } else {
            RetrievalResult a = mystack.top(); mystack.pop();
            RetrievalResult b = mystack.top(); mystack.pop();
            if(i=="&") {
                b.Intersect(a);
            } else if (i=="|") {
                b.Union(a);
            } else if (i=="^") {
                b.Complement(a);
            }
            mystack.push(b);
        }
    }
    return mystack.top();
}

RetrievalResult InvFile::RetrieveExist(string query) {
    RetrievalResult result;
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

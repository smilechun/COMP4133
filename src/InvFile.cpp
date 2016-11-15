#include "InvFile.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stack>
#include "boolean.h"

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
void RetrievalResult::Print() {
    for(auto i : result) {
        cout << "PRINT:" << i.first << endl;
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
}

InvFile::InvFile(): inv_file(HASH_INIT_SIZE) {
    doc_count = 0;
}

InvFile::~InvFile() {
}

void InvFile::Build(string filename) {
    int _docID = -1;
    ifstream file(filename);
    string line;
    while(getline(file, line)) {
        //docnO 0 1
        size_t space1 = line.find(" ");
        size_t space2 = line.find(" ", space1+1);
        int docID = stoi(line.substr(space1+1, space2-space1));
        if (_docID < docID) {
            _docID++;
            if(!(_docID % 10000))
                cout << _docID << endl;
        }
        Add(line.substr(0, space1), docID);
    }
}

void InvFile::Add(string stem_word, DocID docid) {
	auto iter = inv_file.find(stem_word);
	if (iter == inv_file.end()) {
		// insert new word
		HNode hnode = HNode({Post(docid, 1)});
		inv_file[stem_word] = hnode;
	} else {
        // insert into existing word
        // No need a new hnode, just add a new Post into hnode
        auto last_post = iter->second.rbegin();
        if (last_post->first == docid) {
            // last_post is the same as the current docid
            // then increase the term frequency
            last_post->second++;
        } else {
            // the docid is not the same
            // insert a new post
            iter->second.push_back({Post(docid, 1)});
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

// query = a AND ( b OR c)
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
            if(i=="AND") {
                a.Intersect(b);
            } else if (i=="OR") {
                a.Union(b);
            } else if (i=="BUT") {
                a.Complement(b);
            }
            mystack.push(a);
        }
    }
    /*
    for(const string &word : query) {
        cout << "===================" << endl;
        cout << word << endl;
        RetrievalList(word);
    }
    result.Print();
    */
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

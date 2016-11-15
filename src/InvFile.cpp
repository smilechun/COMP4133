#include "InvFile.h"
#include <iostream>
#include <fstream>
#include <algorithm>

#define HASH_INIT_SIZE 330000

void RetrievalResult::Add(DocID docID, Score score) {
    RetrievalDoc doc;
    doc.docID = docID;
    doc.score = score;
    result.push_back(doc);
}
void RetrievalResult::Print() {
    for(auto i : result) {
        cout << "PRINT:" << i.docID << endl;
    }
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
            if(!(_docID % 1000))
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

// a AND ( b OR c)
RetrievalResult InvFile::RetrievalBoolean(string query) {
    RetrievalResult result;
    /*
    for(const string &word : query) {
        cout << "===================" << endl;
        cout << word << endl;
        RetrievalList(word);
    }
    result.Print();
    */
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

#include "tools.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
class tools *Global_Tools;

// Delete the trailing newline and carriage return of a line
void FilterTrailNewline(char * line) {
	char c;

	c = line[strlen(line)-1];
	if ((c == '\n') || (c == '\r')) {
		line[strlen(line)-1] = '\0';
		c = line[strlen(line)-1];
		if ((c == '\n') || (c == '\r')) line[strlen(line)-1] = '\0';
	}

}
string tools::get_trec_map(int docID) {
    return trec_map[docID];
}

// Change the string into lower case
void LowerStr(char * s) {
	char * p;
	if (s == NULL) return;
	p = s;
	while (*p != '\0') {
		*p = (char) tolower(*p);
		p++;
	}
}

// Stopword list from the given file
void tools::load_stop(const char *f) {
	FILE * fp = fopen(f, "rb");
    if(fp==0) {
        fprintf(stderr, "Unable to open file %s\n", f);
        abort();
    }
	std::string x;
	this->stop_list.clear();
	char line[1000];

	while (fgets(line, 1000, fp) != NULL) {
		FilterTrailNewline(line);
		LowerStr(line);
		x = line;
		stop_list.insert(dic::value_type(x,1));
	}

	fclose(fp);
}

tools::tools(string stop_file, string file_txt) {
	load_stop(stop_file.c_str());
    load_file(file_txt);
}

char easytolower(char in) {
	if(in<='Z' && in>='A')
		return in-('Z'-'z');
	return in;
}

void tools::load_file(string file_txt) {
    ifstream file(file_txt);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << file_txt << endl;
        abort();
    }
    string line;
    while(getline(file, line)) {
        //docnO 0 1
        size_t space1 = line.find(" ");
        size_t at = line.find("@");
        size_t space2 = line.find(" ",at+2);
        //int docID = stoi(line.substr(0, space1));
        string name = line.substr(at+2, space2-at-2);
        trec_map.push_back(name);
    }
}

int tools::is_stop(string word, int ignore_case) {
	if (ignore_case) {
		transform(word.begin(), word.end(), word.begin(), easytolower);
	}
	return stop_list.find(word) != stop_list.end();
}

string tools::stem(string word) {
    string tempword(word);
	stem_proc.Stem(tempword);
    return tempword;
}

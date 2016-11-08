#include "tools.h"
#include <algorithm>

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

tools::tools(string stop_file) {
    load_stop(stop_file.c_str());
}

char easytolower(char in){
    if(in<='Z' && in>='A')
        return in-('Z'-'z');
    return in;
} 

int tools::is_stop(string word, int to_lower) {
    if (to_lower) {
        transform(word.begin(), word.end(), word.begin(), easytolower);
    }
    return stop_list.find(word) != stop_list.end();
}

void tools::stem(string &word) {
    stem_proc.Stem(word);
}

#ifndef BOOLEAN
#define BOOLEAN
#include <string>
using namespace std;
class Boolean {
public:
    Boolean(string infix);
private:
    string infix_str;
    string postfix_str;
    int begin_pos;

    string GetNextToken();
    void InfixToPostfix();
};
#endif

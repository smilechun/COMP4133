#ifndef BOOLEAN
#define BOOLEAN
#include <string>
#include <vector>

using namespace std;
class Boolean {
public:
    Boolean(string infix);
    vector<string> GetPostfixExp();
    string GetPostfixStr();
    // AND OR BUT ( ), only check the first byte
    static int IsOperator(const char c);
    static int IsOperator(string s);
private:
    string infix_str;
    vector<string> postfix;
    int begin_pos;

    string _GetNextToken_infix();
    void InfixToPostfix();
};
#endif

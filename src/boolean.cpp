#include "boolean.h"
#include <stdio.h>
#include <iostream>
#include <stack>
using namespace std;

Boolean::Boolean(string infix) : infix_str(infix),begin_pos(0) {
    cout << infix_str << endl;
    InfixToPostfix();
    cout << postfix_str << endl;
}

int is_operator(char c) {
    return ( c=='A' || c=='O' || c=='B' || c=='(' || c==')' );
}

string Boolean::GetNextToken() {
    const char *p = infix_str.c_str()+begin_pos;
    const char *start = 0;
    int size = 0;
    string ret;
    //Trim front space
    while(*p != '\0') {
        if (*p != ' ') {
            break;
        }
        p++;
        size++;
    }
    if(*p == '\0')
        return "";  //empty result

    start = p;
    if(*p == 'A') {
        size+=3;
        ret = "AND";
    } else if (*p == 'O') {
        size+=2;
        ret = "OR";
    } else if (*p == '(') {
        size+=1;
        ret = "(";
    } else if (*p == ')') {
        size+=1;
        ret = ")";
    } else if (*p == 'B') {
        size+=3;
        ret = "BUT";
    } else {
        //token word
        while(*p != '\0') {
            if (*p == ' ' || is_operator(*p))
                break;
            p++;
            size++;
        }
        ret = string(start, p-start);
    }
    begin_pos += size;
    return ret;
}

void Boolean::InfixToPostfix() {
    string token;
    stack<string> mystack;
    while(!(token = GetNextToken()).empty()) {
        if(!is_operator(token[0])) {
            postfix_str += token + " ";
        } else if(token=="(") {
            mystack.push(token);
        } else if(token==")") {
            while(!mystack.empty() && mystack.top() != "(") {
                postfix_str += mystack.top() + " ";
                mystack.pop();
            }
            mystack.pop();  //pop left parenthesis
        } else {
            //operator: AND OR BUT
            if(mystack.empty() || mystack.top() == "(") {
                mystack.push(token);
            } else {
                while(!mystack.empty() && mystack.top() != "(") {
                    postfix_str += mystack.top() + " ";
                    mystack.pop();
                }
                mystack.push(token);
            }
        }
    }
    while(!mystack.empty()) {
        postfix_str += mystack.top() + " ";
        mystack.pop();
    }
}

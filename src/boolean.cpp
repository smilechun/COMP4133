#include "boolean.h"
#include <stdio.h>
#include <iostream>
#include <stack>
#include <sstream>
using namespace std;

Boolean::Boolean(string infix) : infix_str(infix),begin_pos(0) {
    InfixToPostfix();
}

vector<string> Boolean::GetPostfixExp() {
    return postfix;
}

string Boolean::GetPostfixStr() {
    stringstream ss;
    for(size_t i=0; i<postfix.size(); i++) {
        if(i!=0)
            ss << " ";
        ss << postfix[i];
    }
    return ss.str();
}

int Boolean::IsOperator(const char c) {
    return ( c=='&' || c=='|' || c=='^' || c=='(' || c==')' );
}
int Boolean::IsOperator(string c) {
    return c.size()>0 ? IsOperator(c[0]) : 0;
}

string Boolean::GetNextToken_infix() {
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
    if(IsOperator(*p)) {
        size+=1;
        ret = string(1,*p);
    } else {
        //token word
        while(*p != '\0') {
            if (*p == ' ' || IsOperator(*p))
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
    while(!(token = GetNextToken_infix()).empty()) {
        if(!IsOperator(token)) {
            postfix.push_back(token);
        } else if(token=="(") {
            mystack.push(token);
        } else if(token==")") {
            while(!mystack.empty() && mystack.top() != "(") {
                postfix.push_back(mystack.top());
                mystack.pop();
            }
            mystack.pop();  //pop left parenthesis
        } else {
            //operator: & | ^
            if(mystack.empty() || mystack.top() == "(") {
                mystack.push(token);
            } else {
                while(!mystack.empty() && mystack.top() != "(") {
                    postfix.push_back(mystack.top());
                    mystack.pop();
                }
                mystack.push(token);
            }
        }
    }
    while(!mystack.empty()) {
        postfix.push_back(mystack.top());
        mystack.pop();
    }
}

#include <vector>
#include <string>
#include "parse.h"
#include "core.h"
#include "utils.h"
#include "operator.h"

#include <iostream>

using namespace std;

bool parseName(string buff, int & pos, Token** token, CompileTime *ct) {
    if (!isValidNameStart(buff.at(pos)))
        return false;
    int start = pos;
    pos += 1;
    while (pos < buff.length() && isValidNameExtension(buff.at(pos)))
        pos++;
    string name = buff.substr(start, pos - start);
    int nameId = ct->saveName(name);
    *token = new Token(TokenType::NAME, nameId);
    return true;
}

bool parseOperator(string buff, int & pos, Token** token) {
    char a = buff.at(pos);
    char b = pos + 1 == buff.length()? ' ' : buff.at(pos + 1);
    string s({a, b});

    OperatorType type = OperatorType::EQ;

    pos += 2;
    if (s == "==") type = EQ;
    else if(s == "!=") type = NOT_EQ;
    else if(s == "<=") type = LTE;
    else if(s == ">=") type = GTE;
    else if(s == "**") type = POW;
    else {
        pos -= 1;
        switch(a) {
            case '^':
                type = POW;
                break;
            case '<':
                type = LT;
                break;
            case '>':
                type = GT;
                break;
            case '+':
                type = ADD;
                break;
            case '-':
                type = SUB;
                break;
            case '*':
                type = MUL;
                break;
            case '/':
                type = DIV;
                break;
            case '%':
                type = MOD;
                break;
            default:
                pos -= 1;
                return false;
        }
    }
    
    *token = new Token(OPERATOR, (int)type);
    return true;
}

bool parseSymbol(string buff, int& pos, Token** token) {
    char a = buff.at(pos);
    SymbolType type;

    pos += 1;
    switch(a) {
        case '(':
            type = LP;
            break;
        case ')':
            type = RP;
            break;
        default:
            pos -= 1;
            return false;
    }

    *token = new Token(SYMBOL, (int)type);
    return true;

}

bool parseLiteralInteger(string buff, int& pos, Token** token, CompileTime * ct) {
    int len = 0;
    int value = 0;
    while (pos + len < buff.length() && isDigit(buff.at(pos + len))) {
        value = value * 10 + (buff.at(pos + len) - '0');
        len++;
    }
    if (len == 0)
        return false;

    int integerId = ct->saveLiteralInteger(value);
    *token = new Token(LIT_INTERGER, integerId);
    pos += len;
    return true;
}

vector<Token> parseExpression(string buff, int & pos, CompileTime * ct) {
    vector<Token> result;
    Token* parsed = NULL;

    while(pos < buff.length()) {
        if(buff.at(pos) == ' ') {
            pos++;
            continue;
        }
        if(
            parseName(buff, pos, &parsed, ct) ||
            parseOperator(buff, pos, &parsed) ||
            parseLiteralInteger(buff, pos, &parsed, ct) ||
            parseSymbol(buff, pos, &parsed)
        ) {
            result.push_back(*parsed);
        }
        else {
            cout << "error at pos " << pos << "!" << endl;
            break;
            // error
        }
    }

    if (parsed != NULL)
        delete parsed;
    
    return result;
}


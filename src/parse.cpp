#include <vector>
#include <string>
#include "parse.h"
#include "core.h"
#include "utils.h"
#include "operator.h"
#include <cmath>

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
    else if(s == "++") type = INCREMENT;
    else if(s == "--") type = DECREMENT;
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
        case ',':
            type = COMMA;
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
    INT_NATIVE_TYPE value = 0;
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

bool parseLiteralFloat(string buff, int& pos, Token** token, CompileTime * ct) {
    int len1 = 0;
    int val1 = 0;

    while(pos + len1 < buff.length() && isDigit(buff.at(pos + len1))) {
        val1 = val1 * 10 + (buff.at(pos + len1) - '0');
        len1++;
    }

    if (len1 == 0 || pos + len1 == buff.length() || buff.at(pos + len1) != '.')
        return false;

    int len2 = 0;
    int val2 = 0;

    while(pos + len1 + 1 + len2 < buff.length() && isDigit(buff.at(pos + len1 + 1 + len2))) {
        val2 = val2 * 10 + (buff.at(pos + len1 + 1 + len2) - '0');
        len2++;
    }

    if (len2 == 0)
        return false;

    FLOAT_NATIVE_TYPE dec = 0;

    for (int i = len2; i > 0; i--) {
        dec += val2 % 10;
        dec /= 10;
        val2 /= 10;

    }

    FLOAT_NATIVE_TYPE value = val1 + dec;
    
    int floatId = ct->saveLiteralFloat(value);
    *token = new Token(LIT_FLOAT, floatId);
    pos = pos + len1 + len2 + 1;
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
            parseLiteralFloat(buff, pos, &parsed, ct) ||
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


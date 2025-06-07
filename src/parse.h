#pragma once

#include <string>
#include <vector>
#include "core.h"

using namespace std;


enum SymbolType {
    LP, RP
};

enum TokenType {
    LIT_INTERGER, NAME, SYMBOL,
    OPERATOR
};

struct Token {
    TokenType type;
    int hook;
    Token(TokenType type_, int hook_): type(type_), hook(hook_) {}
};

bool parseName(string buff, int & pos, Token** token, CompileTime *ct);

bool parseOperator(string buff, int & pos, Token** token);

bool parseSymbol(string buff, int& pos, Token** token);

bool parseLiteralInteger(string buff, int& pos, Token** token, CompileTime * ct);

vector<Token> parseExpression(string buff, int & pos, CompileTime * ct);
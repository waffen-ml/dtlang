#pragma once

#include "parse.h"
#include <vector>

using namespace std;

enum ChainNodeType {
    START, TEMP, VAR, TOKEN, P_CONTAINER
};

class ChainNode {
public:
    ChainNodeType type;
    int hook;
    Token* token;
    ChainNode* next;
    ChainNode* inner;

    ChainNode(Token * token_, ChainNode* next_): token(token_), type(TOKEN), next(next_) {}
    ChainNode(ChainNodeType type_, int hook_, ChainNode* next_, ChainNode* inner_): type(type_), hook(hook_), next(next_), inner(inner_) {}

    bool isObject() {
        return type == TEMP || type == VAR;
    }

    void printType();

    void printShallow();
};


ChainNode* makeStart(ChainNode* next);

ChainNode* buildChainFromTokens(vector<Token> & expr);

ChainNode* processChainFolds(ChainNode* node, ChainNode** join);


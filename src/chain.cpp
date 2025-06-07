#include <iostream>
#include "chain.h"

using namespace std;

void ChainNode::printType() {
    switch(type) {
        case TEMP:
            cout << "temp" << endl;
            break;
        case START:
            cout << "start" << endl;
            break;
        case TOKEN:
            cout << "token" << endl;
            break;
        case P_CONTAINER:
            cout << "p-container" << endl;
            break;
        default:
            cout << "other" << endl;
            break;
    }
}

void ChainNode::printShallow() {
    ChainNode* start = makeStart(this);
    ChainNode* p = start;

    cout << "Chain(" << endl;

    while(p->next != NULL) {
        p->next->printType();
        p = p->next;
    }

    cout << ")" << endl;

    delete start;
}



ChainNode* makeStart(ChainNode* next) {
    return new ChainNode(START, 0, next, NULL);
}

ChainNode* processChainFolds(ChainNode* node, ChainNode** join) {
    ChainNode* start = makeStart(node);
    ChainNode* p = start;

    ChainNode* after = NULL;
    ChainNode* inner = NULL;

    while(p->next != NULL) {
        if (p->next->type == TOKEN && p->next->token->type == SYMBOL) {
            switch((SymbolType)p->next->token->hook) {
                case LP:
                    inner = processChainFolds(p->next->next, &after);
                    p->next = new ChainNode(P_CONTAINER, 0, after, inner);
                    break;
                case RP:
                    *join = p->next->next;
                    return start->next;
            }
        }

        p = p->next;
    }
    
    return start->next;
}

ChainNode* buildChainFromTokens(vector<Token> & expr) {
    ChainNode* start = makeStart(NULL);
    ChainNode* p = start;

    for(int i = 0; i < expr.size(); i++) {
        p->next = new ChainNode(&expr[i], NULL);
        p = p->next;
    }

    ChainNode* w = NULL;
    ChainNode* result = processChainFolds(start->next, &w);
    
    delete w;

    return result;
}





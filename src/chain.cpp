#include <iostream>
#include "chain.h"

using namespace std;

void ChainNode::print(ExecuteBundle b) {

    switch(type) {
        case TEMP:
            if (b.rt->temp[hook].type == INTEGER)
                cout << "temp (int) " << b.rt->temp[hook].asInteger() << endl;
            else
                cout << "temp (float) " << b.rt->temp[hook].asFloat() << endl;
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

void ChainNode::printShallow(ExecuteBundle b) {
    ChainNode* p = this;

    cout << "Chain(" << endl;

    while(p != NULL) {
        p->print(b);
        p = p->next;
    }

    cout << ")" << endl;
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
                    p->next = NULL;
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

ChainNode* safeAccess(ChainNode* node, int offset) {
    ChainNode* p = node;

    while(offset > 0 && p != NULL) {
        p = p->next;
        offset--;
    }

    return p;
}



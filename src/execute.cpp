#include "execute.h"
#include "parse.h"
#include "operator.h"
#include "utils.h"
#include "primitives.h"

#include <iostream>
#include <vector>

using namespace std;


Var getVar(ChainNode* node, ExecuteBundle b) {
    if(!node->isObject())
        return Var::null();
    return b.rt->temp[node->hook];
}

vector<Var> parseObjArgs(ChainNode* node, ExecuteBundle b) {
    vector<Var> result;

    ChainNode* p = node;

    if (!p->isObject())
        return {};

    // consume first
    result.push_back(getVar(p, b));
    p = p->next;

    while (p != NULL) {
        if (p->type == TOKEN && p->token->type == SYMBOL && (SymbolType)p->token->hook == COMMA && p->next != NULL && p->next->isObject()) {
            result.push_back(getVar(p->next, b));
            p = p->next->next;
        } else {
            cout << "invalid arguments" << endl;
            return {};
        }
    }

    return result;
}


ChainNode* executeObj(ChainNode* node, ExecuteBundle b) {
    if (node->type == TOKEN && node->token->type == LIT_INTERGER) {
        int tempId = b.ct->nTemp++;
        b.rt->temp[tempId] = {INTEGER, (VAR_DATA_TYPE)b.ct->getLiteralInteger(node->token->hook)};
        return new ChainNode(TEMP, tempId, node->next, NULL);
    }
    else if(node->type == TOKEN && node->token->type == LIT_FLOAT) {
        int tempId = b.ct->nTemp++;
        FLOAT_NATIVE_TYPE val = b.ct->getLiteralFloat(node->token->hook);
        b.rt->temp[tempId] = {FLOAT, bit_cast<VAR_DATA_TYPE, FLOAT_NATIVE_TYPE>(val)};
        return new ChainNode(TEMP, tempId, node->next, NULL);
    }
    else if(node->type == TOKEN && node->token->type == NAME && node->next != NULL && node->next->type == P_CONTAINER) {
        int nameId = node->token->hook;
        ChainNode* argsResult = execute(node->next->inner, b);

        vector<Var> args = parseObjArgs(argsResult, b);

        Var result;

        nameId = 1;

        switch(nameId) {
            case 0:
                result = nativeFuncAbs(args);
                break;
            case 1:
                result = nativeMean(args);
                break;
            default:
                cout << "invalid function nameId: " << nameId << endl;
                break;
        }
        
        int tempId = b.ct->nTemp++;
        b.rt->temp[tempId] = result;

        return new ChainNode(TEMP, tempId, node->next->next, NULL);
    }
    else if(node->type == P_CONTAINER) {
        ChainNode* result = execute(node->inner, b);
        if(result != NULL && result->next == NULL && result->isObject()) {
            result->next = node->next;
            return result;
        }
        node->inner = result;
        return node;
    }

    return node;
}


ChainNode* executeBinOperator(ChainNode* firstOperand, int allowPriority, ExecuteBundle b) {
    ChainNode* op = firstOperand->next;
    ChainNode* secondOperand = op == NULL? NULL : op->next;

    if (op == NULL || secondOperand == NULL
        || !firstOperand->isObject() || !secondOperand->isObject()
        || op->type != TOKEN || op->token->type != OPERATOR
        || !isOperatorBinary((OperatorType)op->token->hook)
        || getOperatorPriority((OperatorType)op->token->hook) < allowPriority
    ) {
        return firstOperand;
    }

    Var& v1 = b.rt->temp[firstOperand->hook];
    Var& v2 = b.rt->temp[secondOperand->hook];

    Var result;

    switch((OperatorType)op->token->hook) {
        case ADD:
            result = addVar(v1, v2);
            break;
        case SUB:
            result = subVar(v1, v2);
            break;
        case MUL:
            result = mulVar(v1, v2);
            break;
        case DIV:
            result = divVar(v1, v2);
            break;
        case MOD:
            break;
        case POW:
            break;
        default:
            break;
    }

    // save as a temp value
    int tempId = b.ct->nTemp++;
    b.rt->temp[tempId] = result;

    return new ChainNode(TEMP, tempId, secondOperand->next, NULL);
}


ChainNode* executeFrontUnaryOperator(ChainNode* before, ExecuteBundle b) {
    ChainNode* op = safeAccess(before, 1);
    ChainNode* obj = safeAccess(before, 2);

    if (
        op == NULL || obj == NULL
        || op->type != TOKEN || op->token->type != OPERATOR || !isOperatorUnary((OperatorType)op->token->hook)
        || !obj->isObject()
        || before->isObject()
    ) {
        return before;
    }

    // op -> obj

    Var& v = b.rt->temp[obj->hook];
    Var result;

    switch((OperatorType)op->token->hook) {
        case SUB:
            result = negateVar(v);
            break;
        case INCREMENT:
            break;
        case DECREMENT:
            break;
    }

    int tempId = b.ct->nTemp++;
    b.rt->temp[tempId] = result;

    before->next = new ChainNode(TEMP, tempId, obj->next, NULL);    

    delete op, obj;

    return before;
}


ChainNode* executeBackUnaryOperator(ChainNode* obj, ExecuteBundle b) {
    ChainNode* op = safeAccess(obj, 1);

    if(
        op == NULL
        || !obj->isObject()
        || op->type != TOKEN || op->token->type != OPERATOR || !isOperatorUnary((OperatorType)op->token->hook)
    ) {
        return obj;
    }

    Var& v = b.rt->temp[obj->hook];
    Var result;

    switch((OperatorType)op->token->hook) {
        case INCREMENT:
            break;
        case DECREMENT:
            break;
    }

    int tempId = b.ct->nTemp++;
    b.rt->temp[tempId] = result;

    ChainNode* r = new ChainNode(TEMP, tempId, op->next, NULL);

    delete obj, op;
    
    return r;
}


ChainNode* execute(ChainNode* node, ExecuteBundle b) {
    ChainNode* start = makeStart(node);
    ChainNode* p = start;

    while(p->next != NULL) {
        p->next = executeObj(p->next, b);
        p = p->next;
    }

    p = start;
    while(p != NULL) {
        executeFrontUnaryOperator(p, b);
        p = p->next;
    }

    p = start;
    while(p->next != NULL) {
        p->next = executeBackUnaryOperator(p->next, b);
        p = p->next;
    }

    for(int l = 3; l >= 0; l--) {
        p = start;

        while(p->next != NULL) {
            p->next = executeBinOperator(p->next, l, b);
            p = p->next;
        }
    }

    return start->next;
}


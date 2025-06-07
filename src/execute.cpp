#include "execute.h"
#include "operator.h"
#include "utils.h"


ChainNode* executeObj(ChainNode* node, ExecuteBundle b) {
    if (node->type == TOKEN) {
        if(node->token->type == LIT_INTERGER) {
            int tempId = b.ct->nTemp++;
            b.rt->temp[tempId] = b.ct->getLiteralInteger(node->token->hook);
            return new ChainNode(TEMP, tempId, node->next, NULL);
        }
    }
    else if(node->type == P_CONTAINER) {
        ChainNode* result = execute(node->inner, b);
        result->next = node->next;
        return result;
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

    int val1 = b.rt->temp[firstOperand->hook];
    int val2 = b.rt->temp[secondOperand->hook];
    int result = 0;

    switch((OperatorType)op->token->hook) {
        case ADD:
            result = val1 + val2;
            break;
        case SUB:
            result = val1 - val2;
            break;
        case MUL:
            result = val1 * val2;
            break;
        case DIV:
            result = val1 / val2;
            break;
        case MOD:
            result = val1 % val2;
            break;
        case POW:
            result = naiveIntegerPow(val1, val2);
            break;
        default:
            break;
    }

    // save as a temp value
    int tempId = b.ct->nTemp++;
    b.rt->temp[tempId] = result;

    return new ChainNode(TEMP, tempId, secondOperand->next, NULL);
}

ChainNode* execute(ChainNode* node, ExecuteBundle b) {
    ChainNode* start = makeStart(node);
    ChainNode* p = start;

    while(p->next != NULL) {
        p->next = executeObj(p->next, b);
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


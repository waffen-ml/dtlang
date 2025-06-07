#include "operator.h"


int getOperatorPriority(OperatorType op) {
    switch(op) {
        case POW:
            return 3;
        case MUL:
        case DIV:
        case MOD:
            return 2;
        case ADD:
        case SUB:
            return 1;
        case LT:
        case LTE:
        case GT:
        case GTE:
        case EQ:
        case NOT_EQ:
            return 0;
        default:
            return -1;
    }
}

bool isOperatorBinary(OperatorType op) {
    return true;
}
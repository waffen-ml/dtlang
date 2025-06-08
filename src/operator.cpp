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
    switch(op) {
        case INCREMENT:
        case DECREMENT:
            return false;
        default:
            return true;
    }
}

bool isOperatorUnary(OperatorType op) {
    switch(op) {
        case SUB:true;
        case INCREMENT:
        case DECREMENT:
            return true;
        default:
            return false;
    }
}
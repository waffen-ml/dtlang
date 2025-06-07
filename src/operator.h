#pragma once


enum OperatorType {
    EQ, NOT_EQ,
    LT, LTE, GT, GTE,
    ADD, SUB, MUL, DIV, MOD, POW
};

int getOperatorPriority(OperatorType op);

bool isOperatorBinary(OperatorType op);


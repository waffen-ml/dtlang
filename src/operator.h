#pragma once


enum OperatorType {
    EQ, NOT_EQ,
    LT, LTE, GT, GTE,
    ADD, SUB, MUL, DIV, MOD, POW,

    INCREMENT, DECREMENT
};

int getOperatorPriority(OperatorType op);

bool isOperatorBinary(OperatorType op);

bool isOperatorUnary(OperatorType op);
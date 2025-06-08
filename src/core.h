#pragma once

#include <vector>
#include <string>
#include "utils.h"

#define FLOAT_NATIVE_TYPE double
#define INT_NATIVE_TYPE long
#define VAR_DATA_TYPE long

using namespace std;


enum VarType {
    INTEGER, FLOAT, NULL_
};

class Var {
public:
    VarType type = NULL_;
    VAR_DATA_TYPE data = 0;

    static Var fromFloat(FLOAT_NATIVE_TYPE val) {
        return {FLOAT, bit_cast<VAR_DATA_TYPE, FLOAT_NATIVE_TYPE>(val)};
    }

    static Var fromInteger(INT_NATIVE_TYPE val) {
        return {INTEGER, bit_cast<VAR_DATA_TYPE, INT_NATIVE_TYPE>(val)};
    }

    static Var null() {
        return {NULL_, 0};
    }

    FLOAT_NATIVE_TYPE asFloat() {
        return bit_cast<FLOAT_NATIVE_TYPE, VAR_DATA_TYPE>(data);
    }

    INT_NATIVE_TYPE asInteger() {
        return bit_cast<INT_NATIVE_TYPE, VAR_DATA_TYPE>(data);
    }
    
}; 

class CompileTime {
private:
    vector<FLOAT_NATIVE_TYPE> literalFloats;
    vector<INT_NATIVE_TYPE> literalIntegers;
    vector<string> names;

public:
    int nTemp = 0;

    int saveLiteralFloat(FLOAT_NATIVE_TYPE v) {
        literalFloats.push_back(v);
        return literalFloats.size() - 1;
    }

    int saveLiteralInteger(INT_NATIVE_TYPE v) {
        literalIntegers.push_back(v);
        return literalIntegers.size() - 1;
    }

    int saveName(string name) {
        names.push_back(name);
        return names.size() - 1;
    }

    INT_NATIVE_TYPE getLiteralInteger(int id) {
        return literalIntegers[id];
    }

    FLOAT_NATIVE_TYPE getLiteralFloat(int id) {
        return literalFloats[id];
    }
};


class RunTime {
public:
    Var temp[100];

};

struct ExecuteBundle {
    CompileTime* ct;
    RunTime* rt;
};




// 
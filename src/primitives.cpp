#include <iostream>
#include <vector>

#include "primitives.h"
#include "utils.h"

using namespace std;

bool castInt(Var& a, Var& b, INT_NATIVE_TYPE& aInt, INT_NATIVE_TYPE& bInt, FLOAT_NATIVE_TYPE& aFloat, FLOAT_NATIVE_TYPE& bFloat) {
    if(a.type == FLOAT || b.type == FLOAT) {
        
        if (a.type == FLOAT)
            aFloat = a.asFloat();
        else
            aFloat = (FLOAT_NATIVE_TYPE)a.asInteger();
        
        if (b.type == FLOAT)
            bFloat = b.asFloat();
        else
            bFloat = (FLOAT_NATIVE_TYPE)b.asInteger();

        return true;
    }
    else {
        aInt = a.asInteger();
        bInt = b.asInteger();

        return false;
    }
}

FLOAT_NATIVE_TYPE forceFloat(Var& a) {
    if (a.type == FLOAT)
        return a.asFloat();
    else
        return (FLOAT_NATIVE_TYPE)a.asInteger();
}


Var addVar(Var& a, Var& b) {
    INT_NATIVE_TYPE aI, bI;
    FLOAT_NATIVE_TYPE aF, bF;

    if(castInt(a, b, aI, bI, aF, bF)) {
        return Var::fromFloat(aF + bF);
    } else {
        return Var::fromInteger(aI + bI);
    }
}

Var subVar(Var& a, Var& b) {
    INT_NATIVE_TYPE aI, bI;
    FLOAT_NATIVE_TYPE aF, bF;

    if(castInt(a, b, aI, bI, aF, bF)) {
        return Var::fromFloat(aF - bF);
    } else {
        return Var::fromInteger(aI - bI);
    }
}

Var mulVar(Var& a, Var& b) {
    INT_NATIVE_TYPE aI, bI;
    FLOAT_NATIVE_TYPE aF, bF;

    if(castInt(a, b, aI, bI, aF, bF)) {
        return Var::fromFloat(aF * bF);
    } else {
        return Var::fromInteger(aI * bI);
    }
}

Var divVar(Var& a, Var& b) {
    float aF = forceFloat(a);
    float bF = forceFloat(b);

    if (bF == 0) {
        // hell no
    }

    return Var::fromFloat(aF / bF);
}

Var negateVar(Var& a) {
    if (a.type == FLOAT)
        return Var::fromFloat(-a.asFloat());
    else
        return Var::fromInteger(-a.asInteger());
}

Var nativeFuncAbs(vector<Var> & args) {
    if (args.size() != 1) {
        cout << "error: abs recieves 1 pos argument" << endl;
        return Var::null();
    }

    Var& a = args[0];

    if (a.type == INTEGER) {
        INT_NATIVE_TYPE val = a.asInteger();
        return Var::fromInteger(val >= 0? val : -val);
    }
    else if(a.type == FLOAT) {
        FLOAT_NATIVE_TYPE val = a.asFloat();
        return Var::fromFloat(val >= 0? val : -val);
    }
    else {
        cout << "error: incompatable type in abs" << endl;
        return Var::null();
    }
}

Var nativeMean(vector<Var> & args) {
    if (args.size() == 0) {
        cout << "error: mean needs at least 1 argument" << endl;
        return Var::null();
    }

    FLOAT_NATIVE_TYPE sum = 0;

    for(int i = 0; i < args.size(); i++)
        sum += forceFloat(args[i]);

    return Var::fromFloat(sum / args.size());
}


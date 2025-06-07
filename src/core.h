#pragma once

#include <vector>
#include <string>

using namespace std;


class CompileTime {
private:
    vector<double> literalDoubles;
    vector<int> literalIntegers;
    vector<string> names;

public:
    int nTemp = 0;

    int saveLiteralDouble(double v) {
        literalDoubles.push_back(v);
        return literalDoubles.size() - 1;
    }

    int saveLiteralInteger(int v) {
        literalIntegers.push_back(v);
        return literalIntegers.size() - 1;
    }

    int saveName(string name) {
        names.push_back(name);
        return names.size() - 1;
    }

    double getLiteralDouble(int id) {
        return literalDoubles[id];
    }

    int getLiteralInteger(int id) {
        return literalIntegers[id];
    }
};


class RunTime {
public:
    int temp[100];

};

struct ExecuteBundle {
    CompileTime* ct;
    RunTime* rt;
};

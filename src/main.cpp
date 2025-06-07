#include <iostream>
#include <vector>
#include "core.h"
#include "chain.h"
#include "parse.h"
#include "execute.h"

using namespace std;

int main() {
    string buff = "((2))"; // 29
    cout << "buffer: " << buff << endl;

    CompileTime ct;
    RunTime rt;
    int pos = 0;
    vector<Token> tokens = parseExpression(buff, pos, &ct);

    ChainNode* root = buildChainFromTokens(tokens);
    
    root->printShallow();
    
    cout <<"----- Executing -----" << endl;

    root = execute(root, {&ct, &rt});

    root->printShallow();

    return 0;
}
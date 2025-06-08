#include <iostream>
#include <vector>
#include "core.h"
#include "chain.h"
#include "parse.h"
#include "execute.h"

using namespace std;

int main() {
    string buff = "10.01"; // 29
    cout << "buffer: " << buff << endl;

    CompileTime ct;
    RunTime rt;

    cout << ct.saveName("abs") << endl;
    cout << ct.saveName("mean") << endl;

    ExecuteBundle b = {&ct, &rt};
    int pos = 0;
    vector<Token> tokens = parseExpression(buff, pos, &ct);

    cout << "parsed " << tokens.size() << " tokens" << endl;


    ChainNode* root = buildChainFromTokens(tokens);
    
    root->printShallow(b);

    cout <<"----- Executing -----" << endl;

    root = execute(root, b);

    root->printShallow(b);

    return 0;
}
#include "core.h"
#include <vector>

using namespace std;

Var addVar(Var& a, Var& b);
Var subVar(Var& a, Var& b);
Var mulVar(Var& a, Var& b);
Var divVar(Var& a, Var& b);
Var negateVar(Var& a) ;
Var nativeFuncAbs(vector<Var> & args) ;
Var nativeMean(vector<Var> & args);



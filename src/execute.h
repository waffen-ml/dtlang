
#include "core.h"
#include "chain.h"


ChainNode* executeObj(ChainNode* node, ExecuteBundle b);

ChainNode* executeBinOperator(ChainNode* firstOperand, int allowPriority, ExecuteBundle b);

ChainNode* execute(ChainNode* node, ExecuteBundle b);

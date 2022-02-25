#include "PTLCState.h"

PTLCState ptl_state;

int PTLCState::AddTree(void)
{
    trees.push_back(new PTL::PropertyTree());
    return trees.size()-1;
}
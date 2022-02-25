#ifndef PTL_C_STATE_H
#define PTL_C_STATE_H

#include "PTL.h"
#include <vector>

class PTLCState
{
    public:
        PTLCState(void){}
        ~PTLCState(void)
        {
            for (auto& p:trees) delete p;
        }
        int AddTree(void);
        PTL::PropertyTree& GetTree(int i) {return *trees[i];}
    private:
        std::vector<PTL::PropertyTree*> trees;
};

extern PTLCState ptl_state;

#endif
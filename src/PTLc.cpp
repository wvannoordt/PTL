#include "PTL.h"
#include "PTLc.h"
#include "PTLCState.h"
#include <string>

std::string get_string(char* buf, int len)
{
    std::string output;
    for (int i = 0; i < len; i++) output += buf[i];
    return output;
}

void PTLC_Create_Tree(int* tree_id_out)
{
    *tree_id_out = ptl_state.AddTree();
}

void PTLC_Tree_Read  (int* tree_id, char* filename, int* len_filename)
{
    std::string fname = get_string(filename, *len_filename);
    auto& tree = ptl_state.GetTree(*tree_id);
    tree.Read(fname);
}
void PTLC_Tree_Parse_Real  (int* tree_id, char* query, int* len_query, double* val_out)
{
    std::string fname = get_string(query, *len_query);
    auto& tree = ptl_state.GetTree(*tree_id);
    double val = tree.Query(fname);
    *val_out = val;
}

void PTLC_Tree_Parse_Int   (int* tree_id, char* query, int* len_query, int* val_out)
{
    std::string fname = get_string(query, *len_query);
    auto& tree = ptl_state.GetTree(*tree_id);
    int val = tree.Query(fname);
    *val_out = val;
}

void PTLC_Tree_Parse_RealVec_Length  (int* tree_id, char* query, int* len_query, int* len_out)
{
    std::string fname = get_string(query, *len_query);
    auto& tree = ptl_state.GetTree(*tree_id);
    std::vector<double> data = tree.Query(fname);
    *len_out = data.size();
}

void PTLC_Tree_Parse_RealVec_Entries (int* tree_id, char* query, int* len_query, double* data_out)
{
    std::string fname = get_string(query, *len_query);
    auto& tree = ptl_state.GetTree(*tree_id);
    std::vector<double> data = tree.Query(fname);
    for (int i = 0; i < data.size(); i++)
    {
        data_out[i] = data[i];
    }
}

void PTLC_Tree_Parse_IntVec_Length  (int* tree_id, char* query, int* len_query, int* len_out)
{
    std::string fname = get_string(query, *len_query);
    auto& tree = ptl_state.GetTree(*tree_id);
    std::vector<int> data = tree.Query(fname);
    *len_out = data.size();
}

void PTLC_Tree_Parse_IntVec_Entries (int* tree_id, char* query, int* len_query, int* data_out)
{
    std::string fname = get_string(query, *len_query);
    auto& tree = ptl_state.GetTree(*tree_id);
    std::vector<int> data = tree.Query(fname);
    for (int i = 0; i < data.size(); i++)
    {
        data_out[i] = data[i];
    }
}
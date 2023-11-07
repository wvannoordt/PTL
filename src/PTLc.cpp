#include "PTL.h"
#include "PTLc.h"
#include "PTLCState.h"
#include <string>

bool crash_on_missing_option = true;
std::string get_string(char* buf, int len)
{
    std::string output;
    for (int i = 0; i < len; i++) output += buf[i];
    return output;
}

void PTLC_Except_On_Missing(int* input)
{
    crash_on_missing_option = bool(*input);
}

void PTLC_Create_Tree(int* tree_id_out)
{
    *tree_id_out = ptl_state.AddTree();
}

void PTLC_Exists(int* tree_id, char* query, int* len_var, int* val_out)
{
    auto& tree = ptl_state.GetTree(*tree_id);
    std::string fname = get_string(query, *len_var);
    *val_out = 0;
    bool found = tree.Has(fname);
    if (found)
    {
        *val_out = 1;
    }
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

void PTLC_Tree_Parse_String   (int* tree_id, char* query, int* len_query, char* val_out, int* val_len)
{
    std::string fname = get_string(query, *len_query);
    auto& tree = ptl_state.GetTree(*tree_id);
    std::string val = tree.Query(fname);
    if (*val_len < val.length())
    {
        ErrorKill("Requested value \"" + fname + "\" attempted to copy into string that is too small (value size: "
            + std::to_string(val.length()) + ", recipient size: " + std::to_string(*val_len) + "). Please resize the recipient variable.");
    }
    else
    {
        for (std::size_t i = 0; i < *val_len; i++) val_out[i] = ' ';
        for (std::size_t i = 0; i < val.length(); i++)
        {
            val_out[i] = val[i];
        }
    }
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
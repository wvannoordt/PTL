#ifndef PTL_GLOBAL_H
#define PTL_GLOBAL_H
#ifdef __cplusplus
extern "C" {
#endif

void PTLC_Create_Tree                (int* tree_id_out);
void PTLC_Tree_Read                  (int* tree_id, char* filename, int* len_filename);
void PTLC_Tree_Parse_Real            (int* tree_id, char* query, int* len_query, double* val_out);
void PTLC_Tree_Parse_String          (int* tree_id, char* query, int* len_query, char* val_out, int* val_len);
void PTLC_Tree_Parse_Int             (int* tree_id, char* query, int* len_query, int* val_out);
void PTLC_Tree_Parse_RealVec_Length  (int* tree_id, char* query, int* len_query, int* len_out);
void PTLC_Tree_Parse_RealVec_Entries (int* tree_id, char* query, int* len_query, double* data_out);
void PTLC_Tree_Parse_IntVec_Length   (int* tree_id, char* query, int* len_query, int* len_out);
void PTLC_Tree_Parse_IntVec_Entries  (int* tree_id, char* query, int* len_query, int* data_out);

#ifdef __cplusplus
}
#endif
#endif
#ifndef PTL_GLOBAL_H
#define PTL_GLOBAL_H
#ifdef __cplusplus
extern "C" {
#endif

void PTLC_Create_Tree      (int* tree_id_out);
void PTLC_Tree_Read        (int* tree_id, char* filename, int* len_filename);
void PTLC_Tree_Parse_Real  (int* tree_id, char* query, int* len_query, double* val_out);
void PTLC_Tree_Parse_Int   (int* tree_id, char* query, int* len_query, int* val_out);

#ifdef __cplusplus
}
#endif
#endif
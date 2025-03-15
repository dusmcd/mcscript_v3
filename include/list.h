#ifndef MCSCRIPT_V3_LIST_H
#define MCSCRIPT_V3_LIST_H

typedef struct Node node_t;

struct Node {
  void* val;
  node_t* next;
  int idx;
}; 

typedef struct {
  node_t* head;
  node_t* tail;
  int size;
} list_t;

list_t* initialize_list();
void push_back(list_t* list, void* val);
node_t* get_list_item(list_t* list, int idx);


#endif // MCSCRIPT_V3_LIST_H
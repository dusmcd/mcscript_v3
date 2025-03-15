#include <list.h>
#include <stdlib.h>
#include <string.h>

list_t* initialize_list() {
  list_t* list = malloc(sizeof(list_t));
  if (list == NULL)
    return NULL;
  
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;  
  
  return list;
}

void push_back(list_t* list, void* val) {
  if (list == NULL || val == NULL)
    return;
  
  node_t* new_node = malloc(sizeof(node_t));
  if (new_node == NULL)
    return;
  
  new_node->val = val;
  new_node->next = NULL;

  if (list->head == NULL) {
    new_node->idx = 0;
    list->head = new_node;
    list->tail = new_node;
  } else {
    new_node->idx = list->tail->idx + 1;
    list->tail->next = new_node;
    list->tail = new_node;
  }

  list->size++;
}

node_t* get_list_item(list_t* list, int idx) {
  node_t* current = list->head->next;
  for (int i = 0; i < list->size; i++) {
    if (i == idx)
      return current;
    
    current = current->next;
  }

  return NULL;
}
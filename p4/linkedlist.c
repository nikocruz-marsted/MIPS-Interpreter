#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

struct linkedlist {
  struct linkedlist_node *first;
  // TODO: define linked list metadata
};

typedef struct linkedlist_node {
  // TODO: define the linked list node
  int key;
  int value;
  struct linkedlist_node *next;
} linkedlist_node_t;

linkedlist_t *ll_init() {
  // TODO: create a new linked list
  linkedlist_t* a = (linkedlist_t*)malloc(sizeof(linkedlist_t));
  a->first = NULL;
  return a;
}

void ll_add(linkedlist_t *list, int key, int value) {
  // TODO: create a new node and add to the front of the linked list if a
  // node with the key does not already exist.
  // Otherwise, replace the existing value with the new value.
  struct linkedlist_node *temp = list->first;
  int exists = 0;
  while(temp != NULL){
    if(temp->key == key){
      temp->value = value;
      exists = 1;
    }
    temp = temp->next;
  }
  if(exists == 0){
    struct linkedlist_node *newNode = (struct linkedlist_node*)malloc(sizeof(struct linkedlist_node));
    newNode->value = value;
    newNode->key = key;
    newNode->next = list->first;
    list->first = newNode;
  }
    
}

int ll_get(linkedlist_t *list, int key) {
  // TODO: go through each node in the linked list and return the value of
  // the node with a matching key.
  // If it does not exist, return 0.
  struct linkedlist_node *temp = list->first;
  while(temp != NULL){
    if(temp->key == key){
      return temp->value;
    }
    temp = temp->next;
  }
  return 0;
}

int ll_size(linkedlist_t *list) {
  // TODO: return the number of nodes in this linked list
  struct linkedlist_node *temp = list->first;
  int counter = 0;
  while(temp != NULL){
    counter += 1;
    temp = temp->next;
  }
  return counter;
}

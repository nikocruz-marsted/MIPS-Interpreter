#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable {
    // TODO: define hashtable struct to use linkedlist as buckets

  int numBuckets;
  linkedlist_t** buffer;//pointer to a pointer of array  holding buckets of linked list

};

/**
 * Hash function to hash a key into the range [0, max_range)
 */
int hash(int key, int max_range) {
    // TODO: feel free to write your own hash function (NOT REQUIRED)
    return key % max_range;
}

hashtable_t *ht_init(int num_buckets) {
    // TODO: create a new hashtable
  
  //linkedlist_t *bArray = (linkedlist_t*)malloc(sizeof(linkedlist_t)*(num_buckets));
  hashtable_t *h = (hashtable_t*)malloc(sizeof(hashtable_t));
  h->buffer = (linkedlist_t**)calloc(num_buckets, sizeof(linkedlist_t*));
  //calloc sets all index of the memory to 0, to determine if it needs to be init ll later
  h->numBuckets = num_buckets;
 
  return h;
  
}

void ht_add(hashtable_t *table, int key, int value) {
    // TODO: create a new mapping from key -> value.
    // If the key already exists, replace the value.
    int hashIndex = hash(key, table->numBuckets);
    if (table->buffer[hashIndex] == 0) {
      // ll on this bucket index does not exist, need to make
      table->buffer[hashIndex] = ll_init();
      ll_add(table->buffer[hashIndex], key, value);


    } else {
      ll_add(table->buffer[hashIndex], key, value);
    }
    

}

int ht_get(hashtable_t *table, int key) {
  // TODO: retrieve the value mapped to the given key.
  // If it does not exist, return 0
  int hashIndex = hash(key, table->numBuckets);
  if(table->buffer[hashIndex] == NULL){
    return 0;
  }
  int index = ll_get(table->buffer[hashIndex], key);  
  return index;
}

int ht_size(hashtable_t *table) {
    // TODO: return the number of mappings in this hashtable
    int total = 0;
    for (int i = 0; i < table->numBuckets; i++){
      if (table->buffer[i] != 0) {
          total += ll_size(table->buffer[i]);
       }
    }
    return total;
}


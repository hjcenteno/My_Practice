#include "hash_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LOAD 0.75
#define FNV_offset_basis 0xcbf29ce484222325
#define FNV_prime 0x100000001b3

void test(){
    printf("hello\n");
}

void createHashMap(
    hashMap *map, int capacity, 
    size_t keySize, size_t valueSize,
    int (*cmp)(const void* a, const void* b)
){
    //initiate default values for the hashmap
    // hashEntry *table = malloc(sizeof(hashEntry) * capacity);
    map->table = calloc(capacity, sizeof(hashEntry));
    if(map->table == NULL){
        fprintf(stderr, "[Error] Something happen with allocating new memory.");
        return;
    }
    map->size = 0; //number of items in the map
    map->capacity = capacity;
    map->sizeOf_key = keySize;
    map->sizeOf_value = valueSize;
    map->cmp = cmp;
}

void deleteHashMap(hashMap* map){
    for(int i = 0; i < map->capacity; i++){
        free(map->table[i].key);
        free(map->table[i].value);
        map->table[i].key = NULL;            
        map->table[i].value = NULL;                
    }

    free(map->table);
    map->table = NULL;
    map->size = 0;
    map->capacity = 0;
    map->sizeOf_key = 0;
    map->sizeOf_value = 0;        
}

static void resizeHashMap(hashMap *map){
    //resize the hashMap's table if its at MAX_LOAD capacity, then rehash each entry into the new table
    int oldCapacity = map->capacity;
    int newCapacity = oldCapacity * 2;
    if(newCapacity < oldCapacity){ //guard against integer overflow
        fprintf(stderr, "[Error] hit max capacity");
        return;
    }
    map->capacity = newCapacity;

    hashEntry *oldTable = map->table; //get the old address
    hashEntry *newTable = calloc(newCapacity, sizeof(hashEntry));
    if(newTable == NULL){
        fprintf(stderr, "[Error] Something happen with allocating new memory.");
        return;
    }
    map->table = newTable;
    map->size = 0;

    for(int i = 0; i < oldCapacity; i++){ //rehash each entry
        if(oldTable[i].key == NULL){ //skip empty entries
            continue;
        }

        //since key and value are pointers, we can set the flag to HM_HEAP
        hashMapSet(map, oldTable[i].key, map->sizeOf_key, oldTable[i].value, HM_HEAP, HM_HEAP); 
    }


    free(oldTable); //evict the old table
}

//returns 0 if all the bytes are 0, 1 otherwise
static inline int isNull(uint8_t *bytes, size_t length){
    for(size_t i = 0; i < length; i++){
        if(bytes[i] != 0){
            return 1;
        }
    }

    return 0;
}

static hashEntry* linearProbing(hashMap *map, const void *key, size_t keySize){
    //find the bucket to for the key.
    hashEntry *tombstone = NULL; //returns if and only if no other entry in the table is not found
    uint64_t hash = hash_fnv_1a_64(key, keySize);
    int idx = hash % map->capacity;
    for(int i = 0; i < map->capacity; i++){
        hashEntry *entry = &map->table[idx];
        if(
            (entry->status == OCCUPIED) && 
            (entry->key != NULL) && 
            (map->cmp(entry->key, key) == 0)
        ){
            return entry;
        }

        if(entry->status == EMPTY){
            //return the entry that is either empty or has the key
            return entry;
        }

        else if(entry->status == EVICTED){
            tombstone = entry;
        }
        
        idx = (idx + 1) % map->capacity;
    }



    return tombstone;
}

uint64_t hash_fnv_1a_64(const void *key, size_t keySize){
    //using the 64 bit version
    uint64_t hashValue = FNV_offset_basis; //per fnv-1a, set the initial value of the hash to the prime
    uint8_t *bytes = (uint8_t *)key; //read each byte of the key as a uint8

    //xor each byte and multipy it by the hash value
    for(size_t i = 0; i < keySize; i++){
        hashValue ^= bytes[i];
        hashValue *= FNV_prime;
    }

    return hashValue;
}

hashEntry* hashMapGet(hashMap* map, const void* key, size_t keySize){
    //early return if capacity is 0
    if(map->capacity == 0){
        return NULL;
    }
    
    //hasn't returned, so check if the entry.key == key
    hashEntry *entry = linearProbing(map, key, keySize);
    
    //check if the bucket is empty
    if((entry->status == EMPTY) || (entry->status == EVICTED)){ //NULL key means empty bucket
        return NULL;
    }

    if(
        (entry->status == OCCUPIED) && 
        (entry->key != NULL) && 
        (map->cmp(entry->key, key) == 0)
    ){
        return entry; //keys match
    }

    return NULL;
}

void hashMapSet(
    hashMap *map, 
    void* key, size_t keySize, void* value, 
    hm_flag keyFlag, hm_flag valueFlag
){
    //copy the bytes if the flags are stack, set the pointers if the flags are heap
    //nothing to set if the capacity is 0;
    if(map->capacity == 0){
        return;
    }

    if(map->capacity * MAX_LOAD <= map->size){ //resize the map
        resizeHashMap(map);
    }

    //get an empty entry or entry of the same key
    hashEntry *entry = linearProbing(map, key, keySize);
    if (entry == NULL) {
        fprintf(stderr, "[Error] hash map full or probing failed\n");
        return;
    }

    if(entry->key == NULL){ //key is null, new item
        //set the entry key and value
        if(keyFlag == HM_STACK){ //copy the key into the entry
            void *destKey = malloc(map->sizeOf_key);
            if(destKey == NULL){
                fprintf(stderr, "[Error] failure in malloc");
                return;
            }
            entry->key = memcpy(destKey, key, map->sizeOf_key); //pointer to this buffer
        }

        else if(keyFlag == HM_HEAP){ //point to the key
            entry->key = key; //bp, check if the key is an address
        }

        if(valueFlag == HM_STACK){ //copy the value into the entry
            void *destValue = malloc(map->sizeOf_value);
            if(destValue == NULL){
                fprintf(stderr, "[Error] failure in malloc");
                return;
            }
            entry->value = memcpy(destValue, value, map->sizeOf_value);
        }

        else if(valueFlag == HM_HEAP){ //point to the value
            entry->value = value;
        }

        entry->status = OCCUPIED;
        map->size++;
    }

    else if(map->cmp(entry->key, key) == 0){ //updating the entry
        //free the duplicate key
        if(keyFlag == HM_HEAP){
            free(key);
        }
        
        //reset the entry value
        if(valueFlag == HM_HEAP){
            //free the pointer to the value
            free(entry->value);
            entry->value = value; //change the ptr to the value
        }

        else if(valueFlag == HM_STACK){
            memcpy(entry->value, value, map->sizeOf_value);
        }
    }
}

void hashMapRemoveKey(hashMap *map, const void *key, size_t keySize){
    hashEntry *entry = linearProbing(map, key, keySize);
    if ((entry == NULL) || (entry->status != OCCUPIED)) {
        fprintf(stderr, "[Error] hash map full or probing failed\n");
        return;
    }

    //set the bucket that is of the key to be a tombstone
    if(map->cmp(entry->key, key) == 0){
        free(entry->key);
        free(entry->value);
        entry->key = NULL;            
        entry->value = NULL;
        entry->status = EVICTED;                
    }
}
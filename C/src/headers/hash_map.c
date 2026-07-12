#include "hash_map.h"
#include <stdio.h>
#include <stdlib.h>


#define MAX_SIZE_FRACTION 0.75
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
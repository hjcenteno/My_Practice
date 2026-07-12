/*
    Author: Henry Centeno
    Description:
        This file will define the implementation of the hash map and its related functions.

    Notes:
        *implementation based on https://medium.com/@anthonybaxter819/hash-maps-in-c-93be2b18c412
*/

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct hashEntry{
    //void pointers to avoid restricting the type of whatever key and the entries can be
    void *key;
    void *value;
} hashEntry;

typedef struct hashMap{
    hashEntry *table; //dynamic array that will house the buckets each item in the hash map
    size_t sizeOf_key; //the byte size of the key since we store these as void
    size_t sizeOf_value; //the byte size of the value since we store these as void
    int size;
    int capacity;
    int (*cmp)(const void *a, const void *b); //function pointer to some cmp function
} hashMap;

// static char cmp(const void*, const void*); //function pointer to a cmp function

void test();

/*
    implementaion:
        Using the capacity, *table will be allocated enough memory to hold capacity number of items.
        all the other fields will be set to 0;
*/
void createHashMap(
    hashMap *map, int capacity, 
    size_t keySize, size_t valueSize,
    int (*cmp)(const void* a, const void* b)
); //handle initiliazing the hash map

/*
    implementation:
        Goes through all the stuff in tables, frees it, then frees table
*/
void deleteHashMap(hashMap* map); //handle freeing the map from memory

/*
    implementation:
        using the key, gets the hash of the key and sees whether the item exists in the table. 

*/
hashEntry* hashMapGet(hashMap* map, const void* key, int keySize); //see if a key exists in the map, returns the entry if true, null if false

void hashMapSet(hashMap* map, const void* key, const void* value); //set the item

/*
    implementation:
        using the actual bytes of the key, we'll use that to hash the key using the fnv-1a algorithm
*/
uint64_t hash_fnv_1a_64(const void *key, size_t keySize); //the hashing function based on the FNV-1a algorithm, returns the hash

int hashMapLength(hashMap *map); //return the size of the hash map

static void resizeHashMap(hashMap *map); //resize the hash map whenever it needs to grow

void hashMapRemoveKey(hashMap *map, const void *key); //get the item at the key, free/set it to null, then state that the bucket is a tombstone

#endif
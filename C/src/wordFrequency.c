/*
    Author: Henry Centeno 
    Date: 2026-07-06
    Description: 
        This will be my personal implementation of a hashmap in C. Inspiration of this implementation is based on https://benhoyt.com/writings/hash-table-in-c/
        in that I will use the hashmap to keep count of the number of unique words. Difference in my implentation is that I will read a text file of some random
        words or something then output the count. 

    Usage:
        ./hash_table -l/h n < input_file.txt
    
    notes:
        -l will be the n number of the least counted words, h will be the highest numbers
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/hash_map.h"
#include <ctype.h>
#include <inttypes.h>

#define DEFAULT_PRINT_AMOUNT 20
#define MAX_WORD_COUNT 1000 //maximum number of words to be read
#define MAX_WORD_LENGTH 256 //255 + '\0'

enum mode {
    low,
    high,
    error,
};

/*
    converts each word to uppercase for all the characters that are alphanumeric.
    note: 
        *for simplicity, 
*/
char* sanitize_word(const char *unsanitizedWord){
    int length = strlen(unsanitizedWord) + 1; //length is handled by read_input
    char *word = malloc(sizeof(char) * length); //word will never be >= length
    if(word == NULL){
        return NULL;
    }
    
    //read each character in the unsanitized word
    int wordLength = 0; //length of the sanitized word
    for(int i = 0; i < length; i++){
        char byte = unsanitizedWord[i];
        if(byte == '\0'){
            break;
        }

        if(isalnum((unsigned char ) byte)){ //convert to unsigned to avoid unknown behavior from non ascii encoding of the word
            word[wordLength++] = toupper((unsigned char ) byte);
        }

        //check if the byte is a possession apostrophe ('s)
        else if(byte == '\''){
            if((i + 2 == length - 1) && (unsanitizedWord[i + 1] == 's')){
                break;
            }
        }
    }
    
    word[wordLength] = '\0';
    return word;
}

/*
    read's in the input one word at a time, creates a key from that word to then be tracked by the hashmap.
    returns -1 upon some issue that happens while reading. 0 by default
*/
int read_input(hashMap *map){
    char buffer[MAX_WORD_LENGTH]; //includes the null character
    int wordCount = 0; //keep count of the number of words
    int result;

    //read in each word from the file
    while((result = scanf("%255s", buffer)) == 1){
        if(wordCount > MAX_WORD_COUNT){
            fprintf(stderr, "[Warning] Number of words read exceeds %d.\n", MAX_WORD_COUNT);
            return -1;
        }
        
        //before hashing, sanitize the word
        char *key = sanitize_word(buffer);
        if(key == NULL){
            fprintf(stderr, "[Error] Problem with malloc");
            return -1;
        }

        size_t keyLength = strlen(key);
        printf("%s = %ld\n", key, keyLength);

        uint64_t hashedKey = hash_fnv_1a_64(key, keyLength);
        int index = hashedKey % 16;
        printf(
            "Key: %s hashes to %"PRIu64" with index of %d\n", 
            key, hashedKey, index
        );

        //insert the word into the map
        hashEntry *item;
        if((item = hashMapGet(map, key, keyLength)) != NULL){ //first seee if it exist
            //update the value if true
            int value = *(int *) item->value;
            value++;
            hashMapSet(map, key, keyLength, &value, HM_HEAP, HM_STACK);
        }else{
            int initialValue = 1;
            hashMapSet(map, key, keyLength, &initialValue, HM_HEAP, HM_STACK); //begin count of the word
        }
        
        wordCount++;
        // free(key);
    }

    if(result != EOF){
        fprintf(stderr, "[Error] Unexpected scanf() error\n");
        return -1;
    }

    return 0;
}

int keycmp(const void *a, const void *b){
    const char *sa = (const char *)a;
    const char *sb = (const char *)b;
    return strcmp(sa, sb);
}

int main(int argc, char const *argv[]){
    //error check the usage
    if(argc != 2){
        printf(
            "[Error] usage: %s [-l/h] < input_file.txt\n"
            "-l for least seen words\n"
            "-h for most seen words\n",
            argv[0]
        );
        return 1;
    }

    //error check the args
    enum mode output = error;
    if(strcmp("-l", argv[1]) == 0){
        output = low;
    }

    if(strcmp("-h", argv[1]) == 0){
        output = high;
    }

    if(output == error){
        printf(
            "[Error] usage: %s [-l/h] < input_file.txt\n"
            "-l for least seen words\n"
            "-h for most seen words\n",
            argv[0]
        );
        return 1;
        
    }

    test(); //see if headers/hashMap.h is included
    printf("sizeof(hashMap) = %ld\n", sizeof(hashMap));
    printf("sizeof(hashEntry) = %ld\n", sizeof(hashEntry));
    hashMap map;
    int initialCap = 16; //hold 16 unique words for now
    createHashMap(
        &map, initialCap,
        MAX_WORD_LENGTH, sizeof(int),
        keycmp
    );
    
    //read from stdin
    if(read_input(&map) == -1){
        printf("[Error] Error encounter while reading input. Exiting program.\n");
        return -1;
    }
    
    if(output == low){
        printf("low\n");
    }
    
    if(output == high){
        printf("high\n");
    }
    for(int i = 0; i < map.capacity; i++){
        hashEntry *bucket = &map.table[i];
        if(bucket->status != OCCUPIED){
            continue;
        }

        printf("key: %s\nValue: %d\n", (char *)bucket->key, *(int *)bucket->value);
    }

    printf("deleting hashMap\n");
    deleteHashMap(&map);
    return 0;
}
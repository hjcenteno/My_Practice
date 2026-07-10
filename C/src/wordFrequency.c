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

enum mode {
    low,
    high,
    error,
};

#define DEFAULT_PRINT_AMOUNT 20

int main(int argc, char const *argv[]){
    //error check the usage
    printf("%d\n", argc);
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

    if(output == low){
        printf("low\n");
    }

    if(output == high){
        printf("high\n");
    }

    test(); //see if headers/hashMap.h is included

    //read from stdin

    

    return 0;
}

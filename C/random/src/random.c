/*
    author: Henry Centeno
    description:
        Take in 4 arguments: seed N upper lower
        -seed: user specified seed
        -N: number of random numbers to output
        -upper: upper limit of the random numbers
        -lower: lower limit of the random numbers
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char const *argv[]){
    //error check usage
    if(argc != 5){
        fprintf(stderr, "Usage: %s seed N upper lower\n", argv[0]);
        printf(
            "-seed: user specified seed\n"
            "-N: number of random numbers to output\n"
            "-upper: upper limit of the random numbers\n"
            "-lower: lower limit of the random numbers\n"
        );
        return 1;
    }
    
    // It is recommended to instead use the strtol() and strtoul() family of functions in new programs.
    char *endptr;
    int base, seed, numIter, upper, lower;
    base = 0;

    errno = 0;    /* To distinguish success/failure after call */
    strtol("0", NULL, base);
    if (errno == EINVAL) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

    //verify seed argument
    errno = 0;
    seed = (int)strtol(argv[1], &endptr, base);
    if((errno == ERANGE) || (endptr == argv[1])){
        fprintf(stderr, "Invalid argument: %s\n", argv[1]);
        printf("seed should be a valid integer\n");
        return 1;
    }
    printf("seed: %d\n", seed);

    //verify n argument
    errno = 0;
    numIter = (int)strtol(argv[2], &endptr, base);
    if((numIter <= 0) || (errno == ERANGE) || (endptr == argv[2])){
        fprintf(stderr, "Invalid argument: %s\n", argv[2]);
        printf("N should be an integer > 0\n");
        return 1;
    }
    printf("numIter: %d\n", numIter);

    //verify upper argument
    errno = 0;
    upper = (int)strtol(argv[3], &endptr, base);
    if((errno == ERANGE) || (endptr == argv[3])){
        fprintf(stderr, "Invalid argument: %s\n", argv[3]);
        printf("upper should be a valid integer\n");
        return 1;
    }
    printf("upper: %d\n", upper);

    // //verify lower argument
    errno = 0;
    lower = (int)strtol(argv[4], &endptr, base);
    if((errno == ERANGE) || (endptr == argv[4]) || (lower >= upper)){
        fprintf(stderr, "Invalid argument: %s\n", argv[4]);
        printf("lower should be a valid integer\n");
        return 1;
    }
    printf("lower: %d\n", lower);
    
    srand(seed); //use the seed
    for(int i = 0; i < numIter; i++){
        int randNum = (rand() % (upper - lower + 1)) + lower;
        printf("%d\n", randNum);
    }

    return 0;
}

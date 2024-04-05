/* um 
 * CS 40 Assignment 6
 * Made by Spenser Rose (*UTLN*) and Cheng Li (cli31)
 */

#include <stdio.h>
#include <stdlib.h>
#include <um_operations.h>

int main(int argc, char *argv[]) {
    /* Usage: ./um [file.um] */
    if (argc != 2) {
        fprintf(stderr, "Usage: ./um [file.um]\n");
        exit(EXIT_FAILURE);
    }
    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        fprintf(stderr, 
                "Error: Cannot open file '%s' for reading.\n", 
                argv[argc - 1]);
        exit(EXIT_FAILURE);
    }

    /* template done, code starts here */


    return 0;
}
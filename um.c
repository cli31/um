/* um 
 * CS 40 Assignment 6
 * Made by Spenser Rose (*UTLN*) and Cheng Li (cli31)
 */

/* c lib called */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* lower abstractions */
#include <um.h>
#include <um_operations.h>

/* Hansen ADTs */
#include <Table.h>

#define UM_segments Table_T
const int HINT = 1000; /* estimated number of segments */


typedef struct UM {
    uint32_t r[8]; /* 8 general-purpose registers holding 32-bit word each. */
    UM_segments * segs; /* ptr to a Table_T where ids are key and address of
                           memory (in 64 bits) as value */
    // TODO IO device: gonna implement as function ptr?
    uint32_t counter;
} UM;

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
    /* Step1: read in instructions from fp */
    uint32_t *buffer = (uint32_t *)malloc(sizeof(uint32_t));
    size_t   num_of_inst = 0;
    while (fread(buffer, sizeof(buffer), 1, fp) == 1) {
        num_of_inst++; /* first get num of insts for easier memeory alloc */
    }
    assert(fp == EOF); /* check .um file has complete instuctions */
    fclose(fp); /* re-read the file to set fp back to the beginning */
    fp = fopen(argv[1], "rb");
    if (!fp) {
        fprintf(stderr, 
                "Error: Cannot open file '%s' for reading.\n", 
                argv[argc - 1]);
        exit(EXIT_FAILURE);
    }
    buffer = realloc(buffer, sizeof(uint32_t) * num_of_inst);
    fread(buffer, sizeof(buffer), 1, fp);

    /* Step2: initialize um */
    UM um = {
        .r = { 0UL } /* regs starts at all 0s*/
        .segs = Table_new(HINT, strcmp(), void); // ? not sure the right syntax
        .counter = 0;
    }
    /* all pseudo code later
    table_put [0, buffer]
    execution cycle:
    for counter: 0 - num of inst
        access each inst and parse it: opcode, a, b, c
        switch case 0-13: // ? we can pass the um in so regs and segs can be accessed, should we do that?
        for seg map: create a new ptr and alloc r[c] of 32 bits, table
            put [r[b], ptr] (remember to check run-time error);
        for seg ummap: table remove r[c]. push id (r[c]) to seq. // ? I still did not figure out how to reuse the id if alloc'ed memory is not always the same

        for seg store: table get key r[a], dereference and replace the r[b]_th word 
        with r[c] (use bitpack) table put the new value back
        for seg load:  table get key r[b], dereference and retrieve the r[c]_th word
        (use bitpack) and assign it to r[a]

        for load program: // TODO
    */


    return 0;
}
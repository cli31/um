/* um 
 * CS 40 Assignment 6
 * Made by Spenser Rose (srose05) and Cheng Li (cli31)
 */

/* c lib called */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* lower abstractions */
#include "um_operations.h"

const int HINT = 1000; /* estimated number of segments */

/* struct UM is in um_operations.h */

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
    /* it is an oop idea, code inside of this block is equivalent to um.run() */
    /* Step1: read in instructions from fp */
    uint32_t *buffer = (uint32_t *)malloc(sizeof(uint32_t));
    size_t   num_of_inst = 0;
    while (fread(buffer, sizeof(buffer), 1, fp) == 1) {
        num_of_inst++; /* first get num of insts for easier memeory alloc */
    }
    assert(feof(fp)); /* check .um file has complete instuctions */
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
        .r = { 0UL }, /* regs starts at all 0s */
        .segs = NULL, /* rvalue to lvalue need to be done outside */
        .counter = 0
    };
    UM_segments t = Table_new(HINT, NULL, NULL);
    um.segs = &t;
    assert(Table_put(*um.segs, &um.counter, buffer)); /* check m[0] was null */

    /* Step3: execution cycle */
    for (; um.counter < num_of_inst; um.counter++) {
        /* use buffer instead of Table_get for a simpler expression */
        Um_instruction curr_inst = buffer[um.counter];
        Um_opcode opcode = 14; /* opcode from 0-13, check for valid inst */
        Um_register ra = 8, rb = 8, rc = 8; /* reg from 0-7 */
        parse_inst(&curr_inst, &opcode, &ra, &rb, &rc);
        switch (opcode) {
            case CMOV:
                break;
            
            default:
        }
        
    }
    /* all pseudo code later
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

        for load program:
    */



    return 0;
}
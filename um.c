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
    UM um;
    UM_segments t = Table_new(HINT, NULL, NULL);
    um.segs = &t;
    assert(Table_put(*um.segs, (void *)(uintptr_t)um.counter,
                               (void *)(uintptr_t)buffer)); /* m[0] != null */
                               /* buffer was treated as a 64-bit int */
    Seq_T t = Seq_new(HINT);
    um.reuse_id = &t;

    /* Step3: execution cycle */
    for (; um.counter < num_of_inst; um.counter++) {
        /* use buffer instead of Table_get for a simpler expression */
        Um_instruction curr_inst = *(buffer + um.counter);
        Um_opcode opcode = 14; /* opcode from 0-13, check for valid inst */
        Um_register ra = 8, rb = 8, rc = 8; /* reg from 0-7 */
        parse_inst(&curr_inst, &opcode, &ra, &rb, &rc);
        switch (opcode) {
            case CMOV:
                conditional_move(&um, ra, rb, rc);
                break;
            case SLOAD:
                break;
            case SSTORE:
                break;
            case ADD:
                addition(&um, ra, rb, rc);
                break;
            case MUL:
                multiplication(&um, ra, rb, rc);
                break;
            case DIV:
                division(&um, ra, rb, rc);
                break;
            case NAND:
                bitwise_NAND(&um, ra, rb, rc);
                break;
            case HALT:
                halt();
                break;
            case ACTIVATE:

                break;
            case INACTIVATE:
                break;
            case OUT:
                ouput(&um, rc);
                break;
            case IN:
                input(&um, rc);
                break;
            case LOADP:
                /* since in the beginning of func cycle, we chose to use buffer 
                instead of table get $m[0] */
                /* need to update buffer and num_of_inst */
                break;
            case LV:
                uint32_t value = Bitpack_getu(curr_inst, 25, 0);
                load_value(&um, ra, value);
                break;
            default:
        }

    }

    /* free everything */
    

    return 0;
}
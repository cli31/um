/*
 * um_components.h
 * interface of components of UM
 */
#ifndef UM_COMPONENTS_H
#define UM_COMPONENTS_H

#include <stdlib.h>
#include <string.h>

#include <table.h>
#include <seq.h>

const int HINT = 1000; /* estimated number of segments */
const int num_of_regs = 8;

/* UM segments struct 
 * a pointer pointing to the memory space, used in form of segments
 * contains:
 *      a Table ADT, key = id, value = address; address was taken
 *  literally. Each address points to a segment.
 *      a Seq ADT, stores unmapped ids.
 * usages:
 *      um initialized with segment 0 load with program
 *      map segment (opcode 8) allocates a new word sequence with all 0s except
 *  the first word to be the size of the seq.
 *      ummap segment (opcode 9) does not dealloc' the memory, but only remove
 *  key and value pair from the Table, and push the key to Seq; when the id is 
 *  going to be reused, realloc' a new memory space to it (and prolly
 *  initialize right after).
 *      load program use memcpy to copy a segment to 0th segment, and reset
 *  counter, while both pointer was unchanged.
*/

/* UM_segments struct 
 * contains memory space and unmapped id
 
 */
typedef struct UM_segments
{
    Table_T mem_space;
    Seq_T   unmapped_ids;
} UM_segments;

/* UM struct: 
 * all the components will be used in the Universal Machine
 * contains:
 *      8 registers, each holds 32 bit
 *      a ptr to UM_segments type
 *      a counter, 32 bits
 * 
 * (no need to implement IO device since all the I/O's are from stdin and 
 * stdout, which can be done by using outer system)
 */
typedef struct UM {
        uint32_t r[num_of_regs]; /* 8 general-purpose registers, 32-bit each. */
        UM_segments segs;
        uint32_t counter;
} UM;

/* helper functions */
/* cmp is used in Table_new. For it's functionality, pls refer to Hanson */
int intcmp(const void *x, const void *y) {
    return (int)(*x) - (int)(*y);
}

extern void initialize_UM(UM *um) {
    for (int i = 0; i < num_of_regs; i++) {
        um->r[i] = 0;
    }
    um->counter = 0;
    um->segs.mem_space    = Table_new(HINT, intcmp(), NULL);
    um->segs.unmapped_ids = Seq_new  (HINT); 
}

/* used for debugging -- print out the machine to stderr */
void print_um(const UM *um) {
    fprintf(stderr, "counter = %d\n", um->counter);
    for (int i = 0; i < num_of_regs; i++) {
        fprintf(stderr, "r[%d] = %d\t", i, um->r[i]);
    }

    fprintf(stderr, "\nsegmented memory:\n");
    void **iterator = Table_toArray(um->segs.memspace, NULL);
    /* Iterate over the array until the end marker is encountered */
    while (*iterator != NULL) {
        const uint32_t *key = (const uint32_t *)*iterator++;
        const uint32_t **value = (const uint32_t **)*iterator++;

        // Print the key-value pair
        fprintf("id: %d,\t", *key);
        uint32_t size = value[0];
        fprintf("number of inst: %d,\t", size);
        for (int i = 1; i < size + 1; i++) {
            uint32_t curr_inst = value[i];
            Um_opcode opcode = 14;
            Um_register ra = 8, rb = 8, rc = 8;
            parse_inst(&curr_inst, &opcode, &ra, &rb, &rc);
            if (opcode == LV) {
                fprintf(stderr, "opcode = %d,\tra = %d,\tvalue = %d\n", opcode, 
                ra, Bitpack_getu(curr_inst, 25, 0));
            }
            else {
                fprintf(stderr, "opcode = %d,\tra = %d,\trb = %d,\trc = %d\n",
                        opcode, ra, rb, rc);
            }
            fprintf("*****next word*****\n");
        }
        fprintf("**********next segment**********\n\n");
    }

    fprintf(stderr, "\nunmapped id:\n");
    uint32_t length = Seq_length(um->segs.unmapped_ids);
    for (int i )
}

#undef UM_COMPONENTS_H
/*
 * um_components.h
 * interface of components of UM
 */
#ifndef UM_COMPONENTS_H
#define UM_COMPONENTS_H

#include <table.h>
#include <seq.h>

const int HINT = 1000; /* estimated number of segments */
const int num_of_regs = 8;

/* UM segments struct 
 * a pointer pointing to the memory space, used in form of segments
 * contains:
 *      a Table ADT, key = id (atom), value = address (atom); address was taken
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
typedef struct UM_segments
{
    Table_T mem_space;
    Seq_T   unmapped_ids;
};

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
        UM_segments *segs;
        uint32_t counter;
} UM;

/* helper functions */
void initialize_UM(UM *um) {
    for (int i = 0; i < num_of_regs; i++) {
        um->r[i] = 0;
    }
    um->segs->
}

#undef UM_COMPONENTS_H
/*
 * um_components.h
 * interface of components of UM
 */
#ifndef UM_COMPONENTS_H
#define UM_COMPONENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <bitpack.h>
#include <assert.h>

#include <table.h>
#include <seq.h>

#define HINT 1000 /* estimated number of segments */
#define num_of_regs 8

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
 * Table:
 *      key     value
 *      id      pointer --> sequence of 32 bit words
 *                          size    0th     1th     2th     ...
 * By using this structure, 32 bit id can be used to indicate the 64 bit address
 * 
 * Seq:
 *      unmapped ptr (not the 32 bit id! since under the hood we use realloc to
 *  recycle id)
 *      additionally, the spec proved my point: "A bit pattern that is not all 
 * zeroes and that does not identify any CURRENTLY MAPPED segment is placed in 
 * $r[B]." It means, for example, when calling map segment and gives a new id
 * 17, even though there is an id 17 in the seq, a new pair is still created in
 * table. In this case, if we store the 32 bit id instead of actual address, it
 * actually has nothing to do with the program.
 *      so to understand the id reusing in map_segment and ummap_segment:
 * unmap_segment removes the id and ptr pair, push the ptr into the seq and
 * forget id. When the seq is not empty, map_segment prioritize to use ptrs in
 * seq, and make it accessible from client by assigning a new id to it -- an
 * id not conflict with any available segment. In the end, we recycled the 
 * memory by having a new identifier to it.
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
 *      a segment type which explained before
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

typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;
typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

/* helper functions */
/* cmp is used in Table_new. For it's functionality, pls refer to Hanson */
int intcmp(const void *x, const void *y);
unsigned hash(const void *x);

void parse_inst(Um_instruction *curr_inst, Um_opcode *opcode, Um_register *ra, 
                Um_register *rb, Um_register *rc);

extern void initialize_UM(UM *um);

/* used for debugging -- print out the machine to stderr */
void print_um(const UM *um);

void free_um(UM *um);

#endif
/******************************************************************
 *  um_operations.h
 *  Spencer Rose (srose05) and Cheng Li (cli31)
 *  Date last edited: 4/12/24
 *
 *  CS 40 HW 6: UM
 *
 *  Header file defining the operations for the UM.The UM operations manipulate
 *  registers and memory segments according to the instructions encoded in UM
 *  programs, facilitating the execution of a wide range of computations and
 *  I/O interactions.
 ******************************************************************/
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
 *      a Seq ADT, index = id, value = ptr. Each address points to a segment.
 *      a Seq ADT, stores unmapped ids.
 * usages:
 *      um initialized with segment 0 load with program
 *      map segment (opcode 8) allocates a new word sequence with all 0s except
 *  the first word to be the size of the seq.
 *      ummap segment (opcode 9) does not dealloc' the memory, but only push
 *  the id to Seq; when another map segment is called, unmapped id is
 *  prioritized to be reused, realloc' a new memory space to the ptr
 *      load program use memcpy to copy a segment to 0th segment, and reset
 *  counter, while both pointer was unchanged.
*/

/* UM_segments struct
 * contains memory space and unmapped id
 * mem_space (Seq_T):
 *      index      id
 *      value      pointer --> sequence of 32 bit words
 *                               size    0th     1th     2th     ...
 * By using this structure, 32 bit id can be used to indicate the 64 bit 
 * address
 *
 * unmapped_id (Seq_T):
 *      used as a stack: first in last out; use the lastly freed id.
 *      when unmapped_id is not empty and map_segment called, pop the high_end
 * and use the ptr at that index.
 */
typedef struct UM_segments
{
    Seq_T mem_space;
    Seq_T unmapped_ids;
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
        uint32_t r[num_of_regs]; /* 8 general-purpose registers, 32-bit each.*/
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
int parse_inst(Um_instruction *curr_inst,
               Um_register *ra, Um_register *rb, Um_register *rc);

void initialize_UM(UM *um);

/* used for debugging -- print out the machine to stderr */
void print_um(const UM *um);

void free_um(UM *um);

#endif
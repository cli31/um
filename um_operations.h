/*
 * um_operations.h
 * made by Cheng Li
 * the interface of operations 0 - 13
 */
#include <stdbool.h>
#include <table.h>
#include <bitpack.h>
#include <assert.h>

typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

typedef Table_T UM_segments;

typedef struct UM {
    uint32_t r[8]; /* 8 general-purpose registers holding 32-bit word each. */
    UM_segments *segs; /* ptr to a Table_T where ids are key and address of
                           memory (in 64 bits) as value */
    // ? IO device: gonna implement as function ptr?
    uint32_t counter;
} UM;

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;
extern bool parse_inst(Um_instruction *curr_inst, 
        Um_opcode *opcode, Um_register *ra, Um_register *rb, Um_register *rc);

extern void bitwise_NAND(UM *um, Um_register a, Um_register b, Um_register c);

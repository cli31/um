/* um_operations.c
 * made by Cheng Li
 * the implement of um_operations
 */
#include "um_operations.h"

/* parse_inst
 * parse through the instruction and populate opcode and regs
 * return true if there is a value to load (false for opcode 0-12)
 */
extern void parse_inst(Um_instruction *curr_inst, Um_opcode *opcode, 
                       Um_register *ra, Um_register *rb, Um_register *rc) 
{
    *opcode = Bitpack_getu(*curr_inst, 4, 28);
    assert(*opcode >= 0 && *opcode <= 13);

    if (*opcode == 13) {
        (void)rb;
        (void)rc;
        *ra = Bitpack_getu(*curr_inst, 3, 25);
        /* no need to assert since 3 bits are already 0-7 */
        /* value will be parsed */
    }
    else {
        *ra = Bitpack_getu(*curr_inst, 3, 6);
        *rb = Bitpack_getu(*curr_inst, 3, 3);
        *rc = Bitpack_getu(*curr_inst, 3, 0);
    }
}

/* opcode 0
 * Conditional Move
 * if $r[C]= 0 then $r[A] := $r[B]
 */
void conditional_move(UM *um, Um_register ra, Um_register rb, Um_register rc)
{
    if (um->r[rc] != 0) {
        um->r[ra] = um->r[rb];
    }
}

/* opcode 1
 * Segmented Load
 * $r[A] := $m[$r[B]][$r[C]]
 */
// TODO: not now since details abt segment memory not settled
void sload(Um_register ra, Um_register rb, Um_register rc)
{
    r->ra = segment(r->rb)[r->rc];
}

/* opcode 2
 * Segmented Store
 */
void sstore(Um_register ra, Um_register rb, Um_register rc)
{
    segment(ra)[rb] = r->rc;
}


/* opcode 3
 * Addition
 * $r[A] := ($r[B] + $r[C]) mod 2^32
 */
void addition(UM *um, Um_register ra, Um_register rb, Um_register rc)
{
    um->r[ra] = um->r[rb] + um->r[rc];
}

 /* opcode 4
 * Multiplication
 * $r[A] := ($r[B] × $r[C]) mod 2^32
 */
void multiplication(UM *um, Um_register ra, Um_register rb, Um_register rc)
{
        // ?
        um->r[ra] = ((uint64_t)um->r[rb] * (uint64_t)um->r[rc]) % UINT32_MAX;
}


 /* opcode 5
 * Division
 * $r[A] := ⌊$r[B] ÷ $r[C]⌋
 */
void division(UM *um, Um_register ra, Um_register rb, Um_register rc)
{
        if (um->r[rc] == 0) {
        // Division by zero error handling.
            fprintf(stderr, "Did you really try to divide by 0 :(\n");
            return;
        }
        um->r[ra] = um->r[rb] / um->r[rc];
}

/* opcode 6
 * Bitwise NAND
 * $r[A] := ¬($r[B] ∧ $r[C]);
 */
void bitwise_NAND(UM *um, Um_register a, Um_register b, Um_register c) 
{
    um->r[a] = ~(um->r[b] & um->r[c]);
}

/*
 * opcode 7
 * Halt
 * Computation stops
 */
void halt(void) {
}

/* 
 * opcode 8
 * Map Segment
 * A new segment is created with a number of words equal to the value in $r[C].
 * Each word in the new segment is initialized to 0. A bit pattern that is not
 * all zeroes and that does not identify any currently mapped segment is placed
 * in $r[B]. The new segment is mapped as $m[$r[B]].
 */
void map_segment(UM *um, Um_register b, Um_register c) {
    assert(Table_get(*(um->segs), um->r[b]) != NULL);
    

}

/*
 * opcode 10
 * Output
 * The value in $r[C] is written to the I/O device immediately. Only values
 * from 0 to 255 are allowed.
 */
void output(UM *um, Um_register c) 
{
    assert (um->r[c] <= 255);
    putchar(um->r[c]);
}

/* opcode 11
 * Input
 * The universal machine waits for input on the I/O device. When input arrives,
 * $r[C] is loaded with the input, which must be a value from 0 to 255. If the
 * end of input has been signaled, then $r[C] is loaded with a full 32-bit
 * word in which every bit is 1.
 */
void input(UM *um, Um_register c) 
{
    uint32_t input = fgetc(stdin); // ?
    if (input != EOF) {
        assert(input <= 255);
        um->r[c] = input;
    }
    else {
        um->r[c] = -1;
    }
}

/*
 * opcode 12
 * Load Program
 * Segment $m[$r[B]] is duplicated, and the duplicate replaces $m[0], which is
 * abandoned. The program counter is set to point to $m[0][$r[C]]. If $r[B] = 0,
 * the load-program operation is expected to be extremely quick.
 */
void load_program(UM *um, Um_register b, Um_register c) 
{
    if (um->r[b] == 0) {
        return; /* do nothing == extremely quick */
    }
    void *t = Table_get(*(um->segs), um->r[b]);
    assert(t != NULL); /* check $m[$r[b]] is mapped */
    uint32_t value = (uint32_t)(uintptr_t)t; /* convert t */
    
    /* $m[$r[b]] is duplicated instead of redirected */

}

/* opcode 13
 * Load Value
 * sets $r[A] to the value (lsb 25 bits in instructions)
 */
void load_value(UM *um, Um_register a, uint32_t value) 
{
    um->r[a] = value;
}
/* um_operations.c
 * made by Cheng Li
 * the implement of um_operations
 */
#include "um_operations.h"

/* parse_inst
 * parse through the instruction and populate opcode and regs
 * return true if there is a value to load (false for opcode 0-12)
 */
extern void parse_inst(Um_instruction *curr_inst,
        Um_opcode *opcode, Um_register *ra, Um_register *rb, Um_register *rc) {
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
void execute_cmov(uint32_t* regs, uint32_t ra, uint32_t rb, uint32_t rc)
{
    if (regs[rc] != 0) {
        regs[ra] = regs[rb];
    }
}

//what type should segment be?
Umsegment_T* segment(int n)
{
    if (n == 0) {
        return //?
    } else {
        return return_id(amap, n);
    }
}
/* opcode 1
 * Segmented Load
 * $r[A] := $m[$r[B]][$r[C]]
 */
// TODO: not now since details abt segment memory not settled
void execute_sload(uint32_t ra, uint32_t rb, uint32_t rc)
{
    ra = segment(rb)[rc];
}

// Function to perform the SSTORE operation
void execute_sstore(uint32_t ra, uint32_t rb, uint32_t rc)
{
    segment(ra)[rb] = rc;
}


/* opcode 3
 * Addition
 *
 */
void execute_add(uint32_t* regs, uint32_t ra, uint32_t rb, uint32_t rc)
{
        regs[ra] = (regs[rb] + regs[rc]) % UINT32_MAX;
}



 /* opcode 4
 * Multiplication
 *
 */
void execute_mult(uint32_t* regs, uint32_t ra, uint32_t rb, uint32_t rc)
{
        regs[ra] = (uint64_t)regs[rb] * (uint64_t)regs[rc] % UINT32_MAX;
}


 /* opcode 5
 * Division
 *
 */
void execute_div(uint32_t* regs, uint32_t ra, uint32_t rb, uint32_t rc)
{
        if (regs[rc] == 0) {
        // Division by zero error handling.
        fprintf(stderr, "Did you really try to divide by 0 :(\n");
        EXIT(1);
        return;
        }
        regs[ra] = regs[rb] / regs[rc];
}



/* opcode 6
 * Bitwise NAND
 * $r[A] := ¬($r[B] ∧ $r[C]);
 */
void bitwise_NAND(UM *um, Um_register a, Um_register b, Um_register c) {
    um->r[a] = ~(um->r[b] & um->r[c]);
}

/*
 * opcode 7
 * Halt
 * Computation stops
 */
void Halt(void) {

}

/*
 * opcode 10
 * Output
 * The value in $r[C] is written to the I/O device immediately. Only values
 * from 0 to 255 are allowed.
 */
void output(Um_register c) {

}

/* opcode 11
 * Input
 * The universal machine waits for input on the I/O device. When input arrives,
 * $r[C] is loaded with the input, which must be a value from 0 to 255. If the
 * end of input has been signaled, then $r[C] is loaded with a full 32-bit
 * word in which every bit is 1.
 */
void input(Um_register c) {

}

/* opcode 13
 * Load Value
 * sets $r[A] to the value (lsb 25 bits in instructions)
 */
void load_value(Um_register a) {

}
/******************************************************************
 *  um_operations.c
 *  Spencer Rose (srose05) and Cheng Li (cli31)
 *  Date last edited: 4/11/24
 *
 *  CS 40 HW 6: UM
 *
 *  Implementation of um_operations, defining the set of operations
 *  for the UM. Includes functions for executing each operation defined in the
 *  UM's instruction set, ranging from conditional move to program loading and
 *  input/output handling.
 ******************************************************************/
#include "um_operations.h"

/********** parse_inst **********
 * Parses an instruction and populates opcode and register operands.
 *
 * Parameters:
 *      Um_instruction *curr_inst: Pointer to the current instruction to parse.
 *      Um_opcode *opcode: Pointer to store the parsed opcode.
 *      Um_register *ra, *rb, *rc: Pointers to store parsed register operands.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - curr_inst, opcode, ra, rb, and rc are not NULL.
 *      - curr_inst points to a valid instruction.
 *
 * Effects:
 *      - Extracts the opcode and registers from curr_inst.
 *      - Populates opcode, ra, rb, and rc with the extracted values.
 ******************************/
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

/********** conditional_move **********
 * Performs the Conditional Move operation.
 * If the value in register rc is not zero, the value in register rb is copied
 * to register ra.
 *
 * Parameters:
 *      UM *um: Pointer to the UM instance executing this operation.
 *      Um_register ra, rb, rc: The registers involved in the operation.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um is not NULL and points to a valid UM instance.
 *      - ra, rb, and rc are valid register indices within the UM.
 *
 * Effects:
 *      - Potentially modifies the value of the register pointed to by ra.
 ******************************/
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


/********** addition **********
 * Adds the values in two registers and stores the result in a third register.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance performing the addition.
 *      Um_register ra: The register to store the sum.
 *      Um_register rb: The first addend register.
 *      Um_register rc: The second addend register.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um, ra, rb, and rc are valid and within the bounds of the UM's
 *        register array.
 *
 * Effects:
 *      - Computes the sum of the values in registers rb and rc, modulo 2^32,
 *        and stores the result in register ra.
 ******************************/
void addition(UM *um, Um_register ra, Um_register rb, Um_register rc)
{
    um->r[ra] = um->r[rb] + um->r[rc];
}

/********** multiplication **********
 * Multiplies the values in two registers and stores the result in a third
 * register, modulo 2^32.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance performing the multiplication.
 *      Um_register ra: The register to store the product.
 *      Um_register rb: The first factor register.
 *      Um_register rc: The second factor register.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um, ra, rb, and rc are valid and within the bounds of the UM's
 *        register array.
 *
 * Effects:
 *      - Computes the product of the values in registers rb and rc, modulo
 *        2^32, and stores the result in register ra.
 ******************************/
void multiplication(UM *um, Um_register ra, Um_register rb, Um_register rc)
{
        // ?
        um->r[ra] = ((uint64_t)um->r[rb] * (uint64_t)um->r[rc]) % UINT32_MAX;
}


/********** division **********
 * Divides the value in one register by the value in another register and
 * stores the quotient in a third register.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance performing the division.
 *      Um_register ra: The register to store the quotient.
 *      Um_register rb: The dividend register.
 *      Um_register rc: The divisor register.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um, ra, rb, and rc are valid and within the bounds of the UM's
 *        register array.
 *      - The value in register rc is not zero.
 *
 * Effects:
 *      - Computes the quotient of the values in registers rb divided by rc and
 *        stores the result in register ra.
 *      - If rc is 0, prints an error message and may terminate the program or
 *        handle the error as defined.
 ******************************/
void division(UM *um, Um_register ra, Um_register rb, Um_register rc)
{
        if (um->r[rc] == 0) {
        // Division by zero error handling.
            fprintf(stderr, "Did you really try to divide by 0 :(\n");
            return;
        }
        um->r[ra] = um->r[rb] / um->r[rc];
}

/********** bitwise_NAND **********
 * Performs a bitwise NAND operation on the values in two registers and stores
 * the result in a third register.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance performing the bitwise NAND.
 *      Um_register a: The register to store the result.
 *      Um_register b: The first operand register.
 *      Um_register c: The second operand register.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um, a, b, and c are valid and within bounds of UM's register array
 *
 * Effects:
 *      - Computes the bitwise NAND of the values in registers b and c and
 *        stores the result in register a.
 ******************************/
void bitwise_NAND(UM *um, Um_register a, Um_register b, Um_register c)
{
    um->r[a] = ~(um->r[b] & um->r[c]);
}

/********** halt **********
 * Halts execution of the Universal Machine.
 *
 * Parameters:
 *      void
 *
 * Return:
 *      void
 *
 * Expects:
 *      - This operation has no parameters.
 *
 * Effects:
 *      - Signals the UM to stop execution.
 ******************************/
void halt(void) {
}

/********** output **********
 * Outputs the value in a register to the I/O device, which must be in the
 * range 0 to 255.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance.
 *      Um_register c: The register containing the value to output.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um and c are valid and within the bounds of the UM's register array.
 *      - The value in register c is between 0 and 255, inclusive.
 *
 * Effects:
 *      - Outputs the value in register c to the standard output as a character.
 ******************************/
void output(UM *um, Um_register c)
{
    assert (um->r[c] <= 255);
    putchar(um->r[c]);
}

/********** input **********
 * Reads a value from the I/O device and stores it in a register. If the end of
 * input is signaled, stores -1 (all bits set).
 *
 * Parameters:
 *      UM *um: The Universal Machine instance.
 *      Um_register c: The register to store the input value.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um and c are valid and within the bounds of the UM's register array.
 *
 * Effects:
 *      - Reads a character from the standard input. If the character is EOF,
 *        stores -1 in register c; otherwise, stores the ASCII value.
 ******************************/
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

/********** load_program **********
 * Duplicates a segment and updates the program to start executing from a new
 * instruction in the zeroth segment.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance executing this operation.
 *      Um_register b: The register containing the index of the segment to
 *      duplicate.
 *      Um_register c: The register containing the starting instruction index
 *      for the new program execution within the zeroth segment.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um is not NULL and points to a valid UM instance.
 *      - b and c are valid register indices within the bounds of the UM's
 *        register array.
 *      - The segment referenced by the value in register b exists in memory
 *
 * Effects:
 *      - If the value in register b is 0, the operation returns immediately,
 *        implying that the current program execution is already optimized for
 *        quick loading.
 *      - Otherwise, the segment at index b is duplicated. The original segment
 *        at index 0 is abandoned (freed), and the duplicate becomes the new
 *        segment 0.
 ******************************/
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

/********** load_value **********
 * Directly sets a register to a specified value.
 *
 * Parameters:
 *      UM *um: Pointer to the UM instance executing this operation.
 *      Um_register a: The register to modify.
 *      uint32_t value: The value to load into the register.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um is not NULL and points to a valid UM instance.
 *      - a is a valid register index within the UM.
 *
 * Effects:
 *      - Sets the value of the register a to the specified value.
 ******************************/
void load_value(UM *um, Um_register a, uint32_t value)
{
    um->r[a] = value;
}
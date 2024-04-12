/*
 * tests.c
 *
*/

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "fmt.h"
#include "seq.h"

void build_addition_test(Seq_T stream)
{
    append(stream, loadval(r1, 5));  // Load 5 into register 1
    append(stream, loadval(r2, 10)); // Load 10 into register 2
    append(stream, add(r3, r1, r2)); // Add r1 and r2, store result in r3
    append(stream, output(r3));      // Output the result, expecting 15
    append(stream, halt());          // Halt execution
}

void build_add_test(Seq_T stream)
{
        append(stream, add(r1, r2, r3));
        append(stream, halt());
}

void build_digit_print(Seq_T stream)
{
        append(stream, loadval(r1, 45));
        append(stream, loadval(r2, 3));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

void build_conditional_move_test(Seq_T stream)
{
    append(stream, loadval(r1, 20));
    append(stream, loadval(r2, 0));
    append(stream, loadval(r3, 5));
    append(stream, cmove(r1, r3, r2));
    append(stream, output(r1));
    append(stream, halt());
}

void build_memory_allocation_test(Seq_T stream)
{
    append(stream, loadval(r1, 3));
    append(stream, alloc(r2, r1));
    append(stream, loadval(r3, 42));
    append(stream, sstore(r2, 0, r3));
    append(stream, sload(r4, r2, 0));
    append(stream, output(r4));
    append(stream, halt());
}

void build_jump_test(Seq_T stream)
{
    append(stream, loadval(r1, 1));
    append(stream, loadval(r2, 3));
    append(stream, jump(r1, r2));
    // The following instructions should be skipped if jump works
    append(stream, loadval(r3, 'F'));
    append(stream, output(r3));
    // Target instruction
    append(stream, loadval(r3, 'P')); // This is the instruction we jump to
    append(stream, output(r3));
    append(stream, halt());
}

void build_simple_print(Seq_T stream)
{
        append(stream, loadval(r1, 69));
        append(stream, output(r3));
        append(stream, halt());
}

void build_divide(Seq_T stream)
{
        append(stream, loadval(r1, 202));
        append(stream, loadval(r2, 4));
        append(stream, divide(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

void build_multiply(Seq_T stream)
{
        append(stream, loadval(r1, 2));
        append(stream, loadval(r2, 3));
        append(stream, multiply(r3, r1, r2));
        append(stream, loadval(r4, 48));
        append(stream, add(r5, r4, r3));
        append(stream, output(r5));
        append(stream, halt());
}

void build_multiply_divide(Seq_T stream)
{
        append(stream, loadval(r1, 2));
        append(stream, loadval(r2, 16));
        append(stream, multiply(r3, r1, r2));
        append(stream, loadval(r7, 8));
        append(stream, divide(r6, r3, r7));
        append(stream, loadval(r4, 48));
        append(stream, add(r1, r6, r4));
        append(stream, output(r1));
        append(stream, halt());
}

void build_nand(Seq_T stream)
{
        append(stream, loadval(r2, 5));
        append(stream, loadval(r3, 2));
        append(stream, nand(r1, r2, r3));
        append(stream, add(r2, r1, r3));
        append(stream, output(r2));
        append(stream, halt());
}

void build_in_out(Seq_T stream)
{
        append(stream, input(r1));
        append(stream, output(r1));
        append(stream, halt());
}

void build_sstore_then_load(Seq_T stream)
{
        append(stream, loadval(r0, 0));
        append(stream, loadval(r1, 3));
        append(stream, loadval(r2, 4));
        append(stream, loadval(r3, 21));
        append(stream, sload(r7, r0, r1));
        append(stream, sstore(r0, r2, r7));
        append(stream, output(r3));
        append(stream, halt());
        append(stream, output(r3));
}

void build_map_unmap(Seq_T stream)
{
        append(stream, loadval(r0, 4));
        append(stream, map(r3, r0));
        append(stream, loadval(r1, 1));
        append(stream, map(r3, r0));
        append(stream, loadval(r2, 2));
        append(stream, loadval(r3, 3));
        append(stream, map(r4, r0));
        append(stream, unmap(r1));
        append(stream, unmap(r2));
        append(stream, unmap(r3));
        append(stream, halt());
}

void build_load_program(Seq_T stream)
{
        append(stream, loadval(r0, 2));
        append(stream, loadval(r1, 0));
        append(stream, loadval(r2, 5));
        append(stream, loadval(r7, 6));
        append(stream, loadval(r6, 12));
        append(stream, map(r3, r0));
        append(stream, sload(r4, r1, r2));
        append(stream, sload(r5, r1, r2));
        append(stream, sstore(r6, r1, r4));
        append(stream, sstore(r6, r6, r5));
        append(stream, loadval(r0, 88));
        append(stream, loadp(r6, r1));
        append(stream, halt());
        append(stream, output(r0));
}

void build_read_nand(Seq_T stream)
{
        append(stream, input(r0));
        append(stream, loadval(r1, 15));
        append(stream, loadval(r2, 48));
        append(stream, nand(r3, r0, r1));
        append(stream, add(r4, r3, r2));
        append(stream, output(r4));
        append(stream, halt());
}

void build_map_unmap_loop(Seq_T stream)
{
        append(stream, loadval(r0, 15));
        append(stream, loadval(r1, 1));
        append(stream, loadval(r7, 48));
        for (int i = 0; i < 500000; i++) {
                append(stream, map(r2, r0));
                append(stream, unmap(r2));
        }
        append(stream, output(r7));
        append(stream, halt());
}

void build_bitwise_nand_test(Seq_T stream)
{
    append(stream, loadval(r1, 0));
    append(stream, loadval(r2, 1));
    append(stream, nand(r3, r1, r2));
    append(stream, output(r3));
    append(stream, halt());
}

void build_input_handling_test(Seq_T stream)
{
    append(stream, input(r1));
    append(stream, output(r1));
    append(stream, halt());
}

void build_unconditional_jump_test(Seq_T stream)
{
    append(stream, loadval(r1, 1));
    append(stream, loadval(r2, 0));
    append(stream, jump(r1, r2));
}

void build_map_unmap_large(Seq_T stream)
{
        append(stream, loadval(r0, 15));
        append(stream, loadval(r6, 5));
        append(stream, loadval(r1, 1));
        for (int j = 0; j < 5; j++)
        {
                for (int i = 0; i < 500; i++)
                {
                        append(stream, map(r2, r6));
                }
                for (int i = 1; i < 500; i++)
                {
                        append(stream, loadval(r1, i));
                        append(stream, unmap(r1));
                }
        }
        append(stream, halt());
}

/* invalid tests */
void build_execute_unmapped_segment(Seq_T stream)
{
    append(stream, loadval(r0, 1));
    append(stream, loadval(r1, 0));
    append(stream, jump(r0, r1));
    // This should fail or halt the machine if segment 1 is unmapped
    append(stream, halt());
}

void build_out_of_bounds_access(Seq_T stream)
{
    append(stream, loadval(r0, 2));
    append(stream, alloc(r1, r0));
    append(stream, loadval(r2, 3));
    append(stream, sload(r3, r1, r2));
    append(stream, halt());
}

void build_div_nonzero_by_zero(Seq_T stream)
{
    append(stream, loadval(r0, 123));
    append(stream, loadval(r1, 0));
    append(stream, divide(r2, r0, r1));
    append(stream, halt());
}

void build_invalid_instruction(Seq_T stream)
{
    append(stream, loadval(r0, 0xFFFFFFFF));
    append(stream, (Instruction) r0);
    append(stream, halt());
}

void build_dealloc_and_access(Seq_T stream)
{
    append(stream, loadval(r0, 3));
    append(stream, alloc(r1, r0));
    append(stream, free(r1));
    append(stream, sload(r2, r1, 0));
    // Attempt to load from the deallocated segment
    append(stream, halt());
}

extern void build_invalid_load_seg(Seq_T stream)
{
        append(stream, loadval(r0, 0));
        append(stream, loadval(r1, 69));
        append(stream, sload(r7, r0, r1));
        append(stream, halt());
}

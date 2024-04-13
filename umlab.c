/*
 * umlab.c
 * Edited by Spencer Rose (srose05) and Cheng Li (cli31)
 * HW6: UM
 * Last Edited: 4/12/24
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.
 *
 * Any additional functions and unit tests written for the lab go
 * here.
 *
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
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

typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc) {
        /* assert(!(op & ~15));
        assert(!(ra & ~8 ));
        assert(!(rb & ~8 ));
        assert(!(rc & ~8 )); don't need to do parameter check */
        op = op << 28;
        ra = ra << 6 ;
        rb = rb << 3 ;
        return op | ra | rb | rc;
}
Um_instruction loadval(unsigned ra, unsigned val) {
        /* assert(!(ra & ~8 ));
        assert(!(ra & (uint32_t)(-1) << 25)); */
        return 13 << 28 | ra << 25 | val;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void)
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c)
{
        return three_register(ADD, a, b, c);
}

Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction divide(Um_register a, Um_register b,
                                    Um_register c)
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction multiply(Um_register a, Um_register b,
                                    Um_register c)
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction c_move(Um_register a, Um_register b,
                                    Um_register c)
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b,
                                    Um_register c)
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction map(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction sstore(Um_register a, Um_register b,
                                    Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction sload(Um_register a, Um_register b,
                                    Um_register c)
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction unmap(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction loadp(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }

}


/* Unit tests for the UM */

void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}

void build_addition_test(Seq_T stream)
{
        append(stream, add(r1, r2, r3));
        append(stream, halt());
}

void build_print_a_digit_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6 ));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

/* UM TESTS START HERE */

void build_addition_test1(Seq_T stream)
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

void build_cmove_move(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 69));
        append(stream, loadval(r3, 5));
        append(stream, c_move(r1, r2, r3));
        append(stream, output(r1));
        append(stream, output(r2));
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
        append(stream, loadval(r1, 200));
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
        append(stream, loadval(r6, 1));
        append(stream, map(r3, r0));
        append(stream, sload(r4, r1, r2));
        append(stream, sload(r5, r1, r2));
        append(stream, sstore(r6, r1, r4));
        append(stream, sstore(r6, r6, r5));
        append(stream, loadval(r0, 88));
        append(stream, loadp(r6, r1));
        append(stream, output(r0));
        append(stream, halt());
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
        for (int i = 0; i < 500000; i++)
        {
                append(stream, map(r2, r0));
                append(stream, unmap(r2));
        }
        append(stream, output(r7));
        append(stream, halt());
}

void build_input_handling_test(Seq_T stream)
{
        append(stream, input(r1));
        append(stream, output(r1));
        append(stream, halt());
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

void build_map_unmap_large2(Seq_T stream)
{
        append(stream, loadval(r0, 15));
        append(stream, loadval(r6, 5));
        append(stream, loadval(r1, 1));
        for (int j = 0; j < 5; j++) {
                for (int i = 0; i < 500; i++) {
                        append(stream, map(r2, r6));
                }
                for (int i = 1; i < 500; i++) {
                        append(stream, loadval(r1, i));
                        append(stream, unmap(r1));
                }
        }
        append(stream, halt());
}
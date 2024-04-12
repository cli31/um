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

/********** segmented_load **********
 * Loads a word from a specified segment and offset into a register.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance executing this operation.
 *      Um_register ra: The register to store the loaded word.
 *      Um_register rb: The register containing the identifier of the segment 
 *      to load from.
 *      Um_register rc: The register containing the offset within the segment 
 *      from where to load the word.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um is not NULL and points to a valid UM instance.
 *      - ra, rb, and rc are valid register indices within the bounds of the 
 *        UM's register array.
 *      - The offset specified in register rc is within the bounds of the 
 *        identified segment.
 *
 * Effects:
 *      - Retrieves the word at the specified offset within the segment 
 *        identified by register rb.
 *      - Stores the retrieved word in register ra.
 *      - Asserts ensure that the operation is valid, including segment 
 *        existence, mapping status, and offset bounds.
 ******************************/
void segmented_load(UM *um, Um_register ra, Um_register rb, Um_register rc)
{
    /* retrieve the segment with id $r[b] */
    uint32_t *curr_seg = (uint32_t *)Seq_get(um->segs.mem_space, um->r[rb]);
    assert(curr_seg); /* check if out of bound */
    assert(curr_seg[0] != 0); /* check if the id is unmapped */
    assert(um->r[rc] < curr_seg[0]); /* check in the range */
    um->r[ra] = curr_seg[rc + 1]; /* size takes the 0 idx */ 
}

/********** segmented_store **********
 * Stores a word from a register into a specified segment and offset.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance executing this operation.
 *      Um_register ra: The register containing the identifier of the segment 
 *      to store into.
 *      Um_register rb: The register containing the offset within the segment 
 *      where to store the word.
 *      Um_register rc: The register containing the word to store.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um is not NULL and points to a valid UM instance.
 *      - ra, rb, and rc are valid register indices within the bounds of the 
 *        UM's register array.
 *      - The offset specified in register rb is within the bounds of the i
 *        identified segment.
 *
 * Effects:
 *      - Stores the word from register rc at the specified offset within the 
 *        segment identified by register ra.
 *      - Asserts ensure that the operation is valid, including segment 
 *        existence, mapping status, and offset bounds.
 ******************************/
void segmented_store(UM *um, Um_register ra, Um_register rb, Um_register rc)
{
    uint32_t *curr_seg = (uint32_t *)Seq_get(um->segs.mem_space, um->r[ra]);
    assert(curr_seg); /* check if out of bound */
    assert(curr_seg[0] != 0);
    assert(um->r[rb] < curr_seg[0]); /* check in the range */
    curr_seg[rb + 1] = um->r[rc];
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
        um->r[ra] = (uint32_t)(((uint64_t)um->r[rb] * (uint64_t)um->r[rc]) 
                              % ((uint64_t)1 << 32));
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
            /* Division by zero error handling. */
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

/* opcode 7
* halt not needed, just free memory and return 
 */

/********** map_segment **********
 * Maps a new memory segment in the Universal Machine's memory with a specified 
 * number of words, each initialized to 0.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance.
 *      Um_register b: The register to store the newly mapped segment's 
 *      identifier.
 *      Um_register c: The register containing the size (number of words) of 
 *      the new segment to map.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um is not NULL and points to a valid UM instance with initialized 
 *        memory management structures.
 *      - b and c are valid register indices within the bounds of the UM's 
 *        register array.
 *      - The value in register c represents a non-negative size for the new 
 *        memory segment.
 *
 * Effects:
 *      - Allocates a new memory segment of the specified size, with each word 
 *        initialized to 0.
 *      - Stores the new segment's identifier in register b.
 *      - Updates UM's memory management structures to include the new segment.
 ******************************/
void map_segment(UM *um, Um_register b, Um_register c) 
{
    uint32_t *seg;
    int id = 0; /* invalid */
    /* if there is no available id, alloc a new memory segment */
    if (Seq_length(um->segs.unmapped_ids) == 0) {
        seg = (uint32_t *)malloc(sizeof(uint32_t) * (um->r[c] + 1));
    }
    else {
        /* use from high end like a stack */
        int id = *(int *)Seq_remhi(um->segs.unmapped_ids);
        seg = (uint32_t *)Seq_get(um->segs.mem_space, id);
        seg = (uint32_t *)realloc(seg, sizeof(uint32_t) * (um->r[c] + 1));
    }
    seg[0] = um->r[c]; /* set the first word as size */
    /* initialize each word to 0 */
    for (uint32_t i = 1; i < um->r[c] + 1; i++) {
        seg[i] = 0;
    }

    if (id == 0) { /* if id is new, push to the high end */
        Seq_addhi(um->segs.mem_space, (void *)(uintptr_t)seg);
        um->r[b] = Seq_length(um->segs.mem_space) - 1; /* actual id */
        assert(um->r[b] != 0); /* fail mode: bit pattern that's not all 0s */
    }
    else { /* if id is recycled, update mem_space and pop from ids */
        Seq_put(um->segs.mem_space, id, (void *)(uintptr_t)seg);
        um->r[b] = id; 
        Seq_remhi(um->segs.unmapped_ids);
    }
}

/********** unmap_segment **********
 * Unmaps (deallocates) a previously mapped segment in the Universal Machine's 
 * memory, making its identifier reusable.
 *
 * Parameters:
 *      UM *um: The Universal Machine instance.
 *      Um_register c: The register containing the id of the segment to unmap.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um is not NULL and points to a valid UM instance with initialized 
 *        memory management structures.
 *      - c is a valid register index within the bounds of the UM's register 
 *        array.
 *      - The value in register c is not 0, as unmapping the 0th segment is 
 *        disallowed.
 *      - The segment identified by the value in register c exists and is 
 *        currently mapped.
 *
 * Effects:
 *      - Marks the segment identified by the value in register c as unmapped 
 *        by setting its size indicator to 0.
 *      - Adds the segment's identifier to the list of reusable identifiers, 
 *        allowing future segments to be mapped using this identifier.
 ******************************/
void unmap_segment(UM *um, Um_register c) 
{
    /* unmapping $m[0] is not allowed */
    assert(um->r[c] != 0); 

    uint32_t *seg = (uint32_t *)Seq_get(um->segs.mem_space, um->r[c]);
    assert(seg); /* check out of bound */
    assert(seg[0] != 0);
    /* assign an invalid indicator (size 0) to unmapped segment 
       so when checking if an id is unmapped, we don't need to iterate through
       unmapped_ids */
    seg[0] = 0;

    /* push the id to reusable seq */
    Seq_addhi(um->segs.unmapped_ids, (void *)(uintptr_t)um->r[c]);
    /* does not free memeory so that the address can be reused by realloc */
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
 *        stores UINT32_MAX in register c; otherwise, stores the ASCII value.
 ******************************/
void input(UM *um, Um_register c)
{
    int input = fgetc(stdin);
    if (input != EOF) {
        assert(0 <= input && input <= 255);
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
        um->counter = um->r[c];
        return; /* do nothing == extremely quick */
    }
    /* get $m($r[b]) */
    uint32_t *this_seg = (uint32_t *)Seq_get(um->segs.mem_space, um->r[b]);
    assert(this_seg);
    assert(this_seg[0] != 0); /* check if unmapped */
    /* free $m[0] */
    free(Seq_get(um->segs.mem_space, 0));
    /* duplicate */
    uint32_t size = this_seg[0];
    uint32_t *new_seg = (uint32_t *)malloc(sizeof(uint32_t) * (size + 1));
    memcpy(new_seg, this_seg, sizeof(uint32_t) * (size + 1));
    /* the duplicate replace $m[0] */
    Seq_put(um->segs.mem_space, 0, (void *)(uintptr_t)new_seg);
    /* update the counter to $r[c] */
    assert(um->counter = um->r[c] <= size);
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
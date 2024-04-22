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
#ifndef UM_OPERATIONS_H
#define UM_OPERATIONS_H

#include "um_components.h"

/********** Um_instruction **********
 * Represents an instruction in the Universal Machine, including opcode and
 * operand(s).
 *
 * Members:
 *      Um_opcode opcode: The operation code indicating the instruction type.
 *      Um_register ra, rb, rc: Operand registers, usage varies by instruction.
 *      uint32_t value: Immediate value for certain instructions (e.g. Load)
 *
 * Description:
 *      This structure encapsulates a single instruction's data, including the
 *      operation to perform and the operands on which to perform it. It is
 *      parsed to execute the corresponding action in the UM.
 ******************************/
// void conditional_move(UM *um, Um_register ra, Um_register rb, Um_register rc);
void segmented_load(UM *um, Um_register ra, Um_register rb, 
                                                   Um_register rc);
void segmented_store(UM *um, Um_register ra, Um_register rb, 
                                                    Um_register rc);
// void addition(UM *um, Um_register ra, Um_register rb, Um_register rc);
// void multiplication(UM *um, Um_register ra, Um_register rb, Um_register rc);
// void division(UM *um, Um_register ra, Um_register rb, Um_register rc);
// void bitwise_NAND(UM *um, Um_register a, Um_register b, Um_register c);
void map_segment(UM *um, Um_register b, Um_register c);
void unmap_segment(UM *um, Um_register c);
//void output(UM *um, Um_register c);
//void input(UM *um, Um_register c);
void load_program(UM *um, Um_register b, Um_register c);
//void load_value(UM *um, Um_register a, uint32_t value);

#endif
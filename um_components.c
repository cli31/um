/******************************************************************
 * um_components.c
 * Authors: Spencer Rose (srose05) and Cheng Li (cli31)
 * Date last edited: 4/12/24
 *
 *
 *  CS 40 HW 6: UM
 * Description:
 * This source file implements utility functions for the Universal Machine (UM),
 * including instruction parsing, machine initialization, machine state
 * printing, and resource deallocation. These utilities support the core
 * functionality of the UM by enabling instruction execution, managing memory
 * and registers, and facilitating debugging and clean shutdown.
 *
 * The file depends on "um_components.h" for structure definitions and constants,
 * ensuring a modular and maintainable implementation of the UM.
 ******************************************************************/
#include "um_components.h"

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
void parse_inst(Um_instruction *curr_inst, Um_opcode *opcode, Um_register *ra,
                                           Um_register *rb, Um_register *rc)
{
    *opcode = (*curr_inst) >> 28;
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

/********** initialize_UM **********
 * Initializes um by setting all registers to 0, setting up memory management
 * structures, and initializing the program counter.
 *
 * Parameters:
 *      UM *um: Pointer to the UM instance to initialize.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um is not NULL and points to a valid, but uninitialized, UM structure.
 *
 * Effects:
 *      - Resets all UM registers to 0.
 *      - Initializes the program counter to 1, acknowledging that index 0 in
 *        each segment is reserved for the segment size.
 *      - Allocates and initializes the memory space and unmapped ID sequences
 *        to manage memory segmentation and reclamation.
 ******************************/
void initialize_UM(UM *um)
{
    for (int i = 0; i < num_of_regs; i++) {
        um->r[i] = 0;
    }
    um->counter = 1; /* since idx 0 is always the size */
    um->segs.mem_space    = Seq_new(HINT);
    um->segs.unmapped_ids = Seq_new(HINT);
}

void print_um(const UM *um)
{
    fprintf(stderr, "counter = %d\n", um->counter);
    for (int i = 0; i < num_of_regs; i++) {
        fprintf(stderr, "r[%d] = %d\t", i, um->r[i]);
    }

    fprintf(stderr, "\nsegmented memory:\n");
    /* Iterate over the Seq_T */
    int length = Seq_length(um->segs.mem_space);
    for (int i = 0; i < length; i++) {
        fprintf(stderr, "id = %d,\t", i);
        uint32_t *curr_seg = Seq_get(um->segs.mem_space, i);
        unsigned size = curr_seg[0];
        fprintf(stderr, "number of inst = %u:\n", size);
        for (unsigned j = 1; j <= size; j++) {
            uint32_t curr_inst = curr_seg[j];
            Um_opcode opcode = 14;
            Um_register ra = 8, rb = 8, rc = 8;
            parse_inst(&curr_inst, &opcode, &ra, &rb, &rc);
            if (opcode == LV) {
                fprintf(stderr, "opcode = %d,\tra = %d,\tvalue = %ld\n", opcode,
                ra, Bitpack_getu(curr_inst, 25, 0));
            }
            else {
                fprintf(stderr, "opcode = %d,\tra = %d,\trb = %d,\trc = %d\n",
                        opcode, ra, rb, rc);
            }
        }
        fprintf(stderr, "\n**********next segment**********\n");
    }

    fprintf(stderr, "\nunmapped ids:\n");
    length = Seq_length(um->segs.unmapped_ids);
    for (int i = 0; i < length; i++) {
        fprintf(stderr, "%d,\t", *(int *)Seq_get(um->segs.unmapped_ids, i));
    }
    fprintf(stderr, "\num printed done \n\n");
}

/********** free_um **********
 * Frees all allocated memory associated with a Universal Machine (UM)
 * instance, including memory for all segments and management structures.
 *
 * Parameters:
 *      UM *um: The UM instance to be freed.
 *
 * Return:
 *      void
 *
 * Expects:
 *      - um is not NULL and points to a valid UM instance that has been
 *        previously initialized.
 *
 * Effects:
 *      - Frees each memory segment stored within the UM's memory space.
 *      - Frees the sequences that manage the memory space and the sequence of
 *        unmapped segment identifiers.
 *      - After executing this function, the UM instance and its memory
 *        structures are no longer valid and should not be used.
 ******************************/
void free_um(UM *um)
{
    int length = Seq_length(um->segs.mem_space);
    for (int i = 0; i < length; i++) {
        free(Seq_get(um->segs.mem_space, i));
    }

    Seq_free(&um->segs.mem_space);
    Seq_free(&um->segs.unmapped_ids);
}

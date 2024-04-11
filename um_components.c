#include "um_components.h"

int intcmp(const void *x, const void *y) {
    /* note: when the difference is > 2^31, typecast to int will be +ve */
    /* to avoid it, we compare instead of doing subtraction */
    if (*(uint32_t *)x < *(uint32_t *)y) {
        return -1;
    }
    else if (*(uint32_t *)x > *(uint32_t *)y) {
        return 1;
    }
    else {
        return 0;
    }
}

unsigned hash(const void *x) {
    /* key is always a uint32_t id, and Hanson table request an unsigned return.
    We can simply cast to make them onto */
    return (unsigned)*(uint32_t *)x;
}

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

extern void initialize_UM(UM *um) {
    for (int i = 0; i < num_of_regs; i++) {
        um->r[i] = 0;
    }
    um->counter = 0;
    um->segs.mem_space    = Table_new(HINT, intcmp, hash);
    um->segs.unmapped_ids = Seq_new  (HINT); 
}

void print_um(const UM *um) {
    fprintf(stderr, "counter = %d\n", um->counter);
    for (int i = 0; i < num_of_regs; i++) {
        fprintf(stderr, "r[%d] = %d\t", i, um->r[i]);
    }

    fprintf(stderr, "\nsegmented memory:\n");
    void **iterator = Table_toArray(um->segs.mem_space, NULL);
    /* Iterate over the array until the end marker is encountered */
    while (*iterator != NULL) {
        uint32_t *key = (uint32_t *)*iterator++;
        uint32_t **value = (uint32_t **)*iterator++;

        // Print the key-value pair
        fprintf(stderr, "id: %d,\t", *key);
        uint32_t size = (*value)[0];
        fprintf(stderr, "number of inst: %d,\t", size);
        for (uint32_t i = 1; i < size + 1; i++) {
            uint32_t curr_inst = (*value)[i];
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
            fprintf(stderr, "*****next word*****\n");
        }
        fprintf(stderr, "**********next segment**********\n\n");
    }
}

void free_um(UM *um) {
    Table_free(&um->segs.mem_space);
    Seq_free  (&um->segs.unmapped_ids);
}


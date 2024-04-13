/* um 
 * CS 40 Assignment 6
 * Made by Spencer Rose (srose05) and Cheng Li (cli31)
 */
#include "um_operations.h"

/* struct UM is in um_operations.h */

int main(int argc, char *argv[]) 
{
        /* Usage: ./um [file.um] */
        if (argc != 2) {
        fprintf(stderr, "Usage: ./um [file.um]\n");
        exit(EXIT_FAILURE);
}
        FILE *fp = fopen(argv[1], "rb");
        if (!fp) {
        fprintf(stderr, 
                "Error: Cannot open file '%s' for reading.\n", 
                argv[argc - 1]);
        exit(EXIT_FAILURE);
}
        /* template done, code starts here */
        /* it is an oop idea, code inside of this block is equivalent to 
        um.run() */
        /* Step1: read in instructions from fp */
        /* get the size of file and see if it has complete instructions */
        fseek(fp, 0, SEEK_END);
        uint32_t num_of_inst = ftell(fp);
        assert(num_of_inst % 4 == 0);
        num_of_inst /= 4;
        fseek(fp, 0, SEEK_SET);        
        uint32_t *buffer = (uint32_t *)malloc(sizeof(uint32_t) * 
                                        (num_of_inst + 1));
        if (!buffer) {
                fprintf(stderr, "Error: Memory reallocation failed.\n");
                exit(EXIT_FAILURE);
        }
        buffer[0] = num_of_inst;
        for (uint32_t i = 1; i <= num_of_inst; i++) {
                uint32_t a, b, c, d;
                a = getc(fp);
                b = getc(fp);
                c = getc(fp);
                d = getc(fp);
                buffer[i] = (a << 24) | (b << 16) | (c << 8) | d;
        }
        fclose(fp);
        /* Step2: initialize um */
        UM um;
        initialize_UM(&um);        
        /* check $m[0] is first loaded with program */
        assert(um.segs.mem_space);
        assert(um.segs.unmapped_ids);
        Seq_addhi(um.segs.mem_space, buffer);        
        /* Step3: execution cycle */
        for ( ;; um.counter++) {
                /* use buffer instead of Seq_get for a simpler expression */
                /* reset of program ptr was done in load program block */
                Um_instruction curr_inst = buffer[um.counter];
                /* opcode from 0-13, check for valid inst */
                Um_opcode opcode = 14;
                Um_register ra = 8, rb = 8, rc = 8; /* reg from 0-7 */
                parse_inst(&curr_inst, &opcode, &ra, &rb, &rc);
                switch (opcode) {
                case CMOV:
                            conditional_move(&um, ra, rb, rc);
                        break;
                case SLOAD:
                        segmented_load(&um, ra, rb, rc);
                        break;
                case SSTORE:
                        segmented_store(&um, ra, rb, rc);
                        break;
                case ADD:
                        addition(&um, ra, rb, rc);
                        break;
                case MUL:
                        multiplication(&um, ra, rb, rc);
                        break;
                case DIV:
                        division(&um, ra, rb, rc);
                        break;
                case NAND:
                        bitwise_NAND(&um, ra, rb, rc);
                        break;
                case HALT:  
                        /* free everything */
                        free_um(&um);
                        return 0;
                case ACTIVATE:
                        map_segment(&um, rb, rc);
                        break;
                case INACTIVATE:
                        unmap_segment(&um, rc);
                        break;
                case OUT:
                        output(&um, rc);
                        break;
                case IN:
                        input(&um, rc);
                        break;
                case LOADP: {
                        /* since in the beginning of func cycle, we chose to 
                        use buffer instead of running Seq_get $m[0], we need 
                        to update buffer and num_of_inst after $m[0] was 
                        replaced in load program */
                        load_program(&um, rb, rc);
                        /* the old $m[0] should've been properly freed */
                        buffer = (uint32_t *)Seq_get(um.segs.mem_space, 0);
                        break;
                }
                case LV: {
                        uint32_t value = (curr_inst << 7) >> 7;
                        load_value(&um, ra, value);
                        break;
                }
                default:
                        assert(false);
                        break;
                }
        }
        free_um(&um);
        return 0;
}
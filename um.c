/* um 
 * CS 40 Assignment 6
 * Made by Spencer Rose (srose05) and Cheng Li (cli31)
 */
#include "um_operations.h"

#define N (curr_inst)
#define opcode (N >> 28)
#define A (((N) >> 6) & 0x7)
#define B (((N) >> 3) & 0x7)
#define C ((N)        & 0x7)
#define RA (um.r[A])
#define RB (um.r[B])
#define RC (um.r[C])
#define value ((N << 7) >> 7)
#define Rvalue (um.r[(N >> 25) & 0x7])

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
        Um_instruction curr_inst;
        for ( ;; um.counter++) {
                /* use buffer instead of Seq_get for a simpler expression */
                /* reset of program ptr was done in load program block */
                curr_inst = buffer[um.counter];
                switch (opcode) {
                case CMOV:
                        if (RC != 0) {
                                RA = RB;
                        }
                        //conditional_move(&um, ra, rb, rc);
                        break;
                case SLOAD:
                        segmented_load(&um, A, B, C);
                        break;
                case SSTORE:
                        segmented_store(&um, A, B, C);
                        break;
                case ADD:
                        RA = RB + RC;
                        //addition(&um, ra, rb, rc);
                        break;
                case MUL:
                        RA = RB * RC;
                        //multiplication(&um, ra, rb, rc);
                        break;
                case DIV:
                        RA = RB / RC;
                        //division(&um, ra, rb, rc);
                        break;
                case NAND:
                        RA = ~(RB & RC);
                        //bitwise_NAND(&um, ra, rb, rc);
                        break;
                case HALT:  
                        /* free everything */
                        free_um(&um);
                        return 0;
                case ACTIVATE:
                        map_segment(&um, B, C);
                        break;
                case INACTIVATE:
                        unmap_segment(&um, C);
                        break;
                case OUT:
                        assert (RC <= 255);
                        putchar((char)RC);
                        //output(&um, rc);
                        break;
                case IN: {
                        int input = fgetc(stdin);
                        if (input != EOF) {
                                assert(0 <= input && input <= 255);
                                RC = input;
                        } else {
                                RC = -1;
                        }
                        //input(&um, rc);
                        break;
                }
                case LOADP: {
                        /* since in the beginning of func cycle, we chose to 
                        use buffer instead of running Seq_get $m[0], we need 
                        to update buffer and num_of_inst after $m[0] was 
                        replaced in load program */
                        load_program(&um, B, C);
                        /* the old $m[0] should've been properly freed */
                        buffer = (uint32_t *)Seq_get(um.segs.mem_space, 0);
                        break;
                }
                case LV: {
                        Rvalue = value;
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
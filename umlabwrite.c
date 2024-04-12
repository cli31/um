#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void build_halt_test(Seq_T instructions);
extern void build_verbose_halt_test(Seq_T instructions);
extern void build_addition_test(Seq_T instructions);
extern void build_print_a_digit_test(Seq_T instructions);
//void build_addition_test1(Seq_T stream);
void build_digit_print(Seq_T stream);
void build_cmove_move(Seq_T stream);
void build_simple_print(Seq_T stream);
void build_divide(Seq_T stream);
void build_multiply(Seq_T stream);
void build_multiply_divide(Seq_T stream);
void build_nand(Seq_T stream);
void build_in_out(Seq_T stream);
void build_sstore_then_load(Seq_T stream);
void build_map_unmap(Seq_T stream);
void build_load_program(Seq_T stream);
void build_read_nand(Seq_T stream);
void build_map_unmap_loop(Seq_T stream);
void build_bitwise_nand_test(Seq_T stream);
void build_input_handling_test(Seq_T stream);
void build_map_unmap_large(Seq_T stream);
/* invalid tests start here */
void build_unmap_0seg(Seq_T stream);
void build_div_0(Seq_T stream);
void build_invalid_load(Seq_T stream);
extern void build_invalid_loadseg(Seq_T stream);
extern void build_map_invalid_storesegment(Seq_T stream);

    /* The array `tests` contains all unit tests for the lab. */

    static struct test_info
{
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*build_test)(Seq_T stream);
} tests[] = {
    {"halt", NULL, "", build_halt_test},
    {"halt-verbose", NULL, "", build_verbose_halt_test},
    {"add", NULL, "", build_addition_test},
    {"print-six", NULL, "", build_print_a_digit_test},
    //{"build_add_test", NULL, build_addition_test1},
    {"build_digit_print", NULL, "", build_digit_print},
    {"build_cmove_move", NULL, "", build_cmove_move},
    {"build_simple_print", NULL, "", build_simple_print},
    {"build_divide", NULL, "", build_divide},
    {"build_multiply", NULL, "", build_multiply},
    {"build_multiply_divide", NULL, "", build_multiply_divide},
    {"build_nand", NULL, "", build_nand},
    {"build_in_out", NULL, "", build_in_out},
    {"build_sstore_then_load", NULL, "", build_sstore_then_load},
    {"build_map_unmap", NULL, "", build_map_unmap},
    {"build_load_program", NULL, "", build_load_program},
    {"build_read_nand", NULL, "", build_read_nand},
    {"build_map_unmap_loop", NULL, "", build_map_unmap_loop},
    {"build_bitwise_nand_test", NULL, "", build_bitwise_nand_test},
    {"build_input_handling_test", NULL, "", build_input_handling_test},
    {"build_map_unmap_large", NULL, "", build_map_unmap_large},
    /* invalid tests start here*/
    {"build_unmap_0seg", NULL, "", build_unmap_0seg},
    {"build_div_0", NULL, "", build_div_0},
    {"build_invalid_load", NULL, "", build_invalid_load},
    {"build_invalid_loadseg", NULL, "", build_invalid_loadseg},
    {"build_map_invalid_storesegment", NULL,"", build_map_invalid_storesegment},
    {"build_map_unmap_large", NULL, "", build_map_unmap_large}
};

#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path',
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}


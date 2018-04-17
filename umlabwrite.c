/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 6 - UM
  * umlabwrite.c
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void emit_halt_test(Seq_T instructions);
extern void emit_verbose_halt_test(Seq_T instructions);
extern void emit_add_test(Seq_T instructions);
extern void emit_print_six_test(Seq_T instructions);
extern void emit_io_test(Seq_T instructions);
extern void emit_loadval_test(Seq_T instructions);
extern void emit_multiply_test(Seq_T instructions);
extern void emit_divide_test(Seq_T instructions);
extern void emit_nand_test(Seq_T instructions);
extern void emit_move_test(Seq_T instructions);
extern void emit_map_test(Seq_T instructions);
extern void emit_unmap_test(Seq_T instructions);
extern void emit_loadprog_test(Seq_T instructions);
extern void emit_segload_test(Seq_T instructions);
extern void emit_segstore_test(Seq_T instructions);
extern void emit_test(Seq_T instructions);
extern void emit_test2(Seq_T instructions);

/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*emit_test)(Seq_T stream);
} tests[] = {
        { "halt",         NULL, "",   emit_halt_test },
        { "halt-verbose", NULL, "",   emit_verbose_halt_test },
        { "add",          NULL, "b",  emit_add_test },
        { "print-six",    NULL, "6",  emit_print_six_test },
        { "io",           "a",  "a",  emit_io_test },
        { "load-val",     NULL, "!",  emit_loadval_test },        
        { "multiply",     NULL, "!",  emit_multiply_test },
        { "divide",       NULL, "a",  emit_divide_test },
        { "nand",         NULL, "!",  emit_nand_test },
        { "move",         NULL, "bb", emit_move_test },
        { "map",          NULL, "!",  emit_map_test },
        { "unmap",        NULL, "!",  emit_unmap_test },
        { "seg-load",     NULL, "!",  emit_segload_test },
        { "seg-store",    NULL, "a",  emit_segstore_test },
        { "load-prog",    NULL, "a",  emit_loadprog_test },
        { "test",         NULL, "",   emit_test },
        { "test2",        NULL, "",   emit_test2 }
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
        test->emit_test(instructions);
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

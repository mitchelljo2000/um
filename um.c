/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 7 - Profile
  * um.c
  *
  * Reads 32 bit words in a file provided as command line argument and
  * stores them in segment 0. Executes specific instructions based on the 
  * opcode in the 32 bit words.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include "bitpack_opt.h"
#include "instructions.h"

#define REG_SIZE 8

// Function prototypes
void read_file(memory mem, const char *path);
void run_program(memory mem, uint32_t *registers);
void execute_instr(uint32_t instr, bool *is_done, 
                        uint32_t *count, memory mem, uint32_t *registers);

int main(int argc, char *argv[])
{
        if (argc != 2) {
                fprintf(stderr, "usage: ./um program.um\n");
                return EXIT_FAILURE;
        }

        memory mem = create_memory(100);
        read_file(mem, argv[1]);

        uint32_t registers[REG_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
        run_program(mem, registers);
}

/*
 * read_file takes a memory struct and a file name and reads the 32 bit words
 * int the file and stores them in a segment which is then stored at index
 * zero in the sequence representing memory
 *
 * Parameters:
 *      mem: The memory struct
 *      path: The file name of the file to read
 */
void read_file(memory mem, const char *path)
{
        FILE *fp = fopen(path, "rb");
        assert(fp != NULL);

        struct stat fileStat;
        stat(path, &fileStat);

        int seg_size = fileStat.st_size / 4;
        segment seg = segment_new(seg_size);

        for (int i = 0; i < seg_size; i++) {
                uint32_t word = 0;
                word = Bitpack_newu(word, 8, 24, fgetc(fp));
                word = Bitpack_newu(word, 8, 16, fgetc(fp));
                word = Bitpack_newu(word, 8, 8, fgetc(fp));
                word = Bitpack_newu(word, 8, 0, fgetc(fp));
                add_word(seg, word, i);
        }

        fclose(fp);

        add_segment(seg, mem);
}

/*
 * run_program takes a memory struct and array of uint32_t integers
 * representing registers and runs the universal machine program
 *
 * Parameters:
 *      mem: The memory struct
 *      registers: The array of integers representing registers
 */
void run_program(memory mem, uint32_t *registers)
{
        bool done = false;
        uint32_t count = 0;

        while (!done) {
                uint32_t word = get_word(get_segment(0, mem), count);

                execute_instr(word, &done, &count, mem, registers);

                if (count == (uint32_t) get_size(get_segment(0, mem))) {
                        done = true;
                }
                
                count++;
        }
        free_memory(mem);
}

/*
 * execute_instr takes an 32 bit word, a boolean, a struct representing
 * segment zero, an integer representing what the current word in segment
 * zero is, a memory struct, and an array of uint32_t integers representing
 * registers and executes a specific instruction based on the opcode in the
 * 32 bit word
 *
 * Parameters:
 *      instr: The 32 bit word
 *      is_done: A boolean that is true only when the program should end
 *      seg_zero: The segment struct representing segment zero
 *      count: The current word in segment zero
 *      mem: The memory struct
 *      registers: The array of uint32_t integers representing registers
 */
void execute_instr(uint32_t instr, bool *is_done, 
                        uint32_t *count, memory mem, uint32_t *registers)
{
        int opcode = Bitpack_getu(instr, 4, 28);
        
        switch(opcode) {
                case 0:  move(instr, registers);
                         break;
                case 1:  load_segment(instr, registers, mem);
                         break;
                case 2:  store_segment(instr, registers, mem);
                         break;
                case 3:  add(instr, registers);
                         break;
                case 4:  multiply(instr, registers);
                         break;
                case 5:  divide(instr, registers);
                         break;
                case 6:  nand(instr, registers);
                         break;
                case 7:  *is_done = true;
                         break;
                case 8:  map(instr, registers, mem);
                         break;
                case 9:  unmap(instr, registers, mem);
                         break;
                case 10: output(instr, registers);
                         break;
                case 11: input(instr, registers);
                         break;
                case 12: load_program(instr, registers, mem, count);
                         break;
                case 13: load_value(instr, registers);
                         break;
                default:
                         break;
        }
}

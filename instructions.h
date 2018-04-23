 /*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 6 - UM
  * instructions.h
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <stdio.h>
#include "memory.h"
#include "bitpack.h"

static inline void parse_registers(uint32_t instruction, int *a_ind, int *b_ind,
                                int *c_ind)
{
        *a_ind = Bitpack_getu(instruction, 3, 6);
        *b_ind = Bitpack_getu(instruction, 3, 3);
        *c_ind = Bitpack_getu(instruction, 3, 0);
}

void move(uint32_t instruction, uint32_t *registers);

static inline void load_segment(uint32_t instruction, uint32_t *registers, 
								memory mem)
{
        int a_ind, b_ind, c_ind;
        parse_registers(instruction, &a_ind, &b_ind, &c_ind);

        segment seg = get_segment(registers[b_ind], mem);
        registers[a_ind] = get_word(seg, registers[c_ind]);
}

static inline void store_segment(uint32_t instruction, uint32_t *registers, memory mem)
{
        int a_ind, b_ind, c_ind;
        parse_registers(instruction, &a_ind, &b_ind, &c_ind);

        segment seg = get_segment(registers[a_ind], mem);
        add_word(seg, registers[c_ind], registers[b_ind]);
}

void add(uint32_t instruction, uint32_t *registers);

void multiply(uint32_t instruction, uint32_t *registers);

void divide(uint32_t instruction, uint32_t *registers);

void nand(uint32_t instruction, uint32_t *registers);

void map(uint32_t instruction, uint32_t *registers, memory mem);

void unmap(uint32_t instruction, uint32_t *registers, memory mem);

void output(uint32_t instruction, uint32_t *registers);

void input(uint32_t instruction, uint32_t *registers);

void load_program(uint32_t instruction, uint32_t *registers,
                        memory mem, uint32_t *count);

static inline void load_value(uint32_t instruction, uint32_t *registers)
{
        int index = Bitpack_getu(instruction, 3, 25);
        registers[index] = Bitpack_getu(instruction, 25, 0);
}

#endif

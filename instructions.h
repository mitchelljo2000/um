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

void move(uint32_t instruction, uint32_t *registers);

void load_segment(uint32_t instruction, uint32_t *registers, memory mem);

void store_segment(uint32_t instruction, uint32_t *registers, memory mem);

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

void load_value(uint32_t instruction, uint32_t *registers);

#endif

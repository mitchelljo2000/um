/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 6 - UM
  * memory.h
 */

#include <stdlib.h>
#include "segments.h"
#include "seq.h"

typedef struct memory *memory;

memory create_memory(int hint);

void add_segment(segment seg, memory mem);

segment get_segment(uint32_t id, memory mem);

void rem_segment(uint32_t id, memory mem);

void free_memory(memory mem);

int get_mem_size(memory mem);

void add_id(memory mem, uint32_t id);

uint32_t remove_id(memory mem);

int get_num_unused(memory mem);

void put_segment(segment seg, memory mem, uint32_t id);

uint32_t get_curr_id(memory mem);

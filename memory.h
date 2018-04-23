/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 7 - Profile
  * memory.h
 */

#include <stdlib.h>
#include "segments.h"
#include "seq.h"

struct memory {
        segment *segments_map;
        uint32_t *unmap_id;
        uint32_t curr_id;
        uint32_t curr_id2;
        uint32_t size;
        uint32_t size2;
        uint32_t capacity;
        uint32_t capacity2;
};
typedef struct memory *memory;

memory create_memory(int hint);

void add_segment(segment seg, memory mem);

//segment get_segment(uint32_t id, memory mem);
static inline segment get_segment(uint32_t id, memory mem)
{
        return mem->segments_map[id];
}


void rem_segment(uint32_t id, memory mem);

void free_memory(memory mem);

int get_mem_size(memory mem);

void add_id(memory mem, uint32_t id);

uint32_t remove_id(memory mem);

int get_num_unused(memory mem);

void put_segment(segment seg, memory mem, uint32_t id);

uint32_t get_curr_id(memory mem);

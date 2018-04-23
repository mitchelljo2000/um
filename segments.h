/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 7 - Profile
  * segments.h
 */

#ifndef SEGMENTS_H_
#define SEGMENTS_H_

#include <stdint.h>
#include <stdlib.h>

struct segment {
        int capacity;
        int curr_index;
        uint32_t instructions[];
};
typedef struct segment *segment;

segment segment_new(int num_words);

void segment_free(segment seg);

void add_word(segment seg, uint32_t instruction, int index);

static inline uint32_t get_word(segment seg, int index)
{
        return seg->instructions[index];
}

static inline int get_size(segment seg)
{
        return seg->capacity;
}

#endif

/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 6 - UM
  * segments.h
 */

#ifndef SEGMENTS_H_
#define SEGMENTS_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct segment *segment;

segment segment_new(int num_words);

void segment_free(segment seg);

void add_word(segment seg, uint32_t instruction, int index);

uint32_t get_word(segment seg, int index);

int get_size(segment seg);

#endif

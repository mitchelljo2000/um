/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 6 - UM
  * segments.c
  *
  * Handles reading the words into a segment. Creates a segment struct, 
  * frees a segment struct or adds words to segments.
 */

#include "segments.h"

struct segment {
        int capacity;
        int curr_index;
        uint32_t instructions[];
};

/*
 * segment_new takes an integer and creates a new segment struct with an array
 * of uint32_t words that has a capacity matching the given integer
 *
 * Parameters:
 *      num_words: The integer number of words the segment should contain
 */
segment segment_new(int num_words)
{
        segment seg = malloc(sizeof(*seg) + num_words * 
                                                sizeof(*seg->instructions));
        seg->capacity = num_words;      

        for (int i = 0; i < num_words; i++) {
                seg->instructions[i] = 0;
        }

        return seg;
} 

/*
 * segment_free takes a segment struct and frees it
 *
 * Parameters:
 *      seg: The segment struct
 */
void segment_free(segment seg)
{
        free(seg);
}

/*
 * add_word takes a segment struct, a uint32_t word, and an integer index, and
 * puts the given word inside the array of instructions within the segment
 * struct at the given index
 *
 * Parameters:
 *      seg: The segment struct
 *      instruction: The uint32_t word to add to the array of instructions
 *      index: The integer index in the array to add the instruction
 */
void add_word(segment seg, uint32_t instruction, int index)
{
        seg->instructions[index] = instruction;
}

/*
 * get_word takes a segment struct and an integer index and returns the
 * uint32_t word from the array of instructions within the segment struct
 * corresponding to the index given
 *
 * Parameters:
 *      seg: The segment struct
 *      index: The integer index into the array of instructions
 */
uint32_t get_word(segment seg, int index)
{
        return seg->instructions[index];
}

/*
 * get_size takes a segment struct and returns the integer capacity of the
 * array of uint32_t words within the struct
 *
 * Parameters:
 *      seg: The segment struct
 */    
int get_size(segment seg)
{
        return seg->capacity;
}

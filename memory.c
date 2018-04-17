/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 6 - UM
  * memory.c
  *
  * Creates a struct that holds a Hanson sequence containing segments as
  * values and 32-bit segment identifiers as their indices in the sequence
 */
#include <stdio.h>
#include "memory.h"

struct memory {
        Seq_T segments_map;
        Seq_T unmap_id;
        uint32_t curr_id;
};

/*
 * create_memory takes an integer and allocates and returns a memory struct
 * representing memory. The integer is used as a guess as to the number of
 * segments memory needs to store
 *
 * Parameters:
 *      hint: An integer guess of the number of segments memory needs to store
 */
memory create_memory(int hint)
{
        memory mem = malloc(sizeof(*mem));
        mem->segments_map = Seq_new(hint);
        mem->unmap_id = Seq_new(hint);
        mem->curr_id = 0;
        return mem;
}

/*
 * add_segment takes a segment struct and a memory struct and adds the segment
 * struct to a sequence within the memory struct. The curr_id data member
 * within the memory struct is incremented so that every segment corresponds
 * to a unique segment identifier
 *
 * Parameters:
 *      seg: The segment struct
 *      mem: The memory struct
 */
void add_segment(segment seg, memory mem)
{
        Seq_addhi(mem->segments_map, (void *)seg);
        mem->curr_id++;
}

/*
 * get_segment takes a uint32_t and a memory struct and returns a segment
 * from the sequence in the memory struct indexed by the uint32_t
 *
 * Parameters:
 *      id: The uint32_t identifier
 *      mem: The memory struct
 */
segment get_segment(uint32_t id, memory mem)
{
        segment seg = Seq_get(mem->segments_map, id);
        return seg;
}

/*
 * rem_segment takes a memory struct and a uint32_t as an index into the
 * sequence of mapped segments and frees the segment corresponding to that
 * index
 *
 * Parameters:
 *      id: The 32 bit segment identifier used to index into the sequence
 *      mem: The memory struct
 */
void rem_segment(uint32_t id, memory mem)
{
        segment_free(get_segment(id, mem));
}

/*
 * free_memory frees the memory allocated for the memory struct, not including
 * the segments the client may have added to the memory struct's sequence of
 * mapped segments
 *
 * Parameters:
 *      mem: The memory struct
 */
void free_memory(memory mem)
{
        int size = get_mem_size(mem);
        for (int i = 0; i < size; i++) {
                segment seg = Seq_get(mem->segments_map, i);
                if (seg != NULL) {
                        segment_free(seg);
                }
        }       

        Seq_free(&(mem->segments_map));
        Seq_free(&(mem->unmap_id));
        free(mem);
}

/*
 * get_mem_size takes a memory struct and returns an integer that is the
 * length of the sequence of mapped segments
 *
 * Parameters:
 *      mem: The memory struct
 */
int get_mem_size(memory mem)
{
        return Seq_length(mem->segments_map);
}

/*
 * add_id takes a memory struct and a uint32_t and add the uint32_t to the
 * sequence of ids for unmapped segments located in the memory struct
 *
 * Parameters:
 *      mem: The memory struct
 *      id: The uint32_t to add to the sequence
 */
void add_id(memory mem, uint32_t id)
{
        Seq_addlo(mem->unmap_id, (void *)(uintptr_t)id);
}

/*
 * remove_id takes a memory struct and removes and returns the first element
 * in a sequence uint32_t segment identifiers
 *
 * Parameters:
 *      mem: The memory struct
 */
uint32_t remove_id(memory mem)
{
        return (uint32_t)(uintptr_t)Seq_remlo(mem->unmap_id);
}

/*
 * get_num_unused takes a memory struct and returns the length of the sequence
 * of segment identifiers for unmapped segments
 *
 * Parameters:
 *      mem: The memory struct
 */
int get_num_unused(memory mem)
{
        return Seq_length(mem->unmap_id);
}

/*
 * put_segment takes a segment struct, a memory struct, and a uint32_t, and
 * puts the segments struct, at the index specified by the uint32_t, into a
 * sequence of mapped segments within the memory struct
 *
 * Parameters:
 *      seg: The segment struct
 *      mem: The memory struct
 *      id: The 32 bit segment identifier 
 */
void put_segment(segment seg, memory mem, uint32_t id)
{
        Seq_put(mem->segments_map, id, (void *) seg);
}

/*
 * get_curr_id takes a memory struct and returns a uint32_t that is the
 * next new 32 bit segment identifier that will be used to map a new segment
 *
 * Parameters:
 *      mem: The memory struct
 */
uint32_t get_curr_id(memory mem)
{
        return mem->curr_id;
}

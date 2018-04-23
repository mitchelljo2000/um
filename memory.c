/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 7 - Profile
  * memory.c
  *
  * Creates a struct that holds a Hanson sequence containing segments as
  * values and 32-bit segment identifiers as their indices in the sequence
 */
#include "memory.h"

// Function prototypes
void expand_memory(memory mem);
void expand_ids(memory mem);

/*
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
*/

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
        mem->segments_map = malloc(hint * sizeof(*mem->segments_map));
        mem->unmap_id = malloc(hint * sizeof(*mem->unmap_id));
        mem->curr_id = 0;
	mem->curr_id2 = 0;
	mem->size = 0;
	mem->size2 = 0;
	mem->capacity = hint;
	mem->capacity2 = hint;
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
	 if (mem->size == mem->capacity) {
                expand_memory(mem);
        }

        mem->segments_map[mem->curr_id] = seg;
	mem->curr_id++;
	mem->size++;
}

/*
 * get_segment takes a uint32_t and a memory struct and returns a segment
 * from the sequence in the memory struct indexed by the uint32_t
 *
 * Parameters:
 *      id: The uint32_t identifier
 *      mem: The memory struct
 */
//segment get_segment(uint32_t id, memory mem)
//{
//        return mem->segments_map[id];
//}

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
	mem->size--;
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
        for (uint32_t i = 0; i < mem->curr_id; i++) {
                segment seg = mem->segments_map[i];
                if (seg != NULL) {
                        segment_free(seg);
                }
        }       

        free(mem->segments_map);
	free(mem->unmap_id);
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
        return mem->size;
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
	mem->unmap_id[mem->curr_id2] = id;
	mem->curr_id2++;
	mem->size2++;

	if (mem->size2 == mem->capacity2) {
		expand_ids(mem);
	}
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
        mem->size2--;
        mem->curr_id2--;
	return mem->unmap_id[mem->curr_id2];
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
	return mem->size2;
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
	mem->segments_map[id] = seg;

	if (seg != NULL) {
		mem->size++;
	}
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

/*
 * expand_memory takes a memory struct and expands the segments_map array
 * within the struct
 *
 * Parameters:
 * 	mem: The memory struct
 */
void expand_memory(memory mem)
{
	int size = mem->size;
	segment *new_seg = malloc(2 * size * sizeof(segment));
	for (int i = 0; i < size; i++) {
		new_seg[i] = mem->segments_map[i];
	}

	free(mem->segments_map);

	mem->segments_map = new_seg;
	mem->capacity = 2 * size;
}

/*
 * expand_ids takes a memory struct and expands the unmap_id array within the 
 * struct
 *
 * Parameters:
 *      mem: The memory struct
 */
void expand_ids(memory mem)
{
        int size = mem->size2;
        uint32_t *new_ids = malloc(2 * size * sizeof(*new_ids));
        for (int i = 0; i < size; i++) {
                new_ids[i] = mem->unmap_id[i];
	}

        free(mem->unmap_id);

        mem->unmap_id = new_ids;
        mem->capacity2 = 2 * size;
}


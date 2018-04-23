/*
  * Joshua Mitchell and Ruchelly Almeida
  * COMP 40 - Spring 2018
  * Homework 6 - UM
  * instructions.c
  *
  * Handles the instructions functions
 */

#include "instructions.h"

// Function prototype
//void parse_registers(uint32_t instruction, int *a_ind, int *b_ind, int *c_ind);

/*
 * move takes a 32 bit word and an array of uint32_t integers, representing 
 * the registers, and makes register $r[A] get the value in register $r[B] if
 * the value in register $r[C] is not equal to zero.
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 */
void move(uint32_t instruction, uint32_t *registers) 
{
        int a_ind, b_ind, c_ind;
        parse_registers(instruction, &a_ind, &b_ind, &c_ind);

        if (registers[c_ind] != 0) {
                registers[a_ind] = registers[b_ind];
        }
}

/*
 * load_segment takes a 32 bit word an array of uint32_t integers, representing
 * the registers, and a memory struct, and uses the value in register $r[C] to
 * index into the array of instructions in a segment indexed from the sequence
 * representing memory by the value in register $r[B] in order to store the
 * word in register $r[A]
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 *      memory: The memory struct representing memory
 */
//void load_segment(uint32_t instruction, uint32_t *registers, memory mem)
//{
//        int a_ind, b_ind, c_ind;
//        parse_registers(instruction, &a_ind, &b_ind, &c_ind);
//
//        segment seg = get_segment(registers[b_ind], mem);
//        registers[a_ind] = get_word(seg, registers[c_ind]);
//}

/*
 * store_segment takes a 32 bit word an array of uint32_t integers, representing
 * the registers, and a memory struct, and uses the value in register $r[B] to
 * index into the array of instructions in a segment indexed from the sequence
 * representing memory by the value in register $r[A] in order to store the
 * value in register $r[C] there
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 *      memory: The memory struct representing memory
 */
/*
void store_segment(uint32_t instruction, uint32_t *registers, memory mem)
{
        int a_ind, b_ind, c_ind;
        parse_registers(instruction, &a_ind, &b_ind, &c_ind);

        segment seg = get_segment(registers[a_ind], mem);
        add_word(seg, registers[c_ind], registers[b_ind]);
}
*/

/*
 * add takes a 32 bit word and an array of uint32_t integers, representing 
 * the registers, and adds the values in registers $r[B] and $r[C] and stores 
 * the result in register $r[A]
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 */
void add(uint32_t instruction, uint32_t *registers)
{
        int a_ind, b_ind, c_ind;
        parse_registers(instruction, &a_ind, &b_ind, &c_ind);
        registers[a_ind] = (registers[b_ind] + registers[c_ind]);
}

/*
 * multiply takes a 32 bit word and an array of uint32_t integers, representing 
 * the registers, and multiplies the values in registers $r[B] and $r[C] and 
 * stores the result in register $r[A]
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 */
void multiply(uint32_t instruction, uint32_t *registers)
{
        int a_ind, b_ind, c_ind;
        parse_registers(instruction, &a_ind, &b_ind, &c_ind);

        registers[a_ind] = (registers[b_ind] * registers[c_ind]);
}

/*
 * divide takes a 32 bit word and an array of uint32_t integers, representing 
 * the registers, and divides the values in registers $r[B] and $r[C] and
 * stores the result in register $r[A]
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 */
void divide(uint32_t instruction, uint32_t *registers)
{
        int a_ind, b_ind, c_ind;
        parse_registers(instruction, &a_ind, &b_ind, &c_ind);

        registers[a_ind] = (registers[b_ind] / registers[c_ind]);
}

/*
 * nand takes a 32 bit word and an array of uint32_t integers, representing 
 * the registers, and bitwise NANDs the values in registers $r[B] and $r[C] and
 * stores the result in register $r[A]
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 */
void nand(uint32_t instruction, uint32_t *registers)
{
        int a_ind, b_ind, c_ind;
        parse_registers(instruction, &a_ind, &b_ind, &c_ind);

        registers[a_ind] = ~(registers[b_ind] & registers[c_ind]);
}

/*
 * map takes a 32 bit word an array of uint32_t integers, representing
 * the registers, and a memory struct, and creates a segment struct with a
 * number of words equal to the value in register $r[C]. Each word in the
 * segment is initialized to 0 and a unique 32 bit segment identifier is
 * placed in register $r[B] and the new segment is placed in the sequence
 * representing memory at the index in register $r[B]
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 *      memory: The memory struct representing memory
 */
void map(uint32_t instruction, uint32_t *registers, memory mem)
{
        int b_ind = Bitpack_getu(instruction, 3, 3);
        int c_ind = Bitpack_getu(instruction, 3, 0);
        
        segment seg = segment_new(registers[c_ind]);    

        if (get_num_unused(mem) == 0) {
                registers[b_ind] = get_curr_id(mem);
                add_segment(seg, mem);
        } else {
                uint32_t id = remove_id(mem);
                put_segment(seg, mem, id);
                registers[b_ind] = id;
        }
}

/*
 * map takes a 32 bit word an array of uint32_t integers, representing
 * the registers, and a memory struct, and frees the segment corresponding to
 * the 32 bit segment identifier in register $r[C]. map also adds the
 * identifier of the now unmapped segment to a sequence of ids of unmapped
 * segments for future use.
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 *      memory: The memory struct representing memory
 */
void unmap(uint32_t instruction, uint32_t *registers, memory mem)
{
        int index = Bitpack_getu(instruction, 3, 0);
        uint32_t unmap_id = registers[index];
        rem_segment(unmap_id, mem);
        add_id(mem, unmap_id);

        segment temp = NULL;
        put_segment(temp, mem, unmap_id);
}

/*
 * output takes a 32 bit word and an array of uint32_t integers, representing 
 * the registers, and prints the value in register $r[C] to stdout
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 */

void output(uint32_t instruction, uint32_t *registers)
{
        int index = Bitpack_getu(instruction, 3, 0);
        fprintf(stdout, "%c", registers[index]);
}

/*
 * input takes a 32 bit word and an array of uint32_t integers, representing 
 * the registers, and reads in an input from stdin and stores the input in
 * register $r[C]
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 */
void input(uint32_t instruction, uint32_t *registers)
{
        int index = Bitpack_getu(instruction, 3, 0);
        char temp;
        scanf("%c", &temp);
        registers[index] = (uint32_t) temp;
}

/*
 * load_program takes a 32 bit word and an array of uint32_t integers, 
 * representing the registers, a pointer to the segment struct
 * representing segment zero, and a memory struct, and loads the segment
 * corresponding to the 32 bit segment identifier in register $r[B] into
 * segment zero, discarding the previous segment zero. 
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 *      seg_zero: The segment struct representing segment zero
 *      mem: The memory struct
 */
void load_program(uint32_t instruction, uint32_t *registers,
                        memory mem, uint32_t *count)
{
        int b_ind = Bitpack_getu(instruction, 3, 3);
        int c_ind = Bitpack_getu(instruction, 3, 0);
        
        if (registers[b_ind] == 0) {
                *count = registers[c_ind] -1;
                return;
        }

        segment seg = get_segment(registers[b_ind], mem);

        int size = get_size(seg);
        segment new_seg = segment_new(size);
        
        for (int i = 0; i < size; i++) {
                add_word(new_seg, get_word(seg, i), i);
        }

        segment_free(get_segment(0, mem));
        put_segment(new_seg, mem, 0);

        *count = registers[c_ind] - 1;
}

/*
 * load_value takes a 32 bit word and an array of uint32_t integers, 
 * representing the registers, and gets the value of the last 25 bits of the
 * word and stores it in register $r[A]
 *
 * Parameters:
 *      instruction: The 32 bit word
 *      registers: The array of uint32_t integers representing the registers
 */
//void load_value(uint32_t instruction, uint32_t *registers)
//{
//        int index = Bitpack_getu(instruction, 3, 25);
//        registers[index] = Bitpack_getu(instruction, 25, 0);
//}

/*
 * parse_registers takes a 32 bit word and pointers to the integer indexes
 * corresponding to registers $r[A], $r[B], and $r[C], and stores the correct
 * indices in the integers based on the word instruction given.
 *
 * Parameters:
 *      instruction; The 32 bit word to parse
 *      a_ind: A pointer to the integer index or $r[A]
 *      b_ind: A pointer to the integer index or $r[B]
 *      c_ind: A pointer to the integer index or $r[C]
 */
//inline void parse_registers(uint32_t instruction, int *a_ind, int *b_ind, 
//                                int *c_ind)
//{
//        *a_ind = Bitpack_getu(instruction, 3, 6);
//        *b_ind = Bitpack_getu(instruction, 3, 3);
//        *c_ind = Bitpack_getu(instruction, 3, 0);
//}

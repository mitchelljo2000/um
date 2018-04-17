/*
 * Joshua Mitchell and Ruchelly Almeida
 * COMP 40 - Spring 2018
 * Homework 6 - UM
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>

#include <assert.h>
#include <seq.h>
#include "bitpack.h"


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        uint32_t instr = 0;
        instr = Bitpack_newu(instr, 3, 0, rc);
        instr = Bitpack_newu(instr, 3, 3, rb);
        instr = Bitpack_newu(instr, 3, 6, ra);
        instr = Bitpack_newu(instr, 4, 28, op);
        return instr;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        uint32_t instr = 0;
        instr = Bitpack_newu(instr, 25, 0, val);
        instr = Bitpack_newu(instr, 3, 25, ra);
        instr = Bitpack_newu(instr, 4, 28, 13);
        return instr;
}

/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

Um_instruction output(Um_register c);

Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

Um_instruction input(Um_register c);

Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

Um_instruction multiply(Um_register a, Um_register b, Um_register c)
{
        return three_register(MUL, a, b, c);
}

Um_instruction divide(Um_register a, Um_register b, Um_register c)
{
        return three_register(DIV, a, b, c);
}

Um_instruction nand(Um_register a, Um_register b, Um_register c)
{
        return three_register(NAND, a, b, c);
}

Um_instruction move(Um_register a, Um_register b, Um_register c)
{
        return three_register(CMOV, a, b, c);
}

Um_instruction map(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

Um_instruction unmap(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

Um_instruction loadprog(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}

Um_instruction segload(Um_register a, Um_register b, Um_register c)
{
        return three_register(SLOAD, a, b, c);
}

Um_instruction segstore(Um_register a, Um_register b, Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

/* Functions for working with streams */

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

extern void Um_write_sequence(FILE *output, Seq_T stream);

extern void Um_write_sequence(FILE *output, Seq_T stream)
{
        int seq_size = Seq_length(stream);

        for (int i = 0; i < seq_size; i++) {
                uint32_t value = (uint32_t)(uintptr_t)Seq_get(stream, i);
                
                char temp = Bitpack_getu(value, 8, 24);
                fprintf(output, "%c", temp);

                temp = Bitpack_getu(value, 8, 16);
                fprintf(output, "%c", temp);

                temp = Bitpack_getu(value, 8, 8);
                fprintf(output, "%c", temp);

                temp = Bitpack_getu(value, 8, 0);
                fprintf(output, "%c", temp);
        }
}

/* Unit tests for the UM */

void emit_halt_test(Seq_T stream)
{
        emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
        emit(stream, halt());
        emit(stream, loadval(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
}

void emit_add_test(Seq_T stream)
{
        emit(stream, loadval(r2, 96));
        emit(stream, loadval(r3, 2));
        emit(stream, add(r1, r2, r3));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_print_six_test(Seq_T stream)
{
        emit(stream, loadval(r1, 48));
        emit(stream, loadval(r2, 6));
        emit(stream, add(r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_io_test(Seq_T stream)
{
        emit(stream, input(r3));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_loadval_test(Seq_T stream)
{
        emit(stream, loadval(r1, 33));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_multiply_test(Seq_T stream)
{
        emit(stream, loadval(r2, 11));
        emit(stream, loadval(r3, 3));
        emit(stream, multiply(r1, r2, r3));
        emit(stream, output(r1));
        emit(stream, halt());

}

void emit_divide_test(Seq_T stream)
{
        emit(stream, loadval(r2, 194));
        emit(stream, loadval(r3, 2));
        emit(stream, divide(r1, r2, r3));
        emit(stream, output(r1));
        emit(stream, halt());

}

void emit_nand_test(Seq_T stream)
{
        emit(stream, loadval(r2, 0));
        emit(stream, nand(r1, r2, r2));
        emit(stream, nand(r1, r1, r1));
        
        emit(stream, loadval(r3, 33));
        emit(stream, add(r1, r3, r1));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_move_test(Seq_T stream)
{
        emit(stream, loadval(r2, 98));
        emit(stream, loadval(r3, 99));
        emit(stream, move(r1, r2, r3));
        emit(stream, output(r1));

        emit(stream, loadval(r2, 103));
        emit(stream, loadval(r3, 0));
        emit(stream, move(r1, r2, r3));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_map_test(Seq_T stream)
{
        for (int i = 0; i < 33; i++) {
                emit(stream, loadval(r3, 100));
                emit(stream, map(r2, r3));
        }
        emit(stream, output(r2));
        emit(stream, halt());
}

void emit_unmap_test(Seq_T stream)
{
        for (int i = 0; i < 160; i++) {
                emit(stream, loadval(r3, 100));
                emit(stream, map(r2, r3));
                emit(stream, unmap(r2));
        }
        emit(stream, loadval(r1, 32));
        emit(stream, add(r2, r2, r1));
        emit(stream, output(r2));
        emit(stream, halt());
}

void emit_segload_test(Seq_T stream)
{
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r5, 6));

        emit(stream, loadval(r6, 23));
        emit(stream, segload(r4, r1, r5));
        emit(stream, add(r4, r6, r4));
        emit(stream, output(r4));
        emit(stream, move(r0, r1, r2));

        emit(stream, halt());    
}

void emit_segstore_test(Seq_T stream)
{
        emit(stream, loadval(r3, 100));
        emit(stream, map(r2, r3));      

        emit(stream, loadval(r5, 33));
        emit(stream, loadval(r1, 97));
        emit(stream, segstore(r2, r5, r1));
        emit(stream, segload(r4, r2, r5));
        
        emit(stream, output(r4));

        emit(stream, unmap(r2));

        emit(stream, halt());
}

void emit_loadprog_test(Seq_T stream)
{
        emit(stream, loadval(r1, 12500000));
        emit(stream, loadval(r3, 4));
        emit(stream, loadval(r5, 0));
        emit(stream, nand(r6, r5, r5));

        emit(stream, add(r1, r1, r6));
        emit(stream, loadval(r2, 8));
        emit(stream, move(r2, r3, r1));
        emit(stream, loadprog(r5, r2));

        emit(stream, loadval(r2, 97));
        emit(stream, add(r1, r1, r2));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_test(Seq_T stream)
{
        emit(stream, loadval(r3, 100));
        emit(stream, map(r2, r3));

        emit(stream, loadval(r4, 0));
        emit(stream, loadval(r5, 13));
        emit(stream, segload(r1, r4, r5));
        emit(stream, segstore(r2, r4, r1));

        emit(stream, loadval(r6, 1));
        emit(stream, loadval(r5, 12));
        emit(stream, segload(r1, r4, r5));
        emit(stream, segstore(r2, r6, r1));

        emit(stream, loadprog(r2, r4));
        emit(stream, output(r3));
        emit(stream, halt());
        emit(stream, unmap(r2));
}

void emit_test2(Seq_T stream)
{
        //for (int i = 0; i < 10000000; i++) {
                emit(stream, loadval(r1, 192));
                emit(stream, loadval(r2, 2));
                emit(stream, add(r1, r1, r2));
                emit(stream, divide(r1, r1, r2));
                emit(stream, output(r1)); 
        //}
}

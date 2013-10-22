/****************************************************************

 Monkeymind - an experimental cogitive architecture

 =============================================================

 Copyright 2013 Bob Mottram

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or
 sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the followingp
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

****************************************************************/

#ifndef MONKEYMIND_LANGUAGE_H
#define MONKEYMIND_LANGUAGE_H

#include "monkeymind_datatypes.h"
#include "monkeymind_rand.h"

/* The number of instructions in the language machine */
#define MM_SIZE_LANGUAGE_INSTRUCTIONS 128

/* The number of arguments which each language machine
   instruction takes */
#define MM_SIZE_LANGUAGE_ARGS         4

/* result of a function */
#define MM_INSTRUCTION_RESULT(instr) \
	((instr)->argument[MM_SIZE_LANGUAGE_ARGS-1])

enum mm_language_machine_functions
{
	MM_INSTRUCTION_NONE = 0,
	MM_INSTRUCTION_MATHS,
	MM_INSTRUCTION_COPY,
	MM_INSTRUCTION_JUMP,
	MM_INSTRUCTIONS
};

enum mm_language_conditions
{
	MM_CONDITION_LESS_THAN = 0,
	MM_CONDITION_EQUALS,
	MM_CONDITION_NOT_EQUALS,
	MM_CONDITION_GREATER_THAN,
	MM_CONDITIONS
};

enum mm_language_maths
{
	MM_MATHS_ADD = 0,
	MM_MATHS_ADD_B,
	MM_MATHS_ADD_C,
	MM_MATHS_SUBTRACT,
	MM_MATHS_SUBTRACT_B,
	MM_MATHS_SUBTRACT_C,
	MM_MATHS_MULTIPLY,
	MM_MATHS_MULTIPLY_B,
	MM_MATHS_MULTIPLY_C,
	MM_MATHS_DIVIDE,
	MM_MATHS_DIVIDE_B,
	MM_MATHS_DIVIDE_C,
	MM_MATHS_DIVIDE_D,
	MM_MATHS_FUNCTIONS
};

/* Representation of a language machine instruction */
typedef struct
{
	/* The type of function */
	n_byte function;

	n_int argument[MM_SIZE_LANGUAGE_ARGS];
} mm_language_instruction;

/* The language machine is just a program containing a series
   of instructions */
typedef struct
{
	mm_language_instruction instruction[MM_SIZE_LANGUAGE_INSTRUCTIONS];
} mm_language_machine;


void mm_language_init(mm_language_machine * lang, mm_random_seed * seed);
void mm_language_maths(mm_language_machine * m0,
					   mm_language_machine * m1,
					   n_byte * buffer, n_uint buffer_size,
					   n_uint index);
void mm_language_copy(mm_language_machine * m0,
					  mm_language_machine * m1,
					  n_byte * buffer, n_uint buffer_size,
					  n_uint index);
n_int mm_language_jump(mm_language_machine * m0,
					   mm_language_machine * m1,
					   n_byte * buffer, n_uint buffer_size,
					   n_int index);
n_int mm_language_get_address(n_int address,
							  n_uint data_size);
n_int mm_language_get_data(mm_language_machine * m0,
						   mm_language_machine * m1,
						   n_byte * buffer, n_uint buffer_size,
						   n_int address);
void mm_language_set_data(mm_language_machine * m0,
						  mm_language_machine * m1,
						  n_byte * buffer, n_uint buffer_size,
						  n_int address, n_int value);
n_uint addresses_per_machine();

#endif

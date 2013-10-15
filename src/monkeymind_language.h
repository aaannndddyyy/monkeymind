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

enum language_machine_functions
{
	MM_INSTRUCTION_NONE = 0,
	MM_INSTRUCTION_DATA,
	MM_INSTRUCTION_ADD,
	MM_INSTRUCTION_SUBTRACT,
	MM_INSTRUCTION_MULTIPLY,
	MM_INSTRUCTION_DIVIDE,
	MM_INSTRUCTION_COPY,
	MM_INSTRUCTIONS
};

/* Representation of a language machine instruction */
typedef struct
{
	/* The type of function */
	n_byte function;

	n_int argument[MM_SIZE_LANGUAGE_ARGS];

	n_int output;
} mm_language_instruction;

/* The language machine is just a program containing a series
   of instructions */
typedef struct
{
	mm_language_instruction instruction[MM_SIZE_LANGUAGE_INSTRUCTIONS];
} mm_language_machine;


void mm_language_init(mm_language_machine * lang, mm_random_seed * seed);
void mm_language_add(mm_language_machine * m0,
					 mm_language_machine * m1,
					 n_byte * data, n_uint data_size,
					 unsigned int index);
void mm_language_subtract(mm_language_machine * m0,
						  mm_language_machine * m1,
						  n_byte * data, n_uint data_size,
						  unsigned int index);
void mm_language_multiply(mm_language_machine * m0,
						  mm_language_machine * m1,
						  n_byte * data, n_uint data_size,
						  unsigned int index);
void mm_language_divide(mm_language_machine * m0,
						mm_language_machine * m1,
						n_byte * data, n_uint data_size,
						unsigned int index);
void mm_language_copy(mm_language_machine * m0,
					  mm_language_machine * m1,
					  n_byte * data, n_uint data_size,
					  unsigned int index);

#endif

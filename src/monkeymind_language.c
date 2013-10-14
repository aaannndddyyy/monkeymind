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

#include "monkeymind_language.h"

const n_uint addresses_per_machine =
	(n_uint)(sizeof(mm_language_machine)/sizeof(n_int));

/* ensure that a given address is within range of the
   address space of the language machine */
static n_int get_address(n_int address,
						 n_uint data_size)
{
	/* ensure that the address is within range */
	if (address < 0) address = -address;
    address = address%((addresses_per_machine*2) +
					   (data_size/sizeof(n_int)));
	return address;
}

/* Returns the data at the given address for the given two
   language machines and data store.
   The data store would typically be the current state
   of the cognitive system, and this means that
   brain probes are not needed. */
static n_int get_data(mm_language_machine * m0,
					  mm_language_machine * m1,
					  n_byte * data, n_uint data_size,
					  n_int address)
{
	n_int * m;

	address = get_address(address, data_size);

	if (address < addresses_per_machine) {
		m = (n_int*)m0;
	}
	else if (address < addresses_per_machine*2) {
		address -= addresses_per_machine;
		m = (n_int*)m1;
	}
	else {
		address -= (addresses_per_machine*2);
		m = (n_int*)data;
	}

	return m[address];
}


/* initialise the language machine */
void mm_language_init(mm_language_machine * lang,
					  mm_random_seed * seed)
{
	n_int i, j;

	for (i = 0; i < MM_SIZE_LANGUAGE_INSTRUCTIONS; i++) {
		lang->instruction[i].function =
			mm_rand(seed) & 255;
		for (j = 0; j < MM_SIZE_LANGUAGE_ARGS; j++) {
			lang->instruction[i].argument[j] =
				(n_int)mm_rand(seed);
		}
	}
}

/* sets the value at the given address.
   The total address space consists of the two language machines
   and the data, which is usually the current state of the
   cognitive system */
static void set_data(mm_language_machine * m0,
					 mm_language_machine * m1,
					 n_byte * data, n_uint data_size,
					 n_int address, n_int value)
{
	n_int * m;

	address = get_address(address, data_size);

	if (address < addresses_per_machine) {
		m = (n_int*)m0;
	}
	else if (address < addresses_per_machine*2) {
		address -= addresses_per_machine;
		m = (n_int*)m1;
	}
	else {
		address -= (addresses_per_machine*2);
		m = (n_int*)data;
	}

	m[address] = value;
}

static void fn_add(mm_language_machine * m0,
				   mm_language_machine * m1,
				   n_byte * data, n_uint data_size,
				   unsigned int index)
{
	n_int i, total = 0;
	mm_language_instruction * instruction;

	instruction = &m0->instruction[index];
	for (i = 0; i < MM_SIZE_LANGUAGE_ARGS; i++) {
		total += get_data(m0, m1, data, data_size,
						  instruction->argument[i]);
	}
	set_data(m0, m1, data, data_size,
			 instruction->output, total);
}

static void fn_subtract(mm_language_machine * m0,
						mm_language_machine * m1,
						n_byte * data, n_uint data_size,
						unsigned int index)
{
	n_int i, total = 0;
	mm_language_instruction * instruction;

	instruction = &m0->instruction[index];
	total = get_data(m0, m1, data, data_size,
					 instruction->argument[0]);
	for (i = 1; i < MM_SIZE_LANGUAGE_ARGS; i++) {
		total -= get_data(m0, m1, data, data_size,
						  instruction->argument[i]);
	}
	set_data(m0, m1, data, data_size,
			 instruction->output, total);
}

static void fn_multiply(mm_language_machine * m0,
						mm_language_machine * m1,
						n_byte * data, n_uint data_size,
						unsigned int index)
{
	n_int i, total = 0;
	mm_language_instruction * instruction;

	instruction = &m0->instruction[index];
	total = get_data(m0, m1, data, data_size,
					 instruction->argument[0]);
	for (i = 1; i < MM_SIZE_LANGUAGE_ARGS; i++) {
		total *= get_data(m0, m1, data, data_size,
						  instruction->argument[i]);
	}
	set_data(m0, m1, data, data_size,
			 instruction->output, total);
}

static void fn_divide(mm_language_machine * m0,
					  mm_language_machine * m1,
					  n_byte * data, n_uint data_size,
					  unsigned int index)
{
	n_int i, num, denom, total = 0;
	mm_language_instruction * instruction;

	instruction = &m0->instruction[index];
	num = get_data(m0, m1, data, data_size,
				   instruction->argument[0]);
	for (i = 1; i < MM_SIZE_LANGUAGE_ARGS; i++) {
		denom = get_data(m0, m1, data, data_size,
						 instruction->argument[i])&15;
		total += (num >> denom);
	}
	set_data(m0, m1, data, data_size,
			 instruction->output, total);
}

static void fn_copy(mm_language_machine * m0,
					mm_language_machine * m1,
					n_byte * data, n_uint data_size,
					unsigned int index)
{
	n_int value;
	mm_language_instruction * instruction;

	instruction = &m0->instruction[index];
	value = get_data(m0, m1, data, data_size,
					 instruction->argument[0]);
	set_data(m0, m1, data, data_size,
			 instruction->output, value);
}

/* A dialogue between two language machines.
   These would typically be the inner and outer systems */
void mm_language_dialogue(mm_language_machine * m0,
						  mm_language_machine * m1,
						  n_byte * data, n_uint data_size,
						  unsigned int index)
{
	n_uint i;

	for (i = 0; i < MM_SIZE_LANGUAGE_INSTRUCTIONS; i++) {
		switch(m0->instruction[i].function) {
		case MM_INSTRUCTION_ADD: {
			fn_add(m0, m1, data, data_size, index);
			break;
		}
		case MM_INSTRUCTION_SUBTRACT: {
			fn_subtract(m0, m1, data, data_size, index);
			break;
		}
		case MM_INSTRUCTION_MULTIPLY: {
			fn_multiply(m0, m1, data, data_size, index);
			break;
		}
		case MM_INSTRUCTION_DIVIDE: {
			fn_divide(m0, m1, data, data_size, index);
			break;
		}
		case MM_INSTRUCTION_COPY: {
			fn_copy(m0, m1, data, data_size, index);
			break;
		}
		}
	}
}

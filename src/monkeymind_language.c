/****************************************************************

 Monkeymind - an experimental cogitive architecture

 =============================================================

 Copyright 2013-2014 Bob Mottram

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

n_uint addresses_per_machine()
{
    return (n_uint)(sizeof(mm_language_machine)/sizeof(n_int));
}

/* initialise the language machine */
void mm_language_init(mm_language_machine * lang,
                      mm_random_seed * seed)
{
    n_int i, j;

    for (i = 0; i < MM_SIZE_LANGUAGE_INSTRUCTIONS; i++) {
        lang->instruction[i].function =
            (n_byte)mm_rand(seed);
        for (j = 0; j < MM_SIZE_LANGUAGE_ARGS; j++) {
            lang->instruction[i].argument[j] =
                (n_int)mm_rand(seed);
        }
    }
}

/* ensure that a given address is within range of the
   address space of the language machine */
n_int mm_language_get_address(n_int address,
                              n_uint data_size)
{
    /* ensure that the address is within range */
    if (address < 0) address = -address;
    address = address%((addresses_per_machine()*2) +
                       (data_size/sizeof(n_int)));
    return address;
}

/* Returns the data at the given address for the given two
   language machines and data store.
   The data store would typically be the current state
   of the cognitive system, and this means that
   brain probes are not needed. */
n_int mm_language_get_data(mm_language_machine * m0,
                           mm_language_machine * m1,
                           n_byte * data, n_uint data_size,
                           n_int address)
{
    n_int * m;

    address = mm_language_get_address(address, data_size);

    if (address < addresses_per_machine()) {
        m = (n_int*)m0;
    }
    else if (address < addresses_per_machine()*2) {
        address -= addresses_per_machine();
        m = (n_int*)m1;
    }
    else {
        address -= (addresses_per_machine()*2);
        m = (n_int*)data;
    }

    return m[address];
}

/* sets the value at the given address.
   The total address space consists of the two language machines
   and the data, which is usually the current state of the
   cognitive system */
void mm_language_set_data(mm_language_machine * m0,
                          mm_language_machine * m1,
                          n_byte * data, n_uint data_size,
                          n_int address, n_int value)
{
    n_int * m;

    address = mm_language_get_address(address, data_size);

    if (address < addresses_per_machine()) {
        m = (n_int*)m0;
    }
    else if (address < addresses_per_machine()*2) {
        address -= addresses_per_machine();
        m = (n_int*)m1;
    }
    else {
        address -= (addresses_per_machine()*2);
        m = (n_int*)data;
    }

    m[address] = value;
}

void mm_language_maths(mm_language_machine * m0,
                       mm_language_machine * m1,
                       n_byte * buffer, n_uint buffer_size,
                       n_uint index)
{
    n_int value[2];
    n_byte function;
    mm_language_instruction * instruction;

    instruction = &m0->instruction[index];

    function = mm_language_get_data(m0, m1, buffer, buffer_size,
                                    instruction->argument[0]);

    value[0] = mm_language_get_data(m0, m1, buffer, buffer_size,
                                    instruction->argument[1]);
    value[1] = mm_language_get_data(m0, m1, buffer, buffer_size,
                                    instruction->argument[2]);

    switch(function%MM_MATHS_FUNCTIONS) {
    case MM_MATHS_ADD: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] + value[1]);
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] - value[1]);
        break;
    }
    case MM_MATHS_ADD_B: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] + value[1]);
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] * value[1]);
        break;
    }
    case MM_MATHS_ADD_C: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] + value[1]);
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] >> (value[1]&15));
        break;
    }
    case MM_MATHS_SUBTRACT: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] - value[1]);
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] + value[1]);
        break;
    }
    case MM_MATHS_SUBTRACT_B: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] - value[1]);
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] * value[1]);
        break;
    }
    case MM_MATHS_SUBTRACT_C: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] - value[1]);
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] >> (value[1]&15));
        break;
    }
    case MM_MATHS_MULTIPLY: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] * value[1]);
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] + value[1]);
        break;
    }
    case MM_MATHS_MULTIPLY_B: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] * value[1]);
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] - value[1]);
        break;
    }
    case MM_MATHS_MULTIPLY_C: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] * value[1]);
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] >> (value[1]&15));
        break;
    }
    case MM_MATHS_DIVIDE: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] >> (value[1]&15));
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[1] >> (value[0]&15));
        break;
    }
    case MM_MATHS_DIVIDE_B: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] >> (value[1]&15));
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] + value[1]);
        break;
    }
    case MM_MATHS_DIVIDE_C: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] >> (value[1]&15));
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] - value[1]);
        break;
    }
    case MM_MATHS_DIVIDE_D: {
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[0], value[0] >> (value[1]&15));
        mm_language_set_data(m0, m1, buffer, buffer_size,
                             instruction->argument[1], value[0] * value[1]);
        break;
    }
    }
}

void mm_language_copy(mm_language_machine * m0,
                      mm_language_machine * m1,
                      n_byte * buffer, n_uint buffer_size,
                      n_uint index)
{
    n_int value;
    mm_language_instruction * instruction;

    instruction = &m0->instruction[index];

    /* value to be copied */
    value = mm_language_get_data(m0, m1, buffer, buffer_size,
                                 instruction->argument[0]);
    /* copy to destination */
    mm_language_set_data(m0, m1, buffer, buffer_size,
                         instruction->argument[0], value);
}

n_int mm_language_jump(mm_language_machine * m0,
                       mm_language_machine * m1,
                       n_byte * buffer, n_uint buffer_size,
                       n_int index)
{
    n_int value[2], i, result = 0;
    mm_language_instruction * instruction;
    unsigned char condition;

    instruction = &m0->instruction[index];

    /* get the type of condition to be tested */
    condition = mm_language_get_data(m0, m1, buffer, buffer_size,
                                     instruction->argument[0]);
    if (condition < 0) condition = -condition;
    condition = condition % MM_CONDITIONS;

    /* get two input values */
    for (i = 0; i < 2; i++) {
        value[i] = mm_language_get_data(m0, m1, buffer, buffer_size,
                                        instruction->argument[i+1]);
    }

    /* is the condition satisfied? */
    switch(condition) {
    case MM_CONDITION_LESS_THAN: {
        if (value[0] < value[1]) {
            result = MM_INSTRUCTION_RESULT(instruction);
        }
        break;
    }
    case MM_CONDITION_EQUALS: {
        if (value[0] == value[1]) {
            result = MM_INSTRUCTION_RESULT(instruction);
        }
        break;
    }
    case MM_CONDITION_NOT_EQUALS: {
        if (value[0] != value[1]) {
            result = MM_INSTRUCTION_RESULT(instruction);
        }
        break;
    }
    case MM_CONDITION_GREATER_THAN: {
        if (value[0] > value[1]) {
            result = MM_INSTRUCTION_RESULT(instruction);
        }
        break;
    }
    }

    if (result == 0) return index;

    /* return the new index to jump to */
    return result&255;
}

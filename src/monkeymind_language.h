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

/* the number of instructions in the language machine */
#define MM_SIZE_LANGUAGE_INSTRUCTIONS 128

/* te number of arguments which each language machine
   instruction takes */
#define MM_SIZE_LANGUAGE_ARGS    2

enum language_machine_functions
{
    MM_INSTRUCTION_NONE = 0,
	MM_INSTRUCTION_ADD,
	MM_INSTRUCTION_REMOVE,
	MM_INSTRUCTION_INSERT,
	MM_INSTRUCTION_COPY,
	MM_INSTRUCTIONS
};

/* representation of a language machine instruction */
typedef struct
{
	unsigned char function;
	unsigned int argument[MM_SIZE_LANGUAGE_ARGS];
	/* flags for relative or absolute addressing */
	unsigned char flags;
} mm_language_instruction;

/* the language machine is just a program containing a series
   of instructions */
typedef struct
{
	mm_language_instruction instruction[MM_SIZE_LANGUAGE_INSTRUCTIONS];
} mm_language;


#endif
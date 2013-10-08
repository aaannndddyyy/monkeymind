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

#ifndef MONKEYMIND_NARRATIVE_H
#define MONKEYMIND_NARRATIVE_H

#include <stdio.h>
#include <string.h>
#include "monkeymind_object.h"

/* the maximum number of steps within a narrative sequence */
#define MM_MAX_NARRATIVE_SIZE  16

typedef struct
{
	mm_object who;
	mm_object what;
	mm_object where;
	mm_object when;
	mm_object why;
	mm_object way;
	mm_object means;
} mm_circumstance;

typedef struct
{
    /* a unique reference for the narrative */
    unsigned int id;

    /* the number of steps in the narrative */
    unsigned int length;

    /* array storing the steps in the narrative */
    mm_circumstance step[MM_MAX_NARRATIVE_SIZE];
} mm_narrative;

#endif

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
    /* a unique reference for the narrative */
    unsigned int id;

	/* properties of the narrative */
	mm_object properties;

    /* the number of steps in the narrative */
    unsigned int length;

    /* array storing the steps in the narrative */
    mm_object step[MM_MAX_NARRATIVE_SIZE];
} mm_narrative;

void mm_narrative_init(mm_narrative * narrative, unsigned int id);
int mm_narrative_insert(mm_narrative * narrative,
						mm_object * obj, unsigned int index,
						unsigned int act,
						unsigned int scene,
						unsigned int viewpoint);
int mm_narrative_add(mm_narrative * narrative, mm_object * obj,
					 unsigned int act,
					 unsigned int scene,
					 unsigned int viewpoint);
mm_object * mm_narrative_get(mm_narrative * narrative, unsigned int index);

#endif

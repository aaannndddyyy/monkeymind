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
#include "monkeymind_episodic.h"

/* the maximum number of steps within a narrative sequence */
#define MM_MAX_TALE_SIZE       16

/* size of narrative memory */
#define MM_SIZE_NARRATIVES     32

typedef struct
{
    /* a unique reference for the narrative */
    n_uint id;

    /* properties of the narrative */
    mm_object properties;

    /* the number of steps in the narrative */
    n_uint length;

    /* array storing the steps in the narrative */
    mm_object step[MM_MAX_TALE_SIZE];

    /* the number of times told or heard */
    n_uint times_told;
    n_uint times_heard;
} mm_tale;

void mm_tale_init(mm_tale * tale, n_uint id);
n_int mm_tale_insert(mm_tale * tale,
                     mm_object * obj, n_uint index,
                     n_uint act,
                     n_uint scene,
                     n_uint viewpoint);
n_int mm_tale_remove(mm_tale * tale,
                     n_uint index);
n_int mm_tale_add(mm_tale * tale, mm_object * obj,
                  n_uint act,
                  n_uint scene,
                  n_uint viewpoint);
mm_object * mm_tale_get(mm_tale * tale, n_uint index);
n_int mm_tale_from_events(mm_episodic * events, mm_tale * tale);

/* ===================================================================== */

typedef struct
{
    n_uint length;
    mm_tale tale[MM_SIZE_NARRATIVES];
} mm_narratives;

void mm_narratives_init(mm_narratives * narratives);
void mm_narratives_copy(mm_narratives * narratives,
                        n_uint index,
                        mm_tale * tale);
n_int mm_narratives_insert(mm_narratives * narratives,
                           n_uint index,
                           mm_tale * tale);
n_int mm_narratives_remove(mm_narratives * narratives,
                           n_uint index);
n_int mm_narratives_add(mm_narratives * narratives,
                        mm_tale * tale);
n_int mm_narratives_get(mm_narratives * narratives, n_uint id);
n_int mm_narratives_least_heard(mm_narratives * narratives);

#endif

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

#include "monkeymind_tale.h"

void mm_tale_init(mm_tale * tale, n_uint id)
{
    tale->id = id;
    tale->length = 0;
    tale->times_told = 0;

	// clear the tale
    memset((void*)tale->step, '\0',
           MM_MAX_TALE_SIZE*sizeof(mm_object));
}

/* remove a step from the tale */
n_int mm_tale_remove(mm_tale * tale,
                     n_uint index)
{
    n_uint i;

    if ((tale->length == 0) ||
        (index >= tale->length) ||
        (index >= MM_MAX_TALE_SIZE)) {
        return -1;
    }

    for (i = index+1; i < tale->length; i++) {
        mm_obj_copy(&tale->step[i],
                    &tale->step[i-1]);
    }
    tale->length--;

    return 0;
}

/* insert a tale step at a given index */
n_int mm_tale_insert(mm_tale * tale,
                     mm_object * obj, n_uint index,
                     n_uint act,
                     n_uint scene,
                     n_uint viewpoint)
{
    if ((index > tale->length) ||
        (tale->length >= MM_MAX_TALE_SIZE)) {
        return -1;
    }
    mm_obj_prop_add(obj,
                    MM_PROPERTY_TALE_ACT, act);
    mm_obj_prop_add(obj,
                    MM_PROPERTY_TALE_SCENE, scene);
    mm_obj_prop_add(obj,
                    MM_PROPERTY_TALE_VIEWPOINT, viewpoint);
    mm_obj_copy(obj, &tale->step[index]);
    return 0;
}

/* adds a tale step */
n_int mm_tale_add(mm_tale * tale, mm_object * obj,
                  n_uint act,
                  n_uint scene,
                  n_uint viewpoint)
{
    if (tale->length >= MM_MAX_TALE_SIZE) return -1;
    if (mm_tale_insert(tale, obj, tale->length,
                       act, scene, viewpoint) != 0) {
        return -1;
    }
    tale->length++;
    return 0;
}

/* gets a tale step */
mm_object * mm_tale_get(mm_tale * tale, n_uint index)
{
    if ((index > tale->length) ||
        (tale->length >= MM_MAX_TALE_SIZE)) {
        return 0;
    }
    return &tale->step[index];
}

/* attempts to generate a tale from a sequence of events */
n_int mm_tale_from_events(mm_episodic * events, mm_tale * tale)
{
    /* TODO */
    return -1;
}

/* returns the similarity between two tales */
n_int mm_tale_match(mm_tale * tale1, mm_tale * tale2, n_int * offset)
{
	n_int similarity, max_similarity=0;
	n_int i, off;
	mm_tale * t1, * t2;

	if (tale2->length > tale1->length) {
		t1 = tale2;
		t2 = tale1;
	}
	else {
		t1 = tale1;
		t2 = tale2;
	}

	for (off = 0; off < t1->length - t2->length; off++) {
		similarity = 0;
		for (i = 0; i < t2->length; i++) {
			similarity += mm_obj_match(&t1->step[off+i],&t2->step[i]);
		}
		if (similarity > max_similarity) {
			max_similarity = similarity;
			*offset = off;
		}
	}

	return max_similarity;
}

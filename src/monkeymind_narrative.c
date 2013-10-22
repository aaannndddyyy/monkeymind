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

#include "monkeymind_narrative.h"

void mm_narrative_init(mm_narrative * narrative, n_uint id)
{
	narrative->length = 0;
	memset((void*)narrative->step, '\0',
		   MM_MAX_NARRATIVE_SIZE*sizeof(mm_object));
}

/* remove a step from the narrative */
n_int mm_narrative_remove(mm_narrative * narrative,
						  n_uint index)
{
	n_uint i;

	if ((narrative->length == 0) ||
		(index >= narrative->length) ||
		(index >= MM_MAX_NARRATIVE_SIZE)) {
		return -1;
	}

	for (i = index+1; i < narrative->length; i++) {
		mm_obj_copy(&narrative->step[i],
					&narrative->step[i-1]);
	}
	narrative->length--;

	return 0;
}

/* insert a narrative step at a given index */
n_int mm_narrative_insert(mm_narrative * narrative,
						  mm_object * obj, n_uint index,
						  n_uint act,
						  n_uint scene,
						  n_uint viewpoint)
{
	if ((index > narrative->length) ||
		(narrative->length >= MM_MAX_NARRATIVE_SIZE)) {
		return -1;
	}
	mm_obj_prop_add(obj,
					MM_PROPERTY_NARRATIVE_ACT, act);
	mm_obj_prop_add(obj,
					MM_PROPERTY_NARRATIVE_SCENE, scene);
	mm_obj_prop_add(obj,
					MM_PROPERTY_NARRATIVE_VIEWPOINT, viewpoint);
	mm_obj_copy(obj, &narrative->step[index]);
	return 0;
}

/* adds a narrative step */
n_int mm_narrative_add(mm_narrative * narrative, mm_object * obj,
					   n_uint act,
					   n_uint scene,
					   n_uint viewpoint)
{
	if (narrative->length >= MM_MAX_NARRATIVE_SIZE) return -1;
	if (mm_narrative_insert(narrative, obj, narrative->length,
							act, scene, viewpoint) != 0) {
		return -1;
	}
	narrative->length++;
	return 0;
}

/* gets a narrative step */
mm_object * mm_narrative_get(mm_narrative * narrative, n_uint index)
{
	if ((index > narrative->length) ||
		(narrative->length >= MM_MAX_NARRATIVE_SIZE)) {
		return 0;
	}
	return &narrative->step[index];
}

/* attempts to generate a narrative from a sequence of events */
n_int mm_narrative_from_events(mm_episodic * events, mm_narrative * narrative)
{
	/* TODO */
	return -1;
}

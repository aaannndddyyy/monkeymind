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

void mm_tale_init(mm_tale * tale, n_uint id)
{
	tale->id = id;
	tale->length = 0;
	tale->times_told = 0;
	memset((void*)tale->step, '\0',
		   MM_MAX_TALE_SIZE*sizeof(mm_object));
}

/* remove a step from the narrative */
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

/* insert a narrative step at a given index */
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
					MM_PROPERTY_NARRATIVE_ACT, act);
	mm_obj_prop_add(obj,
					MM_PROPERTY_NARRATIVE_SCENE, scene);
	mm_obj_prop_add(obj,
					MM_PROPERTY_NARRATIVE_VIEWPOINT, viewpoint);
	mm_obj_copy(obj, &tale->step[index]);
	return 0;
}

/* adds a narrative step */
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

/* gets a narrative step */
mm_object * mm_tale_get(mm_tale * tale, n_uint index)
{
	if ((index > tale->length) ||
		(tale->length >= MM_MAX_TALE_SIZE)) {
		return 0;
	}
	return &tale->step[index];
}

/* attempts to generate a narrative from a sequence of events */
n_int mm_tale_from_events(mm_episodic * events, mm_tale * tale)
{
	/* TODO */
	return -1;
}

/* ======================================================================== */

void mm_narratives_init(mm_narratives * narratives)
{
	narratives->length = 0;
}

void mm_narratives_copy(mm_narratives * narratives,
						n_uint index,
						mm_tale * tale)
{
	memcpy((void*)&narratives->tale[index],
		   (void*)tale,
		   sizeof(mm_tale));
}

/* inserts a narrative into the array of narratives at the given
   array index */
n_int mm_narratives_insert(mm_narratives * narratives,
						   n_uint index,
						   mm_tale * tale)
{
	if (index >= MM_SIZE_NARRATIVES) return -1;
	if (index > narratives->length) return -2;

	mm_narratives_copy(narratives, index, tale);

	if (index == narratives->length) {
		narratives->length++;
	}

	return 0;
}

/* remove a tale at the given array index */
n_int mm_narratives_remove(mm_narratives * narratives,
						   n_uint index)
{
	n_uint i;

	if (index >= narratives->length) return -1;

	for (i = index+1; i < narratives->length; i++) {
		mm_narratives_copy(narratives, i-1,
						   &narratives->tale[i]);
	}
	narratives->length--;
	return 0;
}

/* adds a tale to the array */
n_int mm_narratives_add(mm_narratives * narratives,
						mm_tale * tale)
{
	return mm_narratives_insert(narratives, narratives->length, tale);
}

/* returns the array index of the narrative with the given id */
n_int mm_narratives_get(mm_narratives * narratives, n_uint id)
{
	n_uint i;

	for (i = 0; i < narratives->length; i++) {
		if (narratives->tale[i].id == id) return i;
	}
	return -1;
}

/* returns the array index of the least heard tale */
n_int mm_narratives_least_heard(mm_narratives * narratives)
{
	n_int i, index = 0;
	n_uint min_heard = 0;

	for (i = 0; i <narratives->length; i++) {
		if ((i == 0) ||
			(narratives->tale[i].times_heard < min_heard)) {
			min_heard = narratives->tale[i].times_heard;
			index = i;
		}
	}
	return index;
}

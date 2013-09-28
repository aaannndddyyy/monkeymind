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

#include "monkeymind_object.h"

/*
static void mm_object_sort(mm_object * obj)
{
	unsigned int i, j, min, index;
	unsigned int temp_type, temp_value;

	for (i = 0; i < obj->length; i++) {
		min = obj->property_type[i];
		index = i;
		for (j = i+1; i < obj->length; j++) {
			if (obj->property_type[j] < min) {
				min = obj->property_type[j];
				index = j;
			}
		}
		if (index > i) {
			temp_type = obj->property_type[i];
			temp_value = obj->property_value[i];
			obj->property_type[i] = obj->property_type[index];
			obj->property_value[i] = obj->property_value[index];
			obj->property_type[index] = temp_type;
			obj->property_type[index] = temp_value;
		}
	}
}
*/

/* returns the array index of a given property type
   or -1 if not found */
int mm_obj_prop_index(mm_object * obj,
					  unsigned int property_type)
{
	int index = -1, start_index, end_index, curr_index;

	start_index = 0;
	end_index = obj->length-1;
	curr_index = start_index + ((end_index-start_index)/2);
	while ((curr_index != start_index) &&
		   (curr_index != end_index)) {
		if (property_type < obj->property_type[curr_index]) {
			end_index = curr_index;
		}
		else {
			if (property_type > obj->property_type[curr_index]) {
				start_index = curr_index;
			}
			else {
				index = curr_index;
				break;
			}
		}
		curr_index = start_index + ((end_index-start_index)/2);
	}

	return index;
}

/* adds a property to an object and returns its array index */
int mm_obj_prop_add(mm_object * obj,
					unsigned int property_type,
					unsigned int property_value)
{
	int i, index = 0;

	if (obj->length >= MM_MAX_OBJECT_PROPERTIES) return -1;

	index = mm_obj_prop_index(obj, property_type);
	if (index == -1) {
		/* property doesn't already exist
		   so find a location for it */
		index = 0;
		while (index < obj->length) {
			if (obj->property_type[index] > property_type) {
				break;
			}
			index++;
		}
	}
	else {
		/* overwrite existing entry for this property type */
		obj->property_value[index] = property_value;
		return index;
	}

	if (index < obj->length) {
		/* insert */
		for (i = obj->length-1; i >= index; i--) {
			obj->property_type[i+1] = obj->property_type[i];
			obj->property_value[i+1] = obj->property_value[i];
		}
	}
	obj->property_type[index] = property_type;
	obj->property_value[index] = property_value;
	obj->length++;
	return index;
}

/* copies one object to another */
void mm_obj_copy(mm_object *src, mm_object * dest)
{
	int i;

	for (i = 0; i < src->length; i++) {
	    mm_obj_prop_add(dest,
						src->property_type[i],
						src->property_value[i]);
	}
}

/* removes a property from an object */
int mm_obj_prop_remove(mm_object * obj,
					   unsigned int property_type)
{
	int i, index;

	if (obj->length == 0) return -1;

	index = mm_obj_prop_index(obj, property_type);
	if (index == -1) return -1;

	for (i = index+1; i < obj->length; i++) {
		obj->property_type[i-1] = obj->property_type[i];
		obj->property_value[i-1] = obj->property_value[i];
	}
	obj->length--;
	return 0;
}

/* returns the value of the given object property */
unsigned int mm_obj_prop_get(mm_object * obj,
							 unsigned int property_type)
{
	int index = mm_obj_prop_index(obj, property_type);
	if (index > -1) return obj->property_value[index];
	return 0;
}

/* sets an object property value */
int mm_obj_prop_set(mm_object * obj,
					unsigned int property_type,
					unsigned int property_value)
{
	int index = mm_obj_prop_index(obj, property_type);
	if (index > -1) {
		obj->property_value[index] = property_value;
		return 0;
	}
	return -1;
}

/* set an object property at a specific index */
void mm_obj_prop_set_index(mm_object * obj,
						   int index,
						   unsigned int property_type,
						   unsigned int property_value)
{
	obj->property_type[index] = property_type;
	obj->property_value[index] = property_value;
	if (index >= obj->length) obj->length = (unsigned int)index+1;
}

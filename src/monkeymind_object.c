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

/* returns the range of a given object property type */
int mm_obj_prop_range(unsigned int property_type,
					  unsigned int * min, unsigned int * max)
{
    const unsigned int property_range[] = {
	    MM_PROPERTY_NONE, 0,0,

		/* who */
		MM_PROPERTY_MEETER, 0,0,
		MM_PROPERTY_MET, 0,0,
		MM_PROPERTY_NAME, 0,0,
		MM_PROPERTY_MEETER_NAME, 0,0,
		MM_PROPERTY_MET_NAME, 0,0,
		MM_PROPERTY_RELATION, 0,0,
		MM_PROPERTY_SOCIAL_X, 0, 64,
		MM_PROPERTY_SOCIAL_Y, 0, 64,

		/* what */
		MM_PROPERTY_OBJECT, 0,0,
		MM_PROPERTY_NARRATIVE, 0,0,
		MM_PROPERTY_EMOTION, 0,0,
		MM_PROPERTY_HEIGHT, 0,0,
		MM_PROPERTY_WEIGHT, 0,0,
		MM_PROPERTY_HAIR_LENGTH, 0,0,
		MM_PROPERTY_HAIR_COLOUR, 0,0,

		MM_PROPERTY_FRIEND_OR_FOE, 0,0,

		/* where */
		MM_PROPERTY_PLACE_X, 0,0,
		MM_PROPERTY_PLACE_Y, 0,0,
		MM_PROPERTY_PLACE_Z, 0,0,
		MM_PROPERTY_TERRITORY, 0,0,

		/* when */
		MM_PROPERTY_DAY, 0,0,
		MM_PROPERTY_SUBJECTIVE_TIME, 0,0,
	};

	*min = 0;
	*max = 0;

	if (property_type >= MM_PROPERTIES) {
		printf("Property type out of range\n");
		return -1;
	}
	*min = property_range[property_type*3 + 1];
	*max = property_range[property_type*3 + 2];

	return 0;
}


/* returns the array index of a given property type
   or -1 if not found */
int mm_obj_prop_index(mm_object * obj,
					  unsigned int property_type)
{
	int index = -1, start_index, end_index, curr_index;

	if (obj->length == 1) {
		if (obj->property_type[0] == property_type) {
			return 0;
		}
	}

	start_index = 0;
	end_index = obj->length-1;
	curr_index = start_index + ((end_index-start_index)/2);

	if (property_type == obj->property_type[end_index]) {
		index = end_index;
	}
	else if (property_type == obj->property_type[start_index]) {
		index = start_index;
	}

	while ((index == -1) &&
		   (curr_index != start_index) &&
		   (curr_index != end_index)) {

		if (property_type < obj->property_type[curr_index]) {
			end_index = curr_index;
			if (property_type == obj->property_type[end_index]) {
				index = end_index;
			}
		}
		else {
			if (property_type > obj->property_type[curr_index]) {
				start_index = curr_index;
				if (property_type == obj->property_type[start_index]) {
					index = start_index;
				}
			}
			else {
				index = curr_index;
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

/* normalise property values into a single byte range */
void mm_obj_to_vect(mm_object * obj,
					unsigned char * vect)
{
	unsigned int i, p, min, max, v;

	memset((void*)vect, '\0',
		   MM_PROPERTIES*sizeof(unsigned char));
	for (i = 0; i < obj->length; i++) {
		p = obj->property_type[i];
		min = max = 0;
		if (mm_obj_prop_range(p, &min, &max) == 0) {
			if (min + max > 0) {
				v = obj->property_value[i];
				vect[p] =
					(unsigned char)((v - min) * 255 / max);
			}
		}
	}
}

int mm_obj_exists(mm_object * obj)
{
	return (obj->length > 0);
}

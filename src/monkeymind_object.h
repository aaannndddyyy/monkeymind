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

#ifndef MONKEYMIND_OBJECT_H
#define MONKEYMIND_OBJECT_H

#include <stdio.h>
#include <string.h>

/* the maximum number of properties of an object */
#define MM_MAX_OBJECT_PROPERTIES  16

enum relation_types
{
	MM_RELATION_SELF,
	MM_RELATION_BROTHER,
	MM_RELATION_SISTER,
	MM_RELATION_FATHER,
	MM_RELATION_MOTHER,
	MM_RELATION_GRANDFATHER,
	MM_RELATION_GRANDMOTHER,
	MM_RELATION_UNCLE,
	MM_RELATION_AUNT,
	MM_RELATIONS
};

enum property_types
{
	MM_PROPERTY_NONE = 0,

	/* who */
	MM_PROPERTY_MEETER,
	MM_PROPERTY_MET,
	MM_PROPERTY_NAME,
	MM_PROPERTY_MEETER_NAME,
	MM_PROPERTY_MET_NAME,
	MM_PROPERTY_RELATION,

	/* what */
    MM_PROPERTY_OBJECT,
    MM_PROPERTY_NARRATIVE,
	MM_PROPERTY_EMOTION,
	MM_PROPERTY_HEIGHT,
	MM_PROPERTY_WEIGHT,
	MM_PROPERTY_HAIR_LENGTH,
	MM_PROPERTY_HAIR_COLOUR,

	MM_PROPERTY_FRIEND_OR_FOE,

	/* where */
	MM_PROPERTY_PLACE_X,
	MM_PROPERTY_PLACE_Y,
	MM_PROPERTY_PLACE_Z,
	MM_PROPERTY_TERRITORY,

	/* when */
	MM_PROPERTY_DAY,
	MM_PROPERTY_SUBJECTIVE_TIME,

	MM_PROPERTIES
};


typedef struct
{
    unsigned int id;

    /* a number of properties of the object */
    unsigned int length;
    unsigned int property_type[MM_MAX_OBJECT_PROPERTIES];
    unsigned int property_value[MM_MAX_OBJECT_PROPERTIES];

    /* how often has this feature been observed or known */
    unsigned int property_frequency[MM_MAX_OBJECT_PROPERTIES];
    unsigned int observations;
} mm_object;


int mm_obj_prop_index(mm_object * obj,
					  unsigned int property_type);
int mm_obj_prop_range(unsigned int property_type,
					  unsigned int * min, unsigned int * max);
int mm_obj_prop_add(mm_object * obj,
					unsigned int property_type,
					unsigned int property_value);
int mm_obj_prop_remove(mm_object * obj,
					   unsigned int property_type);
unsigned int mm_obj_prop_get(mm_object * obj,
							 unsigned int property_type);
unsigned int mm_obj_prop_get_index(mm_object * obj,
								   int index);
int mm_obj_prop_set(mm_object * obj,
					unsigned int property_type,
					unsigned int property_value);
void mm_obj_copy(mm_object *src, mm_object * dest);

#endif

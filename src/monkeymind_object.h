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
#define MM_MAX_OBJECT_PROPERTIES  8

enum property_types
{
	MM_PROPERTY_NONE = 0,
    MM_PROPERTY_OBJECT,
    MM_PROPERTY_NARRATIVE,
	MM_PROPERTY_DAY,
	MM_PROPERTY_FIRST_NAME,
	MM_PROPERTY_LAST_NAME,
	MM_PROPERTY_PLACE_X,
	MM_PROPERTY_PLACE_Y,
	MM_PROPERTY_PLACE_Z,
	MM_PROPERTY_TERRITORY,
	MM_PROPERTY_EMOTION,
	MM_PROPERTY_MEETER,
	MM_PROPERTY_MET,
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

#endif

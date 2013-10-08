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

#ifndef MONKEYMIND_H
#define MONKEYMIND_H

#include <stdio.h>
#include <string.h>
#include "monkeymind_rand.h"
#include "monkeymind_time.h"
#include "monkeymind_emotion.h"
#include "monkeymind_object.h"
#include "monkeymind_som.h"
#include "monkeymind_narrative.h"
#include "monkeymind_language.h"

/* social graph array index of the self */
#define MM_SELF                    0

/* size of narrative memory */
#define MM_SIZE_NARRATIVES         32

/* maximum size of the social graph */
#define MM_SIZE_SOCIAL_GRAPH       32

/* dimension of a 2D map within which the agent is located */
#define MM_SIZE_SPATIAL            32

/* the maximum number of abstract social categories */
#define MM_SIZE_SOCIAL_STEREOTYPES 32

#define MM_SEX_MALE                0
#define MM_SEX_FEMALE              1

/* dimension of the SOM used to classify social properties */
#define MM_SOCIAL_CATEGORIES_DIMENSION 64

/* radius used to update the social classification SOM */
#define MM_SOCIAL_CATEGORIES_RADIUS    4

/* name is stored as a single 32bit number,
   and contains sex, first name and last name */
#define MM_NAME(sex, first_name, surname) \
	( ((((sex)&255)<<8)|((first_name)&255)) | ((surname)<<16) )
#define MM_SEX(name)             (((name)>>8)&255)
#define MM_FIRST_NAME(name)      ((name)&255)
#define MM_SURNAME(name)         ((name)>>16)

/* different types of attention */
enum {
	MM_ATTENTION_SOCIAL_GRAPH = 0,
	MM_ATTENTION_SIZE
};

enum {
	MM_CATEGORY_FOF = 0,
	MM_CATEGORY_ATTRACTION,
	MM_CATEGORIES
};

typedef struct
{
	int value[MM_SOCIAL_CATEGORIES_DIMENSION*
			  MM_SOCIAL_CATEGORIES_DIMENSION];
} mm_som_categories;

typedef struct
{
    unsigned int id;

	mm_random_seed seed;

	/* intrinsic properties of the individual */
	mm_object * properties;

	/* self-organising map for classifying met individuals */
	mm_som social_categories;

	/* friend or foe categories */
	mm_som_categories category[MM_CATEGORIES];

	/* memory which may contain a number of narratives */
	mm_narrative narrative[MM_SIZE_NARRATIVES];

	/* details of each known agent */
    mm_object social_graph[MM_SIZE_SOCIAL_GRAPH];

	/* different types of attention */
	unsigned int attention[MM_ATTENTION_SIZE];

	/* abstracted individuals */
    mm_object social_stereotype[MM_SIZE_SOCIAL_STEREOTYPES];

	/* language machinery associated with each social graph entry */
	mm_language language[MM_SIZE_SOCIAL_GRAPH];

	/* spatial memory */
	mm_object spatial[MM_SIZE_SPATIAL*MM_SIZE_SPATIAL];
} monkeymind;

void mm_add_property(monkeymind * mind,
					 unsigned int property_type,
					 unsigned int property_value);
void mm_set_property(monkeymind * mind,
					 unsigned int property_type,
					 unsigned int property_value);
unsigned int mm_get_property(monkeymind * mind,
							 unsigned int property_type);
void mm_remove_property(monkeymind * mind,
						unsigned int property_type);
void mm_init(monkeymind * mind,
			 unsigned int id,
			 unsigned char sex,
			 unsigned char first_name,
			 unsigned char surname,
			 mm_random_seed * seed);

#endif

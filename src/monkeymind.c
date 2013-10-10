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

#include "monkeymind.h"

/* make an observation */
void mm_observe(monkeymind * mind, mm_object * observation)
{
	mm_events_add(&mind->events, observation);
}

/* adds a property to the individual */
void mm_add_property(monkeymind * mind,
					 unsigned int property_type,
					 unsigned int property_value)
{
	mm_obj_prop_add(mind->properties, property_type, property_value);
}

/* sets a property of the individualk */
void mm_set_property(monkeymind * mind,
					 unsigned int property_type,
 					 unsigned int property_value)
{
	mm_obj_prop_set(mind->properties, property_type, property_value);
}

/* gets a property of the individual */
unsigned int mm_get_property(monkeymind * mind,
							 unsigned int property_type)
{
	return mm_obj_prop_get(mind->properties, property_type);
}

/* remove a property type from an individual */
void mm_remove_property(monkeymind * mind,
						unsigned int property_type)
{
	mm_obj_prop_remove(mind->properties, property_type);
}

/* initialise the language machine */
static void mm_init_language(monkeymind * mind)
{
	int i, j, k;

	/* initially random language machine */
	for (i = 0; i < MM_SIZE_SOCIAL_GRAPH; i++) {
		for (j = 0; j < MM_SIZE_LANGUAGE_INSTRUCTIONS; j++) {
			mind->language[i].instruction[j].function =
				mm_rand(&mind->seed) & 255;
			mind->language[i].instruction[j].flags =
				mm_rand(&mind->seed) & 255;
			for (k = 0; k < MM_SIZE_LANGUAGE_ARGS; k++) {
				mind->language[i].instruction[j].argument[k] =
					mm_rand(&mind->seed);
			}
		}
	}
}

/* initialise the spatial memory */
static void mm_init_spatial(monkeymind * mind)
{
	int i;

	for (i = 0; i < MM_SIZE_SPATIAL*MM_SIZE_SPATIAL;i++) {
		mind->spatial[i].id = i;
	}
}

/* initially randomly assigned categories */
static void random_categories(mm_random_seed * seed,
							  int * categories)
{
	unsigned int i;

	for (i = 0;
		 i < MM_SOCIAL_CATEGORIES_DIMENSION*
			 MM_SOCIAL_CATEGORIES_DIMENSION;
		 i++) {
		categories[i] = (int)(mm_rand(seed)%3) - 1;
	}
}

/* initialises a mind */
void mm_init(monkeymind * mind,
			 unsigned int id,
			 unsigned char sex,
			 unsigned char first_name,
			 unsigned char surname,
			 mm_random_seed * seed)
{
	mm_object * individual;
	unsigned int name, i;

	mind->id = id;

	if (seed != NULL) {
		mm_rand_copy(seed, &mind->seed);
	}
	else {
		/* a default random seed */
		for (i = 0; i < 4; i++) {
			mind->seed.value[i] = i;
		}
	}

	name = MM_NAME(sex,first_name,surname);

	memset((void*)mind->narrative, '\0',
		   MM_SIZE_NARRATIVES * sizeof(mm_narrative));
	memset((void*)mind->social_graph, '\0',
		   MM_SIZE_SOCIAL_GRAPH * sizeof(mm_object));
	memset((void*)mind->social_stereotype, '\0',
		   MM_SIZE_SOCIAL_STEREOTYPES * sizeof(mm_object));
	memset((void*)&mind->properties, '\0', sizeof(mm_object));
	memset((void*)mind->spatial, '\0',
		   MM_SIZE_SPATIAL * MM_SIZE_SPATIAL * sizeof(mm_object));
	memset((void*)mind->attention, '\0',
		   MM_ATTENTION_SIZE*sizeof(unsigned int));

	for (i = 0; i < MM_CATEGORIES; i++) {
		random_categories(&mind->seed, mind->category[i].value);
	}

	mm_events_init(&mind->events);
	mm_init_language(mind);
	mm_init_spatial(mind);
	mm_som_init(&mind->social_categories,
				MM_SOCIAL_CATEGORIES_DIMENSION,
				MM_PROPERTIES,
				MM_SOCIAL_CATEGORIES_RADIUS,
				&mind->seed);

	mind->properties = &mind->social_graph[MM_SELF];

    individual = &mind->social_graph[MM_SELF];
	individual->length = 0;

	mm_obj_prop_add(individual,
					MM_PROPERTY_MEETER, id);

	mm_obj_prop_add(individual,
					MM_PROPERTY_MEETER_NAME, name);

	mm_obj_prop_add(individual,
					MM_PROPERTY_MET, id);

	mm_obj_prop_add(individual,
					MM_PROPERTY_MET_NAME, name);

	mm_add_property(mind, MM_PROPERTY_NAME, name);
}

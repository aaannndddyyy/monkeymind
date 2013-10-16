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

#include "monkeymind_unittests.h"

static void test_init()
{
	monkeymind mind;

	printf("test_init...");

	mm_init(&mind, 1000, MM_SEX_FEMALE,3,6,NULL);

	printf("Ok\n");
}

static void test_object_add_remove_properties()
{
	mm_object obj;
	n_uint i, prop_value;
	n_uint props[] = {
		3,6268,
		7,3568,
		1,56437,
		99,6732,
		40,1357,
		42,6267,
		23,536774,
		50,2435,
		49,23566,
		12,2356
	};

	printf("test_object_add_remove_properties...");

	/* Clear */
	obj.length = 0;
	for (i = 0; i < 10; i++) {
		obj.property_type[i] = 0;
		obj.property_value[i] = 0;
	}

	/* Add the properties */
	for (i = 0; i < 10; i++) {
		mm_obj_prop_add(&obj,props[i*2],props[i*2+1]);
		prop_value = mm_obj_prop_get(&obj,props[i*2]);
		assert(props[i*2+1] == prop_value);
	}


	/* Check that 10 properties were added */
	assert(obj.length == 10);

	/* Check that values are non-zero */
	for (i = 0; i < 10; i++) {
		assert(obj.property_type[i] > 0);
		assert(obj.property_value[i] > 0);
	}

	/* Check that the property types are in order */
	for (i = 1; i < 10; i++) {
		assert(obj.property_type[i-1] <= obj.property_type[i]);
	}

	/* Property 23 exists in the list */
	assert(mm_obj_prop_index(&obj,23) > -1);

	/* Remove it */
	mm_obj_prop_remove(&obj,23);

	/* Check that the list size is reduced */
	assert(obj.length = 9);

	/* Check that property 23 no longer exists */
	assert(mm_obj_prop_index(&obj,23) == -1);

	printf("Ok\n");
}

static void test_name()
{
	n_uint first_name = 10;
	n_uint sex = MM_SEX_FEMALE;
	n_uint surname = 125;
	n_uint name;
	n_uint extract_first_name, extract_surname, extract_sex;

	printf("test_name...");

	name = MM_NAME(sex,first_name,surname);

	extract_first_name = MM_FIRST_NAME(name);
	assert(extract_first_name == first_name);

	extract_sex = MM_SEX(name);
	assert(extract_sex == sex);

	extract_surname = MM_SURNAME(name);
	assert(extract_surname == surname);

	printf("Ok\n");
}

static void test_social_meet()
{
	monkeymind m0, m1, m2;
	n_int i;
	n_uint name;
	mm_object * g;

	printf("test_social_meet...");

	/* Create three agents */
	mm_init(&m0, 1000, MM_SEX_MALE, 10,20, NULL);
	mm_init(&m1, 2000, MM_SEX_FEMALE, 11,31, NULL);
	mm_init(&m2, 3000, MM_SEX_FEMALE, 7,8, NULL);

	/* Get the name of the first agent */
	name = mm_get_property(&m0, MM_PROPERTY_NAME);
	/* Check that it's what we expect */
	assert(MM_FIRST_NAME(name) == 10);
	assert(MM_SURNAME(name) == 20);

	/* Get the name of the second agent */
	name = mm_get_property(&m1, MM_PROPERTY_NAME);
	/* Check that it's what we expect */
	assert(MM_FIRST_NAME(name) == 11);
	assert(MM_SURNAME(name) == 31);

	/* Get the name of the third agent */
	name = mm_get_property(&m2, MM_PROPERTY_NAME);
	/* Check that it's what we expect */
	assert(MM_FIRST_NAME(name) == 7);
	assert(MM_SURNAME(name) == 8);

	/* First agent meets the second agent */
	mm_social_meet(&m0,&m1);
	/* First agent meets the third agent */
	mm_social_meet(&m0,&m2);
	/* First agent meets the second agent again */
	mm_social_meet(&m0,&m1);

	/* Check that two social graph entries have been made within
	   the first agent, and that other entries are blank.
	   Note that the first entry in the graph is the self. */
	for (i = MM_SELF+1; i < MM_SIZE_SOCIAL_GRAPH; i++) {
		if (i <= 3) {
			assert(SOCIAL_GRAPH_ENTRY_EXISTS(&m0,i));
		}
		else {
			assert(!SOCIAL_GRAPH_ENTRY_EXISTS(&m0,i));
		}
	}

	/* Get the first non-self social graph entry */
	g = &m0.social_graph[MM_SELF+1];
	/* Verify that the properties are what we expect */
	assert(mm_obj_prop_get(g,MM_PROPERTY_MET) == 2000);
	assert(mm_obj_prop_get(g,MM_PROPERTY_MEETER) == 1000);
	name = mm_obj_prop_get(g,MM_PROPERTY_MET_NAME);
	assert(MM_FIRST_NAME(name) == 11);
	assert(MM_SURNAME(name) == 31);
	name = mm_obj_prop_get(g,MM_PROPERTY_MEETER_NAME);
	assert(MM_FIRST_NAME(name) == 10);
	assert(MM_SURNAME(name) == 20);

	/* Get the second non-self social graph entry */
	g = &m0.social_graph[MM_SELF+2];
	/* Verify that the properties are what we expect */
	assert(mm_obj_prop_get(g,MM_PROPERTY_MET) == 3000);
	assert(mm_obj_prop_get(g,MM_PROPERTY_MEETER) == 1000);
	name = mm_obj_prop_get(g,MM_PROPERTY_MEETER_NAME);
	assert(MM_FIRST_NAME(name) == 10);
	assert(MM_SURNAME(name) == 20);
	name = mm_obj_prop_get(g,MM_PROPERTY_MET_NAME);
	assert(MM_FIRST_NAME(name) == 7);
	assert(MM_SURNAME(name) == 8);

	printf("Ok\n");
}

static void test_som()
{
	mm_som som;
	mm_random_seed seed;
	n_int i,j;
	n_uint dimension = 32;
	n_uint vector_length = 16;
	n_uint learning_radius = 4;
	n_byte vector[16];
	n_uint x, y;

	printf("test_som...");

	/* Set a random seed */
	for (i = 0; i < 4; i++) {
		seed.value[i] = i;
	}

	/* Initialise the SOM */
	mm_som_init(&som,
				dimension, vector_length,
				learning_radius, &seed);

	/* SOM learns randomly */
	for (i = 0; i < 10000; i++) {
		for (j = 0; j < vector_length; j++) {
			vector[j] = mm_rand(&seed)&255;
		}
		mm_som_update(&som, vector, &x, &y);
		mm_som_learn(&som, vector, x, y);
	}

	printf("Ok\n");
}

static void test_communicate_social_categorisation()
{
	monkeymind m0, m1;
	n_int c, i, ctr;

	printf("test_communicate_social_categorisation...");

	/* Initialise two agents */
	mm_init(&m0, 1000, MM_SEX_MALE, 10,20, NULL);
	mm_init(&m1, 2000, MM_SEX_FEMALE, 11,31, NULL);

	/* The two agents meet */
	mm_social_meet(&m0,&m1);

	/* Set the social categories to some known values for both agents */
	for (c = 0; c < MM_CATEGORIES; c++) {
		for (i = 0; i < MM_SOCIAL_CATEGORIES_DIMENSION*
				 MM_SOCIAL_CATEGORIES_DIMENSION; i++) {
			m0.category[c].value[i] = 10;
			m1.category[c].value[i] = 20;
		}
	}

	/* The first agent communicates its social categories to the second agent */
	mm_communicate_social_categorisation(&m0, 1, &m1);

	/* Check that the social categories for the second agent have
	   become more similar to those of the first agent */
	ctr = 0;
	for (c = 0; c < MM_CATEGORIES; c++) {
		for (i = 0; i < MM_SOCIAL_CATEGORIES_DIMENSION*
				 MM_SOCIAL_CATEGORIES_DIMENSION; i++) {
			/* Should have reduced by one or two */
			if (m1.category[c].value[i] < 20) {
				ctr++; /* Count the number of changed categories */
				assert(m1.category[c].value[i] >= 18);
			}
		}
	}

	/* Check the total number of changed social categories for the second agent
	   is what we expect */
	assert(ctr > MM_SOCIAL_CATEGORIES_RADIUS*2*
		   MM_CATEGORIES);
	assert(ctr < MM_SOCIAL_CATEGORIES_RADIUS*
		   MM_SOCIAL_CATEGORIES_RADIUS*4*MM_CATEGORIES);

	/* Set the social categories to some known values for both agents,
	   in the reverse order to previously.  This will check that
	   alterations to categories work in both positive and negative directions. */
	for (c = 0; c < MM_CATEGORIES; c++) {
		for (i = 0; i < MM_SOCIAL_CATEGORIES_DIMENSION*
				 MM_SOCIAL_CATEGORIES_DIMENSION; i++) {
			m0.category[c].value[i] = 20;
			m1.category[c].value[i] = 10;
		}
	}

	/* The first agent communicates its social categories to the second agent */
	mm_communicate_social_categorisation(&m0, 1, &m1);

	/* Check that the social categories for the second agent have
	   become more similar to those of the first agent */
	ctr = 0;
	for (c = 0; c < MM_CATEGORIES; c++) {
		for (i = 0; i < MM_SOCIAL_CATEGORIES_DIMENSION*
				 MM_SOCIAL_CATEGORIES_DIMENSION; i++) {
			/* Should have increased by one or two */
			if (m1.category[c].value[i] > 10) {
				ctr++; /* Count the number of changed categories */
				assert(m1.category[c].value[i] <= 12);
			}
		}
	}

	/* Check the total number of changed social categories for the second agent
	   is what we expect */
	assert(ctr > MM_SOCIAL_CATEGORIES_RADIUS*2*
		   MM_CATEGORIES);
	assert(ctr < MM_SOCIAL_CATEGORIES_RADIUS*
		   MM_SOCIAL_CATEGORIES_RADIUS*4*MM_CATEGORIES);

	printf("Ok\n");
}

static void test_events()
{
	n_int i;
	mm_object * test1, * test2;
	mm_events events;
	mm_object observation1, observation2;
	/* Some properties of the first event */
	n_uint props1[] = {
		3,6268,
		7,3568,
		1,56437,
		99,6732,
		40,1357,
		42,6267,
		23,536774,
		50,2435,
		49,23566,
		12,2356
	};
	/* Some properties of the first event */
	n_uint props2[] = {
		3,2521,
		7,7548,
		1,26357,
		99,8392,
		40,9357,
		42,867,
		23,36794,
		50,2235,
		49,22566,
		12,9756
	};

	printf("test_events...");

	/* Create an event sequence and some observation events
	   to be inserted into it */
	mm_events_init(&events);
	mm_obj_init(&observation1);
	mm_obj_init(&observation2);

	/* Check that there are no events in the sequence */
	assert(mm_events_max(&events) == 0);
	/* Check that the observation events are empty */
	assert(!mm_obj_exists(&observation1));
	assert(!mm_obj_exists(&observation2));

	/* Add properties to the observation events */
	for (i = 0; i < 10; i++) {
		mm_obj_prop_add(&observation1,props1[i*2],props1[i*2+1]);
		mm_obj_prop_add(&observation2,props2[i*2],props2[i*2+1]);
	}
	/* Check that the observation events have some contents */
	assert(mm_obj_exists(&observation1));
	assert(mm_obj_exists(&observation2));

	/* Add the first event to the sequence */
	mm_events_add(&events,&observation1);
	/* Check that the sequence length increases */
	assert(mm_events_max(&events) == 1);
	/* Add the second event to the sequence */
	mm_events_add(&events,&observation2);
	/* Check that the sequence length increases */
	assert(mm_events_max(&events) == 2);

	/* Get the first and second events from the sequence */
	test1 = mm_events_get(&events, 0);
	test2 = mm_events_get(&events, 1);
	/* Check that there is no third event */
	assert(mm_events_get(&events, 2)==0);

	/* Check that the returned events are the same as the original observations */
	assert(mm_obj_cmp(&observation1, test1) == 0);
	assert(mm_obj_cmp(&observation2, test2) == 0);

	printf("Ok\n");
}

static void test_narrative()
{
	n_int i;
	mm_object * test1, * test2;
	mm_narrative narrative;
	mm_object observation1, observation2;
	/* Some properties of the first observation */
	n_uint props1[] = {
		3,6268,
		7,3568,
		1,56437,
		99,6732,
		40,1357,
		42,6267,
		23,536774,
		50,2435,
		49,23566,
		12,2356
	};
	/* Some properties of the second observation */
	n_uint props2[] = {
		3,2521,
		7,7548,
		1,26357,
		99,8392,
		40,9357,
		42,867,
		23,36794,
		50,2235,
		49,22566,
		12,9756
	};

	printf("test_narrative...");

	/* Create a narrative and some observation steps to insert into it */
	mm_narrative_init(&narrative, 1234);
	mm_obj_init(&observation1);
	mm_obj_init(&observation2);

	/* Add properties to the observation steps */
	for (i = 0; i < 10; i++) {
		mm_obj_prop_add(&observation1, props1[i*2], props1[i*2+1]);
		mm_obj_prop_add(&observation2, props2[i*2], props2[i*2+1]);
	}
	/* Check that the observations have non-zero properties */
	assert(mm_obj_exists(&observation1));
	assert(mm_obj_exists(&observation2));

	/* Add a first step to the narrative */
	assert(mm_narrative_add(&narrative, &observation1, 1,2,3) == 0);
	assert(narrative.length == 1);
	/* Add a second step to the narrative */
	assert(mm_narrative_add(&narrative, &observation2, 4,5,6) == 0);
	assert(narrative.length == 2);

	/* Retrieve the first and second steps from the narrative */
	test1 = mm_narrative_get(&narrative, 0);
	test2 = mm_narrative_get(&narrative, 1);
	/* Check that they're the same as the original observations */
	assert(mm_obj_cmp(&observation1, test1) == 0);
	assert(mm_obj_cmp(&observation2, test2) == 0);

	/* Remove the first step from the narrative */
	assert(mm_narrative_remove(&narrative, 0) == 0);
	/* Check that the length is reduced */
	assert(narrative.length == 1);
	/* Get the first step in the narrative */
	test1 = mm_narrative_get(&narrative, 0);
	/* Check that it corresponds to the second original observation */
	assert(mm_obj_cmp(&observation2, test1) == 0);

	printf("Ok\n");
}

static void test_language_machine()
{
	monkeymind m0, m1;
	n_int tot_diff = 0;
	n_uint t, i, j, test_x, test_y, x, y, w, n, l;
	const n_uint test_dimension = 10;
	mm_random_seed seed;
	const n_uint subimage_width = 100;
	const n_uint image_width = test_dimension*subimage_width;
	n_byte img[image_width*image_width*3];
	n_uint attention0, ctr=0;
	mm_language_machine * lang0, prev_lang0;
	char filename[256];

	printf("test_language_machine...");

	memset((void*)img,'\0',image_width*image_width*3);

	for (test_y = 0; test_y < test_dimension; test_y++) {
		for (test_x = 0; test_x < test_dimension; test_x++, ctr++) {

			for (i = 0; i < 4; i++) {
				seed.value[i] = ctr*4+i;
			}

			/* Create two agents */
			mm_init(&m0, 1000, MM_SEX_MALE, 10,20, &seed);
			mm_init(&m1, 2000, MM_SEX_FEMALE, 11,31, &seed);

			/* agents engage in dialogue for a number of time steps */
			for (t = 2; t < subimage_width; t++) {
				mm_dialogue(&m0, &m1);
				attention0 = m0.attention[MM_ATTENTION_SOCIAL_GRAPH];
				lang0 = &m0.language[attention0];

				/* draw the time series of language machine states */
				y = test_y*subimage_width + t;
				for (w = 2; w < subimage_width; w++) {
					x = test_x*subimage_width + w;
					n = (y*image_width + x)*3;
					l = w*MM_SIZE_LANGUAGE_INSTRUCTIONS/subimage_width;

					if (((n_byte*)lang0)[l*3] != ((n_byte*)&prev_lang0)[l*3]) {
						img[n] = 255;
					}
					if (((n_byte*)lang0)[l*3+1] != ((n_byte*)&prev_lang0)[l*3+1]) {
						img[n+1] = 255;
					}
					if (((n_byte*)lang0)[l*3+2] != ((n_byte*)&prev_lang0)[l*3+2]) {
						img[n+2] = 255;
					}
				}

				if (t > 2) {
					/* count the number of differences */
					for (i = 0; i < MM_SIZE_LANGUAGE_INSTRUCTIONS; i++) {
						if (prev_lang0.instruction[i].function !=
							lang0->instruction[i].function) tot_diff++;
						if (prev_lang0.instruction[i].output !=
							lang0->instruction[i].output) tot_diff++;
						for (j = 0; j < MM_SIZE_LANGUAGE_INSTRUCTIONS; j++) {
							if (prev_lang0.instruction[i].argument[j] !=
								lang0->instruction[i].argument[j]) {
								tot_diff++;
							}
						}
					}
				}
				memcpy((void*)&prev_lang0, lang0,
					   sizeof(mm_language_machine));
			}
		}
	}

	sprintf(filename,"%s","/tmp/monkeymind_language_machines.png");

	write_png_file((char*)filename, (int)image_width, (int)image_width,
				   (unsigned char*)img);

	/* some changes should have occurred */
	assert(tot_diff > 0);

	printf("Ok\n");
}

void mm_run_tests()
{
	test_init();
	test_object_add_remove_properties();
	test_name();
	test_social_meet();
	test_som();
	test_communicate_social_categorisation();
	test_events();
	test_narrative();
	test_language_machine();

	printf("All tests passed\n");
}

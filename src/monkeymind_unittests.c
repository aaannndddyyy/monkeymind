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
	unsigned int i, prop_value;
	unsigned int props[] = {
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

	/* clear */
	obj.length = 0;
	for (i = 0; i < 10; i++) {
		obj.property_type[i] = 0;
		obj.property_value[i] = 0;
	}

	/* add the properties */
	for (i = 0; i < 10; i++) {
		mm_obj_prop_add(&obj,props[i*2],props[i*2+1]);
		prop_value = mm_obj_prop_get(&obj,props[i*2]);
		assert(props[i*2+1] == prop_value);
	}


	/* check that 10 properties were added */
	assert(obj.length == 10);

	/* check that values are non-zero */
	for (i = 0; i < 10; i++) {
		assert(obj.property_type[i] > 0);
		assert(obj.property_value[i] > 0);
	}

	/* check that the property types are in order */
	for (i = 1; i < 10; i++) {
		assert(obj.property_type[i-1] <= obj.property_type[i]);
	}

	/* property 23 exists in the list */
	assert(mm_obj_prop_index(&obj,23) > -1);

	/* remove it */
	mm_obj_prop_remove(&obj,23);

	/* check that the list size is reduced */
	assert(obj.length = 9);

	/* check that property 23 no longer exists */
	assert(mm_obj_prop_index(&obj,23) == -1);

	printf("Ok\n");
}

static void test_name()
{
	unsigned int first_name = 10;
	unsigned int sex = MM_SEX_FEMALE;
	unsigned int surname = 125;
	unsigned int name;
	unsigned int extract_first_name, extract_surname, extract_sex;

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
	int i;
	unsigned int name;
	mm_object * g;

	printf("test_social_meet...");

	mm_init(&m0, 1000, MM_SEX_MALE, 10,20, NULL);
	mm_init(&m1, 2000, MM_SEX_FEMALE, 11,31, NULL);
	mm_init(&m2, 3000, MM_SEX_FEMALE, 7,8, NULL);

	name = mm_get_property(&m0, MM_PROPERTY_NAME);
	assert(MM_FIRST_NAME(name) == 10);
	assert(MM_SURNAME(name) == 20);

	name = mm_get_property(&m1, MM_PROPERTY_NAME);
	assert(MM_FIRST_NAME(name) == 11);
	assert(MM_SURNAME(name) == 31);

	name = mm_get_property(&m2, MM_PROPERTY_NAME);
	assert(MM_FIRST_NAME(name) == 7);
	assert(MM_SURNAME(name) == 8);

	mm_social_meet(&m0,&m1);
	mm_social_meet(&m0,&m2);
	mm_social_meet(&m0,&m1);

	for (i = MM_SELF+1; i < MM_SIZE_SOCIAL_GRAPH; i++) {
		if (i <= 3) {
			assert(SOCIAL_GRAPH_ENTRY_EXISTS(&m0,i));
		}
		else {
			assert(!SOCIAL_GRAPH_ENTRY_EXISTS(&m0,i));
		}
	}

	g = &m0.social_graph[MM_SELF+1];
	assert(mm_obj_prop_get(g,MM_PROPERTY_MET) == 2000);
	assert(mm_obj_prop_get(g,MM_PROPERTY_MEETER) == 1000);
	name = mm_obj_prop_get(g,MM_PROPERTY_MET_NAME);
	assert(MM_FIRST_NAME(name) == 11);
	assert(MM_SURNAME(name) == 31);
	name = mm_obj_prop_get(g,MM_PROPERTY_MEETER_NAME);
	assert(MM_FIRST_NAME(name) == 10);
	assert(MM_SURNAME(name) == 20);

	g = &m0.social_graph[MM_SELF+2];
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
	int i,j;
	unsigned int dimension = 32;
	unsigned int vector_length = 16;
	unsigned int learning_radius = 4;
	unsigned char vector[16];
	unsigned int x, y;

	printf("test_som...");

	for (i = 0; i < 4; i++) {
		seed.value[i] = i;
	}

	mm_som_init(&som,
				dimension, vector_length,
				learning_radius, &seed);

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
	int c, i, ctr;

	printf("test_communicate_social_categorisation...");

	mm_init(&m0, 1000, MM_SEX_MALE, 10,20, NULL);
	mm_init(&m1, 2000, MM_SEX_FEMALE, 11,31, NULL);

	mm_social_meet(&m0,&m1);

	for (c = 0; c < MM_CATEGORIES; c++) {
		for (i = 0; i < MM_SOCIAL_CATEGORIES_DIMENSION*
				 MM_SOCIAL_CATEGORIES_DIMENSION; i++) {
			m0.category[c].value[i] = 10;
			m1.category[c].value[i] = 20;
		}
	}

	mm_communicate_social_categorisation(&m0, 1, &m1);

	ctr = 0;
	for (c = 0; c < MM_CATEGORIES; c++) {
		for (i = 0; i < MM_SOCIAL_CATEGORIES_DIMENSION*
				 MM_SOCIAL_CATEGORIES_DIMENSION; i++) {
			if (m1.category[c].value[i] < 20) {
				ctr++;
				assert(m1.category[c].value[i] >= 18);
			}
		}
	}

	assert(ctr > MM_SOCIAL_CATEGORIES_RADIUS*2*
		   MM_CATEGORIES);
	assert(ctr < MM_SOCIAL_CATEGORIES_RADIUS*
		   MM_SOCIAL_CATEGORIES_RADIUS*4*MM_CATEGORIES);

	for (c = 0; c < MM_CATEGORIES; c++) {
		for (i = 0; i < MM_SOCIAL_CATEGORIES_DIMENSION*
				 MM_SOCIAL_CATEGORIES_DIMENSION; i++) {
			m0.category[c].value[i] = 20;
			m1.category[c].value[i] = 10;
		}
	}

	mm_communicate_social_categorisation(&m0, 1, &m1);

	ctr = 0;
	for (c = 0; c < MM_CATEGORIES; c++) {
		for (i = 0; i < MM_SOCIAL_CATEGORIES_DIMENSION*
				 MM_SOCIAL_CATEGORIES_DIMENSION; i++) {
			if (m1.category[c].value[i] > 10) {
				ctr++;
				assert(m1.category[c].value[i] <= 12);
			}
		}
	}

	assert(ctr > MM_SOCIAL_CATEGORIES_RADIUS*2*
		   MM_CATEGORIES);
	assert(ctr < MM_SOCIAL_CATEGORIES_RADIUS*
		   MM_SOCIAL_CATEGORIES_RADIUS*4*MM_CATEGORIES);

	printf("Ok\n");
}

static void test_events()
{
	int i;
	mm_object * test1, * test2;
	mm_events events;
	mm_object observation1, observation2;
	unsigned int props1[] = {
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
	unsigned int props2[] = {
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

	mm_events_init(&events);
	mm_obj_init(&observation1);
	mm_obj_init(&observation2);

	assert(mm_events_max(&events) == 0);
	assert(!mm_obj_exists(&observation1));

	/* add properties to the observation */
	for (i = 0; i < 10; i++) {
		mm_obj_prop_add(&observation1,props1[i*2],props1[i*2+1]);
		mm_obj_prop_add(&observation2,props2[i*2],props2[i*2+1]);
	}
	assert(mm_obj_exists(&observation1));
	assert(mm_obj_exists(&observation2));

	mm_events_add(&events,&observation1);
	assert(mm_events_max(&events) == 1);
	mm_events_add(&events,&observation2);
	assert(mm_events_max(&events) == 2);

	test1 = mm_events_get(&events, 0);
	test2 = mm_events_get(&events, 1);
	assert(mm_events_get(&events, 2)==0);

	assert(mm_obj_cmp(&observation1, test1) == 0);
	assert(mm_obj_cmp(&observation2, test2) == 0);

	printf("Ok\n");
}

static void test_narrative()
{
	int i;
	mm_object * test1, * test2;
	mm_narrative narrative;
	mm_object observation1, observation2;
	unsigned int props1[] = {
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
	unsigned int props2[] = {
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

	mm_narrative_init(&narrative,1234);
	mm_obj_init(&observation1);
	mm_obj_init(&observation2);

	/* add properties to the observation */
	for (i = 0; i < 10; i++) {
		mm_obj_prop_add(&observation1,props1[i*2],props1[i*2+1]);
		mm_obj_prop_add(&observation2,props2[i*2],props2[i*2+1]);
	}
	assert(mm_obj_exists(&observation1));
	assert(mm_obj_exists(&observation2));

	assert(mm_narrative_add(&narrative, &observation1, 1,2,3)==0);
	assert(narrative.length==1);
	assert(mm_narrative_add(&narrative, &observation2, 4,5,6)==0);
	assert(narrative.length==2);

	test1 = mm_narrative_get(&narrative, 0);
	test2 = mm_narrative_get(&narrative, 1);
	assert(mm_obj_cmp(&observation1, test1) == 0);
	assert(mm_obj_cmp(&observation2, test2) == 0);

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

	printf("All tests passed\n");
}

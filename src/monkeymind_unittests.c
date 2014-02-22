/****************************************************************

 Monkeymind - an experimental cogitive architecture

 =============================================================

 Copyright 2013-2014 Bob Mottram

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

static void test_spatial()
{
    monkeymind mind;
    n_int i;

    printf("test_spatial...");

    mm_init(&mind, 1000, MM_SEX_FEMALE,3,6,NULL);

    for (i = 0; i < MM_SIZE_SPATIAL*MM_SIZE_SPATIAL; i++) {
        assert(mind.spatial[i].id == i);
    }

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

static void test_episodic()
{
    n_int i;
    mm_object * test1, * test2;
    mm_episodic events;
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

    printf("test_episodic...");

    /* Create an event sequence and some observation events
       to be inserted into it */
    mm_episodic_init(&events);
    mm_obj_init(&observation1);
    mm_obj_init(&observation2);

    /* Check that there are no events in the sequence */
    assert(mm_episodic_max(&events) == 0);
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
    mm_episodic_add(&events,&observation1);
    /* Check that the sequence length increases */
    assert(mm_episodic_max(&events) == 1);
    /* Add the second event to the sequence */
    mm_episodic_add(&events,&observation2);
    /* Check that the sequence length increases */
    assert(mm_episodic_max(&events) == 2);

    /* Get the first and second events from the sequence */
    test1 = mm_episodic_get(&events, 0);
    test2 = mm_episodic_get(&events, 1);
    /* Check that there is no third event */
    assert(mm_episodic_get(&events, 2)==0);

    /* Check that the returned events are the same as the original observations */
    assert(mm_obj_cmp(&observation1, test1) == 0);
    assert(mm_obj_cmp(&observation2, test2) == 0);

    printf("Ok\n");
}

static void test_tale()
{
    n_int i;
    mm_object * test1, * test2;
    mm_tale tale;
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

    printf("test_tale...");

    /* Create a tale and some observation steps to insert into it */
    mm_tale_init(&tale, 1234);
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

    /* Add a first step to the tale */
    assert(mm_tale_add(&tale, &observation1, 1,2,3) == 0);
    assert(tale.length == 1);
    /* Add a second step to the tale */
    assert(mm_tale_add(&tale, &observation2, 4,5,6) == 0);
    assert(tale.length == 2);

    /* Retrieve the first and second steps from the tale */
    test1 = mm_tale_get(&tale, 0);
    test2 = mm_tale_get(&tale, 1);
    /* Check that they're the same as the original observations */
    assert(mm_obj_cmp(&observation1, test1) == 0);
    assert(mm_obj_cmp(&observation2, test2) == 0);

    /* Remove the first step from the tale */
    assert(mm_tale_remove(&tale, 0) == 0);
    /* Check that the length is reduced */
    assert(tale.length == 1);
    /* Get the first step in the tale */
    test1 = mm_tale_get(&tale, 0);
    /* Check that it corresponds to the second original observation */
    assert(mm_obj_cmp(&observation2, test1) == 0);

    printf("Ok\n");
}

static void test_narratives()
{
    n_int i;
    mm_tale tale;
    mm_narratives narratives;

    printf("test_narratives...");

    mm_narratives_init(&narratives);

    for (i = 0; i < 10; i++) {
        mm_tale_init(&tale, i);
        assert(mm_narratives_add(&narratives, &tale) == 0);
        assert(narratives.length == i+1);
    }

    assert(mm_narratives_get(&narratives, 2) == 2);
    assert(mm_narratives_get(&narratives, 9) == 9);

    assert(mm_narratives_remove(&narratives, 3) == 0);
    assert(narratives.length == 9);
    assert(mm_narratives_get(&narratives, 2) == 2);
    assert(mm_narratives_get(&narratives, 4) == 3);

    printf("Ok\n");
}

static void test_language_machine()
{
    monkeymind m0, m1;
    n_int diff, tot_diff = 0, changed_time_steps, multi_step_changes = 0;
    n_uint t, i, test_x, test_y, x, y, w, n, instr, arg;
    const n_uint test_dimension = 16;
    mm_random_seed seed;
    const n_uint subimage_width = 64;
    const n_uint image_width = test_dimension * subimage_width;
    n_byte img[image_width*image_width*3];
    n_uint attention0, ctr = 0;
    mm_language_machine * lang0, prev_lang0;
    char filename[256];

    printf("test_language_machine...");

    for (i = 0; i < image_width*image_width*3; i++) {
        img[i] = 255;
    }

    for (test_y = 0; test_y < test_dimension; test_y++) {
        for (test_x = 0; test_x < test_dimension; test_x++, ctr++) {

            for (i = 0; i < 4; i++) {
                seed.value[i] = ctr*8+i;
            }

            /* Create two agents */
            mm_init(&m0, 1000, MM_SEX_MALE, 10,20, &seed);

            for (i = 0; i < 4; i++) {
                seed.value[i] = ctr*8+4+i;
            }

            mm_init(&m1, 2000, MM_SEX_FEMALE, 11,31, &seed);

            /* agents engage in dialogue for a number of time steps */
            changed_time_steps = 0;
            for (t = 0; t < subimage_width; t++) {
                mm_dialogue(&m0, &m1);
                mm_dialogue_internal(&m0);
                attention0 = m0.attention[MM_ATTENTION_SOCIAL_GRAPH];
                lang0 = &m0.language[attention0];

                /* draw the time series of language machine states */
                diff = 0;
                y = test_y*subimage_width + t;

                w = 0;
                for (instr = 0; instr < MM_SIZE_LANGUAGE_INSTRUCTIONS; instr++) {
                    for (arg = 0; arg < MM_SIZE_LANGUAGE_ARGS; arg++, w++) {
                        x = test_x*subimage_width +
                            (w*subimage_width/
                             (MM_SIZE_LANGUAGE_INSTRUCTIONS*MM_SIZE_LANGUAGE_ARGS));
                        n = (y*image_width + x)*3;

                        if ((t > 2) && (instr > 2)) {
                            if (lang0->instruction[instr].argument[arg] !=
                                prev_lang0.instruction[instr].argument[arg]) {

                                img[n] = 50;
                                img[n+1] = 50;
                                img[n+2] = 50;
                                diff++;
                            }
                        }
                        else {
                            img[n] = 0;
                            img[n+1] = 0;
                            img[n+2] = 0;
                        }
                    }
                }

                if (t > 1) {
                    tot_diff += diff;
                    if (diff > 0) changed_time_steps++;
                }
                memcpy((void*)&prev_lang0, (void*)lang0,
                       sizeof(mm_language_machine));
            }
            if (changed_time_steps > 1) multi_step_changes++;
        }
    }

    sprintf(filename,"%s","/tmp/monkeymind_language_machines.png");

    write_png_file((char*)filename, (int)image_width, (int)image_width,
                   (unsigned char*)img);

    /* some changes should have occurred */
    assert(tot_diff > 0);

    /* some runs have changes on multiple time steps */
    assert(multi_step_changes > 0);

    printf("Ok\n");
}

static void test_language_get_address()
{
    printf("test_language_get_address...");

    n_uint max = addresses_per_machine();

    assert(mm_language_get_address(max, 0) == max);
    assert(mm_language_get_address(max/2, 0) == max/2);
    assert(mm_language_get_address(max*2+3, 0) == 3);
    assert(mm_language_get_address(max*2-1, 0) == max*2-1);
    assert(mm_language_get_address(-9, 0) == 9);
    assert(mm_language_get_address(0, 0) == 0);

    printf("Ok\n");
}

static void test_language_get_data()
{
    printf("test_language_get_data...");

    n_uint max = addresses_per_machine();
    mm_language_machine m0, m1;
    mm_random_seed seed0, seed1;
    n_uint i;
    n_int value, * mm0, * mm1;

    for (i = 0; i < 4; i++) {
        seed0.value[i] = i;
        seed1.value[i] = i+4;
    }

    mm_language_init(&m0, &seed0);
    mm_language_init(&m1, &seed1);

    mm0 = (n_int*)&m0;
    mm1 = (n_int*)&m1;

    for (i = 0; i < max*3; i++) {
        value = mm_language_get_data(&m0, &m1, NULL, 0, i);
        if (i < max) {
            assert(value == mm0[i]);
        }
        else {
            if (i < max*2) {
                assert(value == mm1[i-max]);
            }
            else {
                assert(value == mm0[i-(max*2)]);
            }
        }
    }

    printf("Ok\n");
}

static void test_language_set_data()
{
    printf("test_language_set_data...");

    n_uint max = addresses_per_machine();
    mm_language_machine m0, m1;
    mm_random_seed seed0, seed1;
    n_uint i;
    n_int * mm0, * mm1;

    for (i = 0; i < 4; i++) {
        seed0.value[i] = i;
        seed1.value[i] = i+4;
    }

    mm_language_init(&m0, &seed0);
    mm_language_init(&m1, &seed1);

    mm0 = (n_int*)&m0;
    mm1 = (n_int*)&m1;

    for (i = 0; i < max*3; i++) {
        mm_language_set_data(&m0, &m1, NULL, 0, i, i);
        if (i < max) {
            assert(mm0[i] == i);
        }
        else {
            if (i < max*2) {
                assert(mm1[i-max] == i);
            }
            else {
                assert(mm0[i-(max*2)] == i);
            }
        }
    }

    printf("Ok\n");
}

void mm_run_tests()
{
    test_init();
    test_spatial();
    test_object_add_remove_properties();
    test_name();
    test_social_meet();
    test_som();
    test_communicate_social_categorisation();
    test_episodic();
    test_tale();
    test_narratives();
    test_language_get_address();
    test_language_get_data();
    test_language_set_data();
    test_language_machine();

    printf("All tests passed\n");
}

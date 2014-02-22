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
    mm_episodic_add(&mind->episodic_buffer, observation);
}

/* adds a property to the individual */
void mm_add_property(monkeymind * mind,
                     n_uint property_type,
                     n_uint property_value)
{
    mm_obj_prop_add(mind->properties, property_type, property_value);
}

/* sets a property of the individualk */
void mm_set_property(monkeymind * mind,
                     n_uint property_type,
                     n_uint property_value)
{
    mm_obj_prop_set(mind->properties, property_type, property_value);
}

/* gets a property of the individual */
n_uint mm_get_property(monkeymind * mind,
                             n_uint property_type)
{
    return mm_obj_prop_get(mind->properties, property_type);
}

/* remove a property type from an individual */
void mm_remove_property(monkeymind * mind,
                        n_uint property_type)
{
    mm_obj_prop_remove(mind->properties, property_type);
}

/* initialise the language machine */
static void mm_init_language(monkeymind * mind)
{
    n_int i;

    for (i = 0; i < MM_SIZE_SOCIAL_GRAPH; i++) {
        mm_language_init(&mind->language[i], &mind->seed);
    }
}

/* initialise the spatial memory */
static void mm_init_spatial(monkeymind * mind)
{
    n_int i;

    for (i = 0; i < MM_SIZE_SPATIAL*MM_SIZE_SPATIAL; i++) {
        mind->spatial[i].id = i;
    }
}

/* initially randomly assigned categories */
static void random_categories(mm_random_seed * seed,
                              n_int * categories)
{
    n_uint i;

    for (i = 0;
         i < MM_SOCIAL_CATEGORIES_DIMENSION*
             MM_SOCIAL_CATEGORIES_DIMENSION;
         i++) {
        categories[i] = (n_int)(mm_rand(seed)%3) - 1;
    }
}

/* initialises a mind */
void mm_init(monkeymind * mind,
             n_uint id,
             n_byte sex,
             n_byte first_name,
             n_byte surname,
             mm_random_seed * seed)
{
    mm_object * individual;
    n_uint name, i;

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

    memset((void*)&mind->narratives, '\0',
           sizeof(mm_narratives));
    memset((void*)mind->social_graph, '\0',
           MM_SIZE_SOCIAL_GRAPH * sizeof(mm_object));
    memset((void*)mind->social_stereotype, '\0',
           MM_SIZE_SOCIAL_STEREOTYPES * sizeof(mm_object));
    memset((void*)&mind->properties, '\0', sizeof(mm_object));
    memset((void*)mind->spatial, '\0',
           MM_SIZE_SPATIAL * MM_SIZE_SPATIAL * sizeof(mm_object));
    memset((void*)mind->attention, '\0',
           MM_ATTENTION_SIZE*sizeof(n_uint));

    for (i = 0; i < MM_CATEGORIES; i++) {
        random_categories(&mind->seed, mind->category[i].value);
    }

    mm_episodic_init(&mind->episodic_buffer);
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

    /* no cognitive system specified */
    mind->cognitive_system_state = 0;
    mind->cognitive_system_state_size = 0;

    /* during dialog where to start in the language program
       and how many steps to continue for */
    mind->language_ptr = 0;
}

/* A dialogue between two language machines.
   These would typically be the inner and outer systems */
static void mm_language_dialogue(monkeymind * mind,
                                 mm_language_machine * m0,
                                 mm_language_machine * m1,
                                 n_byte * data, n_uint data_size,
                                 mm_episodic * e0,
                                 mm_episodic * e1)
{
    n_uint index = 0;
    n_int ctr = (n_int)mind->language_ptr; /* program counter */
    n_int instruction_type;

    /* for each step in the language program */
    while (index < MM_SIZE_LANGUAGE_INSTRUCTIONS) {
        instruction_type = m0->instruction[ctr].function%MM_INSTRUCTIONS;
        switch(instruction_type) {
        case MM_INSTRUCTION_MATHS: {
            mm_language_maths(m0, m1, data, data_size, ctr);
            break;
        }
        case MM_INSTRUCTION_COPY: {
            mm_language_copy(m0, m1, data, data_size, ctr);
            break;
        }
        case MM_INSTRUCTION_JUMP: {
            ctr =
                (ctr + mm_language_jump(m0, m1, data, data_size, ctr)) %
                MM_SIZE_LANGUAGE_INSTRUCTIONS;
            if (ctr < 0) ctr += MM_SIZE_LANGUAGE_INSTRUCTIONS;
            break;
        }
        }

        index++;
        ctr++;

        /* keep the program counter within range */
        if (ctr >= MM_SIZE_LANGUAGE_INSTRUCTIONS) {
            ctr -= MM_SIZE_LANGUAGE_INSTRUCTIONS;
        }

        /* the program stops here */
        if (instruction_type == MM_INSTRUCTION_HALT) {
            break;
        }
    }

    /* record the program counter position */
    mind->language_ptr = (n_uint)ctr;
}

/* external dialogue between two agents */
void mm_dialogue(monkeymind * mind0, monkeymind * mind1)
{
    n_uint attention0 = mind0->attention[MM_ATTENTION_SOCIAL_GRAPH];
    n_uint attention1 = mind1->attention[MM_ATTENTION_SOCIAL_GRAPH];
    mm_language_machine * m0, * m1;

    if (mind0 == mind1) {
        /* dialogue with the self */
        m0 = &mind0->language[MM_SELF];
    }
    else {
        /* dialogue with another */
        m0 = &mind0->language[attention0];
    }

    m1 = &mind0->language[attention1];

    /* A talks to B */
    mm_language_dialogue(mind0, m0, m1,
                         mind0->cognitive_system_state,
                         mind0->cognitive_system_state_size,
                         &mind0->episodic_buffer,
                         &mind1->episodic_buffer);

    /* B replies to A */
    mm_language_dialogue(mind1, m1, m0,
                         mind1->cognitive_system_state,
                         mind1->cognitive_system_state_size,
                         &mind1->episodic_buffer,
                         &mind0->episodic_buffer);
}

/* internal dialogue within an agent */
void mm_dialogue_internal(monkeymind * mind)
{
    mm_dialogue(mind, mind);
}

/* transmits a narrative from a speaker to a listener */
n_int mm_dialogue_narrative(monkeymind * speaker, monkeymind * listener)
{
    n_uint speaker_attention = speaker->attention[MM_ATTENTION_NARRATIVE];
    mm_tale * tale[2];
    n_int listener_narrative_index;

    if (speaker->narratives.length == 0) return -1;

    /* get the tale which is the speaker's current focus of attention */
    tale[0] =
        &speaker->narratives.tale[speaker_attention %
                                  speaker->narratives.length];

    tale[0]->times_told++;

    /* does the listener already have the same narrative? */
    listener_narrative_index =
        mm_narratives_get(&listener->narratives, tale[0]->id);
    if (listener_narrative_index > -1) {
        /* the tale was already heard */
        tale[1] = &listener->narratives.tale[listener_narrative_index];
        tale[1]->times_heard++;
        return 0;
    }

    /* listener has not heard the tale */
    if (listener->narratives.length < MM_SIZE_NARRATIVES) {
        /* narrative memory is not yet full */
        listener_narrative_index = listener->narratives.length;
        mm_narratives_add(&listener->narratives, tale[0]);
    }
    else {
        /* narrative memory is full - choose the most forgettable */
        listener_narrative_index =
            mm_narratives_least_heard(&listener->narratives);
        mm_narratives_insert(&listener->narratives,
                             listener_narrative_index, tale[0]);
    }

    /* increment the number of times heard */
    listener->narratives.tale[listener_narrative_index].times_heard++;

    /* listener's attention is on the current narrative */
    listener->attention[MM_ATTENTION_NARRATIVE] =
        (n_uint)listener_narrative_index;
    return 0;
}

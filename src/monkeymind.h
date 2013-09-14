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

/* the maximum number of properties of an object */
#define MM_MAX_OBJECT_PROPERTIES  8

/* the maximum number of steps within a narrative sequence */
#define MM_MAX_NARRATIVE_LENGTH  16

/* size of narrative memory */
#define MM_SIZE_NARRATIVES       32

/* maximum size of the social graph */
#define MM_SIZE_SOCIAL_GRAPH     32

/* te number of arguments which each language machine
   instruction takes */
#define MM_SIZE_LANGUAGE_ARGS    2

/* the number of instructions in the language machine */
#define MM_SIZE_LANGUAGE_INSTRUCTIONS 128

/* dimension of a 2D map within which the agent is located */
#define MM_SIZE_SPATIAL          32

/* emotions binary coded according to the Lövheim Cube
   Lövheim H. A new three-dimensional model for emotions and
   monoamine neurotransmitters. (2012). Med Hypotheses,
   78, 341-348. doi:10.1016/j.mehy.2011.11.016 PMID 22153577 */
#define MM_EMOTION_SHAME      0
#define MM_EMOTION_DISTRESS   1
#define MM_EMOTION_FEAR       2
#define MM_EMOTION_ANGER      3
#define MM_EMOTION_DISGUST    4
#define MM_EMOTION_SURPRISE   5
#define MM_EMOTION_JOY        6
#define MM_EMOTION_EXCITEMENT 7

enum subjective_time
{
    MM_TIME_LONG_AGO = 0,
	MM_TIME_RECENTLY,
	MM_TIME_NOW,
	MM_TIME_NEAR_FUTURE,
	MM_TIME_DISTANT_FUTURE,
    MM_SUBJECTIVE_TIMES
};

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
	MM_PROPERTIES
};

enum language_machine_functions
{
    MM_INSTRUCTION_NONE = 0,
	MM_INSTRUCTION_ADD,
	MM_INSTRUCTION_REMOVE,
	MM_INSTRUCTION_INSERT,
	MM_INSTRUCTION_COPY,
	MM_INSTRUCTIONS
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

typedef struct
{
	mm_object who;
	mm_object what;
	mm_object where;
	mm_object when;
	mm_object why;
	mm_object way;
	mm_object means;
} mm_circumstance;

typedef struct
{
    /* a unique reference for the narrative */
    unsigned int id;

    /* the number of steps in the narrative */
    unsigned int length;

    /* array storing the steps in the narrative */
    mm_circumstance step[MM_MAX_NARRATIVE_LENGTH];
} mm_narrative;

/* representation of a language machine instruction */
typedef struct
{
	unsigned char function;
	unsigned int argument[MM_SIZE_LANGUAGE_ARGS];
	/* flags for relative or absolute addressing */
	unsigned char flags;
} mm_language_instruction;

/* the language machine is just a program containing a series
   of instructions */
typedef struct
{
	mm_language_instruction instruction[MM_SIZE_LANGUAGE_INSTRUCTIONS];
} mm_language;

typedef struct
{
	unsigned int value[4];
} mm_random_seed;

typedef struct
{
	mm_random_seed seed;

	/* memory which may contain a number of narratives */
	mm_narrative narrative[MM_SIZE_NARRATIVES];

	/* details of each known agent */
    mm_object social_graph[MM_SIZE_SOCIAL_GRAPH];

	/* language machinery associated with each social graph entry */
	mm_language language[MM_SIZE_SOCIAL_GRAPH];

	/* spatial memory */
	mm_object spatial[MM_SIZE_SPATIAL*MM_SIZE_SPATIAL];
} monkeymind;

void mm_init(monkeymind * mind);
unsigned int mm_rand(mm_random_seed * seed);
unsigned char mm_neuro_to_emotion(unsigned int serotonin,
								  unsigned int dopamine,
								  unsigned int noradrenaline,
								  unsigned int neurotransmitter_max);
void mm_emotion_to_neuro(unsigned char emotion,
						 unsigned int * serotonin,
						 unsigned int * dopamine,
						 unsigned int * noradrenaline,
						 unsigned int neurotransmitter_max);

#endif

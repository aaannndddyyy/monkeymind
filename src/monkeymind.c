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

/* Xorshift */
unsigned int mm_rand(mm_random_seed * seed)
{
	unsigned int t;

	/* avoid singularity */
	if (seed->value[0]+seed->value[1]+
		seed->value[2]+seed->value[3] == 0) {
		seed->value[0] = 123456789;
		seed->value[1] = 362436069;
		seed->value[2] = 521288629;
		seed->value[3] = 88675123;
	}

	t = seed->value[0] ^ (seed->value[0] << 11);
	seed->value[0] = seed->value[1];
	seed->value[1] = seed->value[2];
	seed->value[2] = seed->value[3];
	seed->value[3] =
		seed->value[3] ^
		(seed->value[3] >> 19) ^
		(t ^ (t >> 8));
	return seed->value[3];
}

/* initialises a mind */
void mm_init(monkeymind * mind)
{
	int i, j, k;

	memset((void*)mind->narrative, '\0',
		   MM_SIZE_NARRATIVES * sizeof(mm_narrative));
	memset((void*)mind->social_graph, '\0',
		   MM_SIZE_SOCIAL_GRAPH * sizeof(mm_object));
	memset((void*)mind->spatial, '\0',
		   MM_SIZE_SPATIAL * MM_SIZE_SPATIAL * sizeof(mm_object));

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

	/* assign id numbers to spatial map */
	for (i = 0; i < MM_SIZE_SPATIAL*MM_SIZE_SPATIAL;i++) {
		mind->spatial[i].id = i;
	}
}

/* convert a set of neurotransmitter levels into a single value
   indicating the type of emotion */
unsigned char mm_neuro_to_emotion(unsigned int serotonin,
								  unsigned int dopamine,
								  unsigned int noradrenaline,
								  unsigned int neurotransmitter_max)
{
	unsigned int threshold = neurotransmitter_max>>1;
	unsigned char emotion = 0;

	if (noradrenaline >= threshold) emotion |= 1;
	if (dopamine >= threshold) emotion |= 2;
	if (serotonin >= threshold) emotion |= 4;
	return emotion;
}

/* convert an emotion into a set of neurotransmitter levels */
void mm_emotion_to_neuro(unsigned char emotion,
						 unsigned int * serotonin,
						 unsigned int * dopamine,
						 unsigned int * noradrenaline,
						 unsigned int neurotransmitter_max)
{
	*serotonin = 0;
	*dopamine = 0;
	*noradrenaline = 0;

	if (emotion & 1) *noradrenaline = neurotransmitter_max;
	if (emotion & 2) *dopamine = neurotransmitter_max;
	if (emotion & 4) *serotonin = neurotransmitter_max;
}

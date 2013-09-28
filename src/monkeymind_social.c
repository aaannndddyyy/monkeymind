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

#include "monkeymind_social.h"

/* returns the social graph array index of the individual having the
   given id */
int mm_social_index_from_id(monkeymind * mind, unsigned int met_id)
{
	int i;

	for (i = MM_SELF+1; i < MM_SIZE_SOCIAL_GRAPH; i++) {
		if (!SOCIAL_GRAPH_ENTRY_EXISTS(mind,i)) break;
		if (mind->social_graph[i].property_value[MET_ID] == met_id) {
			return i;
		}
	}
	return -1;
}

/* returns the social graph array index of the individual having the
   given name */
int mm_social_index_from_name(monkeymind * mind, unsigned int met_name)
{
	int i;

	for (i = MM_SELF+1; i < MM_SIZE_SOCIAL_GRAPH; i++) {
		if (!SOCIAL_GRAPH_ENTRY_EXISTS(mind,i)) break;
		if (mind->social_graph[i].property_value[MET_NAME] == met_name) {
			return i;
		}
	}
	return -1;
}

/* returns the social graph array index of a forgettable individual */
static int mm_social_forget(monkeymind * mind)
{
	int i, index = -1;
	unsigned int min_observations = mind->social_graph[0].observations;

	/* pick the individual with the fewest observations */
	for (i = MM_SELF+1; i < MM_SIZE_SOCIAL_GRAPH; i++) {
		if (!SOCIAL_GRAPH_ENTRY_EXISTS(mind,i)) break;
		if (mind->social_graph[i].observations < min_observations) {
			min_observations = mind->social_graph[i].observations;
			index = i;
		}
	}
	return index;
}

/* the prejudice function */
static void mm_social_evaluate(monkeymind * meeter,
							   monkeymind * met,
							   mm_object * social_graph_entry)
{
	unsigned int fof = MM_NEUTRAL;

	/* TODO: add a friend or foe property */

	mm_obj_prop_add(social_graph_entry,
					MM_PROPERTY_FRIEND_OR_FOE, fof);
}

/* update a coocurrence matrix of the properties of
   known individuals */
static void mm_update_property_matrix(monkeymind * mind,
									  int index)
{
	int i, j, max = 0, min = 0, incr;
	mm_object * individual;
	unsigned int p0, p1;

	/* for every individual in the social graph */
	if (!SOCIAL_GRAPH_ENTRY_EXISTS(mind, index)) return;
	individual = &mind->social_graph[index];

	/* friendly or unfriendly? */
	printf("test3 %d\n",mm_obj_prop_get(individual, MM_PROPERTY_FRIEND_OR_FOE));
	if (mm_obj_prop_get(individual, MM_PROPERTY_FRIEND_OR_FOE) >= MM_NEUTRAL) {
		incr = 1;
	}
	else {
		incr = -1;
	}

	/* combinations of properties for this individual */
	for (i = 4; i < individual->length; i++) {
		p0 = individual->property_type[i];
		for (j = i+1; j < individual->length; j++) {
			p1 = individual->property_type[j];
			mind->property_matrix[p0*MM_PROPERTIES + p1]+=incr;
			mind->property_matrix[p1*MM_PROPERTIES + p0]+=incr;
		}
	}

	/* find the maximum activation */
	for (i = 0; i < MM_PROPERTIES * MM_PROPERTIES; i++) {
		if (mind->property_matrix[i] > max) {
			max = mind->property_matrix[i];
		}
		if (mind->property_matrix[i] < min) {
			min = mind->property_matrix[i];
		}
	}

	/* normalise if out of range */
	if ((max > 64000) || (min < -64000)) {
		for (i = 0; i < MM_PROPERTIES * MM_PROPERTIES; i++) {
			mind->property_matrix[i] /= 2;
		}
	}
}

/* adds a social graph enry at the given index */
static void mm_social_add(monkeymind * meeter, monkeymind * met,
						  int index, unsigned char familiar)
{
	int i;
	mm_object * individual;

    individual = &meeter->social_graph[index];
	individual->property_type[MEETER_ID] = MM_PROPERTY_MEETER;
	individual->property_value[MEETER_ID] = meeter->id;
	individual->property_type[MEETER_NAME] = MM_PROPERTY_NAME;
	individual->property_value[MEETER_NAME] =
		mm_get_property(meeter, MM_PROPERTY_NAME);
	individual->property_type[MET_ID] = MM_PROPERTY_MET;
	individual->property_value[MET_ID] = met->id;
	individual->property_type[MET_NAME] = MM_PROPERTY_NAME;
	individual->property_value[MET_NAME] =
		mm_get_property(met, MM_PROPERTY_NAME);
	individual->length = 4;

	/* remember properties of the met individual */
	mm_obj_copy(met->properties, individual);

	if (familiar == 0) {
		/* first meeting */
		individual->observations = 1;
		printf("test4 %d ",individual->length);
		mm_social_evaluate(meeter, met, individual);
		printf("%d\n",individual->length);
	}
	else {
		/* subsequent meetings */
		individual->observations++;
		if (meeter->social_graph[index].observations >=
			MM_SOCIAL_MAX_OBSERVATIONS) {
			for (i = 0; i < MM_SIZE_SOCIAL_GRAPH; i++) {
				meeter->social_graph[i].observations >>= 1;
			}
		}
	}

	mm_update_property_matrix(meeter, index);
}

/* two individuals meet */
void mm_social_meet(monkeymind * meeter, monkeymind * met)
{
	unsigned char familiar = 0;
	int index = mm_social_index_from_id(meeter, met->id);
	if (index == -1) {
		/* are all array entries occupied? */
		if (SOCIAL_GRAPH_ENTRY_EXISTS(meeter,MM_SIZE_SOCIAL_GRAPH-1)) {
			index = mm_social_forget(meeter);
		}
		else {
			/* find the last entry */
			for (index = MM_SIZE_SOCIAL_GRAPH-1; index >= 0; index--) {
				if (SOCIAL_GRAPH_ENTRY_EXISTS(meeter,index)) {
					index++;
					break;
				}
			}
		}
	}
	else {
		familiar = 1;
	}
	if (index == -1) return;

	mm_social_add(meeter, met, index, familiar);
}

void mm_social_speak(monkeymind * speaker, monkeymind * listener)
{

}

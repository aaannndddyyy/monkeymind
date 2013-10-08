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
		if (mm_obj_prop_get(&mind->social_graph[i],
							MM_PROPERTY_MEETER) == met_id) {
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
		if (mm_obj_prop_get(&mind->social_graph[i],
							MM_PROPERTY_MET_NAME) == met_name) {
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
	for (i = 1; i < MM_SIZE_SOCIAL_GRAPH; i++) {
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
	unsigned int attraction = MM_NEUTRAL;

	/* TODO: calculate or look up friend or foe value */

	/* TODO: calculate or look up attraction value */

	mm_obj_prop_add(social_graph_entry,
					MM_PROPERTY_FRIEND_OR_FOE, fof);
	mm_obj_prop_add(social_graph_entry,
					MM_PROPERTY_ATTRACTION, attraction);
}

/* get the existing categorisation at this location */
static int mm_social_get_category(int * categories,
								  unsigned int social_x,
								  unsigned int social_y)
{
	unsigned int n = social_y*MM_SOCIAL_CATEGORIES_DIMENSION + social_x;
	if (categories[n] > 0) return 1;
	if (categories[n] < 0) return -1;
	return 0;
}

/* update categories using the given increment */
static void mm_social_category_update(int * categories,
									  unsigned int social_x,
									  unsigned int social_y,
									  int increment)
{
	int x, y, dx, dy, max_radius, inner_radius, r, n;
	unsigned char normalise = 0;
	const int max_response = 256;

	if (increment == 0) return;

	max_radius =
		MM_SOCIAL_CATEGORIES_RADIUS*MM_SOCIAL_CATEGORIES_RADIUS;
	inner_radius =
		MM_SOCIAL_CATEGORIES_RADIUS*MM_SOCIAL_CATEGORIES_RADIUS/4;

	for (x = (int)social_x - MM_SOCIAL_CATEGORIES_RADIUS;
		 x <= (int)social_x + MM_SOCIAL_CATEGORIES_RADIUS;
		 x++) {
		if ((x < 0) || (x >= MM_SOCIAL_CATEGORIES_DIMENSION)) {
			continue;
		}
		dx = x - (int)social_x;
		for (y = (int)social_y - MM_SOCIAL_CATEGORIES_RADIUS;
			 y <= (int)social_y + MM_SOCIAL_CATEGORIES_RADIUS;
			 y++) {
			if ((y < 0) || (y >= MM_SOCIAL_CATEGORIES_DIMENSION)) {
				continue;
			}
			dy = y - (int)social_y;
			r = dx*dx + dy*dy;
			if (r > max_radius) continue;

			/* location within the map */
			n = y*MM_SOCIAL_CATEGORIES_DIMENSION + x;

			if (r < inner_radius) {
				categories[n] += increment*2;
			}
			else {
				categories[n] += increment;
			}
			/* range checking */
			if ((categories[n] > max_response) ||
				(categories[n] < -max_response)) {
				normalise = 1;
			}
		}
	}

	/* normalise if necessary to keep values in range */
	if (normalise == 1) {
		for (n = 0;
			 n < MM_SOCIAL_CATEGORIES_DIMENSION*
				 MM_SOCIAL_CATEGORIES_DIMENSION; n++) {
			categories[n] /= 2;
		}
	}
}

/* communicates the social categorisation of a given social
   graph entry to another individual */
void mm_communicate_social_categorisation(monkeymind * mind,
										  int index,
										  monkeymind * other)
{
	mm_object * individual;
	unsigned int social_x, social_y;
	unsigned char normalised_properties[MM_PROPERTIES];

	if (!SOCIAL_GRAPH_ENTRY_EXISTS(mind, index)) return;
	individual = &mind->social_graph[index];

    social_x = mm_obj_prop_get(individual, MM_PROPERTY_SOCIAL_X);
    social_y = mm_obj_prop_get(individual, MM_PROPERTY_SOCIAL_Y);

	/* normalise property values into a single byte range */
	mm_obj_to_vect(individual, normalised_properties);

	/* adjust weights within the social categorisation SOM
	   of the other individual */
	mm_som_learn(&other->social_categories,
				 normalised_properties,
				 social_x, social_y);
}

/* categorise an entry within the social graph */
static void mm_social_categorisation(monkeymind * mind,
									 int index)
{
	mm_object * individual;
	int fof_increment = 0, attraction_increment = 0;
	unsigned char normalised_properties[MM_PROPERTIES];
	unsigned int fof, attraction, social_x=0, social_y=0;

	if (!SOCIAL_GRAPH_ENTRY_EXISTS(mind, index)) return;
	individual = &mind->social_graph[index];

	/* normalise property values into a single byte range */
	mm_obj_to_vect(individual, normalised_properties);

	/* friendly or unfriendly? */
	fof = mm_obj_prop_get(individual, MM_PROPERTY_FRIEND_OR_FOE);
	if (fof > MM_NEUTRAL) {
		fof_increment = 1;
	}
	if (fof < MM_NEUTRAL) {
		fof_increment = -1;
	}

	/* attractive or unattractive? */
	attraction = mm_obj_prop_get(individual, MM_PROPERTY_ATTRACTION);
	if (attraction > MM_NEUTRAL) {
		attraction_increment = 1;
	}
	if (attraction < MM_NEUTRAL) {
		attraction_increment = -1;
	}

	/* find the peak response within the SOM,
	   corresponding to the minimum Euclidean distance */
	mm_som_update(&mind->social_categories,
				  normalised_properties,
				  &social_x, &social_y);

	/* store the categorisation as properties */
	mm_obj_prop_add(individual, MM_PROPERTY_SOCIAL_X, social_x);
	mm_obj_prop_add(individual, MM_PROPERTY_SOCIAL_Y, social_y);

	/* adjust weights within the social categorisation SOM */
	mm_som_learn(&mind->social_categories,
				 normalised_properties,
				 social_x, social_y);

	/* alter the friend of foe status depending upon the existing
	   classification */
	mm_obj_prop_set(individual, MM_PROPERTY_FRIEND_OR_FOE,
					mm_social_get_category(mind->category[MM_CATEGORY_FOF].value,
														  social_x, social_y));

	/* alter the friend or foe values within the classifier */
	mm_social_category_update(mind->category[MM_CATEGORY_FOF].value,
							  social_x, social_y, fof_increment);

	/* alter the attraction status depending upon the existing
	   classification */
	mm_obj_prop_set(individual, MM_PROPERTY_ATTRACTION,
					mm_social_get_category(mind->category[MM_CATEGORY_ATTRACTION].value,
														  social_x, social_y));

	/* alter the attraction values within the classifier */
	mm_social_category_update(mind->category[MM_CATEGORY_ATTRACTION].value,
							  social_x, social_y, attraction_increment);
}

/* adds a social graph enry at the given index */
static void mm_social_add(monkeymind * meeter, monkeymind * met,
						  int index, unsigned char familiar)
{
	int i;
	mm_object * individual;

    individual = &meeter->social_graph[index];

	/* remember properties of the met individual */
	mm_obj_copy(met->properties, individual);

	mm_obj_prop_add(individual,
					MM_PROPERTY_MEETER, meeter->id);

	mm_obj_prop_add(individual,
					MM_PROPERTY_MEETER_NAME,
					mm_get_property(meeter, MM_PROPERTY_NAME));

	mm_obj_prop_add(individual,
					MM_PROPERTY_MET, met->id);

	mm_obj_prop_add(individual,
					MM_PROPERTY_MET_NAME,
					mm_get_property(met, MM_PROPERTY_NAME));

	if (familiar == 0) {
		/* first meeting */
		individual->observations = 1;
		mm_social_evaluate(meeter, met, individual);
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

	/* make generalisations about the met individual */
	mm_social_categorisation(meeter, index);

	/* set attention to the met being */
	meeter->attention[MM_ATTENTION_SOCIAL_GRAPH] = index;
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

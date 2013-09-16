#include "monkeymind_social.h"

/* returns the social graph array index of the individual having the
   given id */
int mm_social_index_from_id(monkeymind * mind, unsigned int met_id)
{
	int i;

	for (i = 1; i < MM_SIZE_SOCIAL_GRAPH; i++) {
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

	for (i = 1; i < MM_SIZE_SOCIAL_GRAPH; i++) {
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
static void mm_social_evaluate(monkeymind * meeter, monkeymind * met,
							   mm_object * social_graph_entry)
{
	/* TODO */
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

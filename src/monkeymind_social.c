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

void mm_social_meet(monkeymind * meeter, monkeymind * met)
{
}

void mm_social_speak(monkeymind * speaker, monkeymind * listener)
{

}

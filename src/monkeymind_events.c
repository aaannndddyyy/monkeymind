/****************************************************************

 Monkeymind - an experimental cogitive architecture

 The event memory is similar to a hippocampus, recording
 the stream of perceived circumstances, then filtering
 out (or compressing) the relevant from the forgettable

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

#include "monkeymind_events.h"

void mm_events_init(mm_events * events)
{
	events->index = 0;

	memset((void*)events->sequence, '\0',
		   sizeof(mm_object)*MM_EVENT_MEMORY_SIZE);
}

/* add an event to the sequence */
void mm_events_add(mm_events * events,
				   mm_object * observation)
{
	if (!mm_obj_exists(observation)) return;

	/* store the observation */
	mm_obj_copy(observation, &events->sequence[events->index]);

	/* increment the location in the sequence */
	events->index++;
	if (events->index >= MM_EVENT_MEMORY_SIZE) {
		events->index -= MM_EVENT_MEMORY_SIZE;
	}
}

/* returns the number of events in the sequence */
n_uint mm_events_max(mm_events * events)
{
	if (events->index == 0) return 0;

	if (mm_obj_exists(&events->sequence[MM_EVENT_MEMORY_SIZE-1])) {
		return MM_EVENT_MEMORY_SIZE;
	}

	return events->index;
}

/* returns the event at the given time step.
   Returns zero if the time step is greater than the maximum */
mm_object * mm_events_get(mm_events * events, n_uint timestep)
{
	n_uint max = mm_events_max(events);
    n_int index;

	if ((max == 0) || (timestep >= max)) return 0;
	index = (n_int)events->index - (n_int)max + (n_int)timestep;
	if (index < 0) index += MM_EVENT_MEMORY_SIZE;
	if (!mm_obj_exists(&events->sequence[index])) {
		return 0;
	}
	return &events->sequence[index];
}

/* returns a list of protagonists which appear between certain time steps in
   the event sequence */
n_int mm_events_protagonists(mm_events * events,
							 n_uint timestep_start,
							 n_uint timestep_end,
							 mm_protagonist * protagonists,
							 n_uint max_protagonists)
{
	n_uint c, i, j, t, agent_id, no_of_protagonists = 0;
	mm_object * event;

	/* properties which contain agent IDs */
	const n_uint agent_properties[] = {
		MM_PROPERTY_MEETER,
		MM_PROPERTY_MET
	};
	n_uint no_of_agent_properties = 2;

	/* validate the start and end time steps */
	if ((timestep_end < timestep_start) ||
		(timestep_start > MM_EVENT_MEMORY_SIZE) ||
		(timestep_end > MM_EVENT_MEMORY_SIZE)) {
		return -1;
	}

	/* end time step must be less than the maximum */
	if (timestep_end > mm_events_max(events)) {
		return -1;
	}

	/* clear the list of protagonists */
	memset((void*)protagonists, '\0',
		   max_protagonists*sizeof(mm_protagonist));

	/* for every time step */
	for (t = timestep_start; t < timestep_end; t++) {
		/* get the event at this time step */
		event = mm_events_get(events, t);
		/* search for agent IDs within the event */
		for (i = 0; i < no_of_agent_properties; i++) {
			agent_id = mm_obj_prop_get(event, agent_properties[i]);
			if (agent_id == 0) continue;

			/* does this agent already exist in the protagonists list? */
			for (j = 0; j < no_of_protagonists; j++) {
				if (agent_id == protagonists[j].agent_id) {
					/* already in the list.  Increment the number of hits */
					protagonists[j].hits++;
					/* update FOF and attraction values for this agent */
					for (c = 0; c < MM_CATEGORIES; c++) {
					    protagonists[j].category[c] +=
							mm_obj_prop_get(event,
											MM_PROPERTY_FRIEND_OR_FOE+c);
					}
					break;
				}
			}
			if (j == no_of_protagonists) {
				/* agent does not already exist within the protagonists list */
				protagonists[no_of_protagonists].agent_id = agent_id;
				protagonists[no_of_protagonists].hits = 1;
				/* update FOF and attraction values for this agent */
				for (c = 0; c < MM_CATEGORIES; c++) {
					protagonists[no_of_protagonists].category[c] =
						mm_obj_prop_get(event, MM_PROPERTY_FRIEND_OR_FOE+c);
				}
				/* increment the number of protagonists */
				no_of_protagonists++;
				if (no_of_protagonists >= max_protagonists) {
					return no_of_protagonists;
				}
			}

		}
	}

	return no_of_protagonists;
}

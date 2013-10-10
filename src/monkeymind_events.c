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
unsigned int mm_events_max(mm_events * events)
{
	if (events->index == 0) return 0;

	if (mm_obj_exists(&events->sequence[MM_EVENT_MEMORY_SIZE-1])) {
		return MM_EVENT_MEMORY_SIZE;
	}

	return events->index;
}

/* returns the event at the given time step */
mm_object * mm_events_get(mm_events * events, unsigned int timestep)
{
	unsigned int max = mm_events_max(events);
    int index;

	if (max == 0) return 0;
	if (timestep >= max) timestep = max-1;
	index = (int)events->index - (int)max + (int)timestep;
	if (index < 0) index += MM_EVENT_MEMORY_SIZE;
	return &events->sequence[index];
}

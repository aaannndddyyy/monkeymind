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

#ifndef MONKEYMIND_SOCIAL_H
#define MONKEYMIND_SOCIAL_H

#include <stdio.h>
#include <string.h>
#include "monkeymind.h"

/* property indexes of meeter and met beings */
#define MEETER_ID   0
#define MEETER_NAME 1
#define MET_ID      2
#define MET_NAME    3

#define SOCIAL_GRAPH_ENTRY_EXISTS(mind,index) \
	(!((mind->social_graph[index].property_value[MEETER_ID] == 0) &&	\
	   (mind->social_graph[index].property_value[MET_ID] == 0)))

int mm_social_index_from_id(monkeymind * mind, unsigned int met_id);
int mm_social_index_from_name(monkeymind * mind, unsigned int met_name);
void mm_social_meet(monkeymind * meeter, monkeymind * met);
void mm_social_speak(monkeymind * speaker, monkeymind * listener);

#endif

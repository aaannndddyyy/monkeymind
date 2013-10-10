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

#include "monkeymind_narrative.h"

void mm_circumstance_copy(mm_circumstance * src,
						  mm_circumstance * dest)
{
	mm_obj_copy(&src->who, &dest->who);
	mm_obj_copy(&src->what, &dest->what);
	mm_obj_copy(&src->where, &dest->where);
	mm_obj_copy(&src->when, &dest->when);
	mm_obj_copy(&src->why, &dest->why);
	mm_obj_copy(&src->way, &dest->way);
	mm_obj_copy(&src->means, &dest->means);
}

int mm_circumstance_exists(mm_circumstance * c)
{
	return (mm_obj_exists(&c->who) ||
			mm_obj_exists(&c->what) ||
			mm_obj_exists(&c->where) ||
			mm_obj_exists(&c->when) ||
			mm_obj_exists(&c->why) ||
			mm_obj_exists(&c->way) ||
			mm_obj_exists(&c->means));
}

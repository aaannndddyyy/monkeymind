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

#include "monkeymind_unittests.h"

static void test_init()
{
	monkeymind mind;

	printf("test_init...");

	mm_init(&mind);

	printf("Ok\n");
}

static void test_object_add_remove_properties()
{
	mm_object obj;
	unsigned int i;
	unsigned int props[] = {
		3,6268,
		7,3568,
		1,56437,
		99,6732,
		40,1357,
		42,6267,
		23,536774,
		50,2435,
		49,23566,
		12,2356
	};

	printf("test_object_add_remove_properties...");

	/* clear */
	obj.length = 0;
	for (i = 0; i < 10; i++) {
		obj.property_type[i] = 0;
		obj.property_value[i] = 0;
	}

	/* add the properties */
	for (i = 0; i < 10; i++) {
		mm_obj_prop_add(&obj,props[i*2],props[i*2+1]);
	}

	/* check that 10 properties were added */
	assert(obj.length == 10);

	/* check that values are non-zero */
	for (i = 0; i < 10; i++) {
		assert(obj.property_type[i] > 0);
		assert(obj.property_value[i] > 0);
	}

	/* check that the property types are in order */
	for (i = 1; i < 10; i++) {
		assert(obj.property_type[i-1] <= obj.property_type[i]);
	}

	/* property 23 exists in the list */
	assert(mm_obj_prop_index(&obj,23) > -1);

	/* remove it */
	mm_obj_prop_remove(&obj,23);

	/* check that the list size is reduced */
	assert(obj.length = 9);

	/* check that property 23 no longer exists */
	assert(mm_obj_prop_index(&obj,23) == -1);

	printf("Ok\n");
}

void mm_run_tests()
{
	test_init();
	test_object_add_remove_properties();

	printf("All tests passed\n");
}

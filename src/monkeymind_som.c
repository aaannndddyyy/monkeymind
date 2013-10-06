#include "monkeymind_som.h"

void mm_som_init(mm_som * s,
				 unsigned int dimension,
				 unsigned int vector_length,
				 mm_random_seed * seed)
{
	s->dimension = dimension;
	s->vector_length = vector_length;
	s->seed = seed;

	memset((void*)s->activation, '\0',
		   MM_SOM_MAX_DIMENSION*MM_SOM_MAX_DIMENSION*
		   sizeof(int));
	memset((void*)s->disposition, '\0',
		   MM_SOM_MAX_DIMENSION*MM_SOM_MAX_DIMENSION*
		   sizeof(int));
}

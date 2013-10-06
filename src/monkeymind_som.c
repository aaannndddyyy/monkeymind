#include "monkeymind_som.h"

void mm_som_init(mm_som * s,
				 unsigned int dimension,
				 unsigned int vector_length,
				 unsigned int learning_radius,
				 mm_random_seed * seed)
{
	s->dimension = dimension;
	s->vector_length = vector_length;
	s->learning_radius = learning_radius;
	s->seed = seed;

	memset((void*)s->activation, '\0',
		   MM_SOM_MAX_DIMENSION*MM_SOM_MAX_DIMENSION*
		   sizeof(int));

	/* TODO random weights */
}

void mm_som_update(mm_som * s,
				   unsigned char * vector,
				   unsigned int * x, unsigned int * y)
{
	unsigned int xx, yy, i, ctr = 0, n = 0;
	int diff, min_diff = 999999;

	*x = 0;
	*y = 0;

	for (yy = 0; yy < s->dimension; yy++) {
		for (xx = 0; xx < s->dimension; xx++, ctr++) {
			/* activation is the Euclidean distance between
			   the weights and the input vector */
			s->activation[ctr] = 0;
			for (i = 0; i < s->vector_length; i++, n++) {
				diff = (int)vector[i] - (int)s->weight[n];
				if (diff < 0) diff = -diff;
				s->activation[ctr] += diff;
			}
			/* record the highest activation */
			if (s->activation[ctr] < min_diff) {
				*x = xx;
				*y = yy;
				min_diff = s->activation[ctr];
			}
		}
	}
}

/* adjust the weights depending upon the location of the peak response */
void mm_som_learn(mm_som * s,
				  unsigned char * vector,
				  unsigned int x, unsigned int y)
{
	int xx, yy, dx, dy, max_radius, inner_radius, r, n, i;
	int increment, v;

	max_radius =
		s->learning_radius*s->learning_radius;
	inner_radius =
		s->learning_radius*s->learning_radius/4;

	for (xx = (int)x - s->learning_radius;
		 xx <= (int)x + s->learning_radius;
		 xx++) {
		if ((xx < 0) || (xx >= s->dimension)) {
			continue;
		}
		dx = xx - (int)x;
		for (yy = (int)y - s->learning_radius;
			 yy <= (int)y + s->learning_radius;
			 yy++) {
			if ((yy < 0) || (yy >= s->dimension)) {
				continue;
			}
			dy = yy - (int)y;
			r = dx*dx + dy*dy;
			if (r > max_radius) continue;

			/* location within the map */
			n = (yy*s->dimension + xx) * s->vector_length;

			if (r < inner_radius) {
				increment = 2;
			}
			else {
				increment = 1;
			}

			/* adjust the weights */
			for (i = 0; i < s->vector_length; i++,n++) {
				if (s->weight[n] > vector[i]) {
					v = (int)s->weight[n] - increment;
				}
				else if (s->weight[n] < vector[i]) {
					v = (int)s->weight[n] + increment;
				}
				else {
					continue;
				}
				if (v < 0) v = 0;
				if (v > 255) v = 255;
				s->weight[n] = v;
			}
		}
	}
}

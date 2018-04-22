#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

int main()
{
	memory mem = create_memory(2);

	for (int i = 0; i < 100; i++) {
		segment seg = segment_new(10);
		add_segment(seg, mem);
		fprintf(stderr, "%d: %u\n", i, get_word(get_segment(i, mem), 0));
	}

	for (uint32_t i = 0; i < 100; i += 10) {
		segment seg = segment_new(10);
		add_word(seg, 5, 0);
		rem_segment(i, mem);
		put_segment(seg, mem, i);
	}

	fprintf(stderr, "\n\n\n\n\n\n\n\n");

	for (uint32_t i = 0; i < 100; i++) {
		fprintf(stderr, "%u: %u\n", i, get_word(get_segment(i, mem), 0));
	}
	free_memory(mem);
}

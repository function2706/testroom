/**
 * @file pyramid.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "pyramid.hh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000

#define MIN(a, b) (a < b) ? a : b

static int correct_stones(int len_edge, int row, int column)
{
	int rad = (len_edge + 1) / 2 - 1;
	int dx = rad - abs(rad - column), dy = rad - abs(rad - row);

	return 1 + ((MIN(dx, dy) > 0) ? MIN(dx, dy) : 0);
}

int pyramid::answer(void)
{
	int len_edge;
	int ret = 0;
	char str[MAX_LEN];
	char *term, *head, *fret;

	fret = fgets(str, sizeof(str), stdin);
	if (!fret) {
		return -1;
	}
	len_edge = atoi(str);

	for (int row = 0; row < len_edge; row++) {
		head = str;
		memset(str, 0, sizeof(str));
		fret = fgets(str, sizeof(str), stdin);
		if (!fret) {
			return -1;
		}

		for (int column = 0; column < len_edge; column++) {
			term = strchr(head, ' ');
			if (term != NULL) {
				*term = '\0';
			}
			ret += atoi(head) - correct_stones(len_edge, row, column);

			if (term != NULL) {
				head = term + 1;
			}
		}
	}
	printf("%d\n", ret);

	return 0;
}
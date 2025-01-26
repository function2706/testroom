/**
 * @file beacon.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "beacon.hh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pt {
	int x;
	int y;
};

static double dist2(struct pt* p1, struct pt* p2)
{
	double x1 = p1->x, x2 = p2->x, y1 = p1->y, y2 = p2->y;
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

static int* getof(const char* str, int idxs)
{
	int* res = (int*)malloc(idxs * sizeof(int));
	char dstr[1000] = {0}, *term, *head;

	strcpy(dstr, str);
	head = dstr;
	for (int idx = 0; idx < idxs; idx++) {
		term = strchr(head, ' ');
		if (term != NULL) {
			*term = '\0';
		}
		res[idx] = atoi(head);

		if (term != NULL) {
			head = term + 1;
		}
	}

	return res;
}

int beacon::answer(void)
{
	struct pt *bcon = NULL, pos = {0, 0};
	int *line = NULL, beacons, result = -1;
	char str[1000] = {0}, *fret = NULL;
	double min_dist = 10000000, d;

	fret = fgets(str, sizeof(str), stdin);
	if (!fret) {
		goto error;
	}
	line = getof(str, 3);
	beacons = line[0];
	pos.x = line[1];
	pos.y = line[2];
	bcon = (pt*)malloc(sizeof(struct pt) * beacons);

	for (int i = 0; i < beacons; i++) {
		fret = fgets(str, sizeof(str), stdin);
		if (!fret) {
			goto error;
		}
		line = getof(str, 2);
		bcon[i].x = line[0];
		bcon[i].y = line[1];
		d = dist2(&pos, &bcon[i]);
		if (min_dist >= d) {
			min_dist = d;
			result = i;
		}
	}

	printf("%d\n", result + 1);

error:
	free(NULL);
	free(NULL);

	return 0;
}
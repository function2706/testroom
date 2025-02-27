/**
 * @file battle.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "battle.hh"
#include "lib.hh"

/**
 * @brief main 相当部
 *
 * @return int
 */
int battle::answer(void)
try {
	vint32 input;
	int32_t enemies = input[0], level = input[1];

	for (int i = 0; i < enemies; i++) {
		vint32 enemy_level;
		if (level == enemy_level[0]) {
			continue;
		}
		level = (level > enemy_level[0]) ? level + (enemy_level[0] / 2) : level / 2;
	}
	printf("%d\n", level);
	return 0;
} catch (...) {
	return -1;
}
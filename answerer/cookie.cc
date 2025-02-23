/**
 * @file cookie.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "cookie.hh"
#include "lib.hh"

static void set_charbit(uint64_t& bits, char c)
{
	uint64_t stand = 0;
	if ((c >= '0') && (c <= '9')) {
		stand = (uint64_t)1 << (c - 48);
	}
	else if ((c >= 'A') && (c <= 'Z')) {
		stand = (uint64_t)1 << (c - 55);
	}
	else if ((c >= 'a') && (c <= 'z')) {
		stand = (uint64_t)1 << (c - 61);
	}
	bits = stand ? bits | stand : bits;
}

static uint8_t get_bitdensity(uint64_t bits)
{
	uint8_t num = 0;

	for (; bits != 0; bits &= bits - 1) {
		num++;
	}
	return num;
}

/**
 * @brief main 相当部
 *
 * @return int
 */
int cookie::answer(void)
try {
	vint32 len;
	fstring fstr;
	uint64_t bits = 0;
	for (int i = 0; i < len[0]; i++) {
		set_charbit(bits, fstr[i]);
	}
	printf("%d\n", get_bitdensity(bits));
	return 0;
} catch (...) {
	return -1;
}
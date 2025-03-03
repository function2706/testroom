/**
 * @file symmetry.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "symmetry.hh"
#include "lib.hh"

namespace nsp_symmetry
{
static bool is_symmetric_string(const vint32& row)
{
	for (size_t i = 0; i < row.size() / 2; i++) {
		if (row[i] != row[row.size() - i - 1]) {
			return false;
		}
	}
	return true;
}
}  // namespace nsp_symmetry

/**
 * @brief main 相当部
 *
 * @return int
 */
int symmetry::answer(void)
try {
	using namespace nsp_symmetry;
	vint32 size;
	bool is_symmetry = true;
	for (int i = 0; i < size[0]; i++) {
		vint32 row;
		is_symmetry &= is_symmetric_string(row);
	}
	std::string ans = is_symmetry ? "Yes" : "No";
	printf("%s\n", ans.c_str());
	return 0;
} catch (...) {
	return -1;
}
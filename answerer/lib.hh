/**
 * @file lib.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#ifndef ANSWERER_LIB_HH_
#define ANSWERER_LIB_HH_

#include <unistd.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <initializer_list>
#include <string>
#include <vector>

#define debuglog(fmt, arg...)                                            \
	do {                                                             \
		printf("%s(L.%d) " fmt "\n", __FILE__, __LINE__, ##arg); \
	} while (0)

#define xxxxxMARKERxxxxx debuglog("through")

/**
 * @brief 標準入力の文字列を扱うクラス
 */
class fstring : public std::string
{
public:
	fstring() { fget(); }

	void fget();
};

/**
 * @brief 以下のフォーマットに則した文字列から生成される整数配列クラス
 * "[整数] [整数] ... [整数]"
 */
class vint32 : public std::vector<int32_t>
{
public:
	vint32() { fget(); }
	vint32(const char* str) { get(str); }

	void add(const char* str);
	void get(const char* str);
	void fadd();
	void fget();
};

/**
 * @brief 初めに入力をまとめて指定し,
 * get() の度に一行ずつ std::string を引き出すクラス
 * デバッグ用, 一々入力するのを避けるために使用
 */
class vinput
{
	std::deque<std::string> strs_;

public:
	vinput(std::initializer_list<std::string> rows) : strs_(rows) {}

	const std::string& get();
};
#endif /* ANSWERER_LIB_HH_ */

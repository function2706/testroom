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

#include <sys/time.h>
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
	vint32(const std::string& str) { get(str.c_str()); }

	void add(const char* str);
	void get(const char* str);
	void fadd();
	void fget();
};

/**
 * @brief ランダム値生成クラス
 */
class randval
{
public:
	randval()
	{
		timeval t = {};
		gettimeofday(&t, nullptr);
		srandom((unsigned int)t.tv_usec);
	}

	long get() { return random(); }
	long get(long max) { return max ? get() % (max + 1) : 0; }
	long get(long min, long max) { return (max >= min) ? get(max - min) + min : 0; }
	bool pass(long scale) { return scale ? !(get() % scale) : false; }
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
	vinput(uint32_t num, uint32_t cols, char delim,
	       std::initializer_list<std::string> strs,
	       std::initializer_list<std::string> fixstrs, std::string topstr);

	const std::string get();
	void print() const;
};

#endif /* ANSWERER_LIB_HH_ */

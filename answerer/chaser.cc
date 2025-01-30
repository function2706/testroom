/**
 * @file chaser.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "chaser.hh"

/* =================================== 擬ライブラリ =================================== */

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

/**
 * @brief 標準入力の文字列を扱うクラス
 */
class fstring : public std::string
{
	const size_t MAX_STRLEN = 4096;

public:
	fstring() { fget(); }

	void fget();
};

/**
 * @brief 標準入力の文字列を得る
 * それまでに記憶している文字列はクリアされる
 */
void fstring::fget()
{
	char str[4096] = {0};
	char* cret = fgets(str, sizeof(str), stdin);
	if (!cret) {
		throw -1;
	}
	this->clear();
	this->append(str);
}

/**
 * @brief 以下のフォーマットに則した文字列から生成される整数配列クラス
 * "[整数] [整数] ... [整数]"
 */
class intvector : public std::vector<int>
{
	const size_t MAX_STRLEN = 4096;

public:
	intvector() { fget(); }
	intvector(const char* str) { get(str); }

	void add(const char* str);
	void get(const char* str);
	void fadd();
	void fget();
};

/**
 * @brief 以下のフォーマットに則した文字列から各カラムの整数を得る
 * "[整数] [整数] ... [整数]"
 * 整数は末尾に追加される
 *
 * @param str フォーマットに即した文字列
 */
void intvector::add(const char* str)
{
	char dstr[MAX_STRLEN] = {0}, *term, *head;
	size_t restlen = strlen(str);

	strcpy(dstr, str);
	head = dstr;
	while (restlen > 0) {
		term = strchr(head, ' ');
		if (term != NULL) {
			*term = '\0';
			restlen -= term - head + 1;
		}
		else {
			term = strchr(head, '\0');
			restlen -= term - head;
		}
		this->push_back(atoi(head));

		if (restlen > 0) {
			head = term + 1;
		}
	}
}

/**
 * @brief 以下のフォーマットに則した文字列から各カラムの整数を得る
 * "[整数] [整数] ... [整数]"
 * それまでに記憶している整数はクリアされる
 *
 * @param str フォーマットに即した文字列
 */
void intvector::get(const char* str)
{
	this->clear();
	add(str);
}

/**
 * @brief 以下のフォーマットに則した標準入力上の文字列から各カラムの整数を得る
 * "[整数] [整数] ... [整数]"
 * 整数は末尾に追加される
 *
 * @exception fgets 失敗時に -1
 */
void intvector::fadd()
{
	char str[MAX_STRLEN] = {0};

	char* cret = fgets(str, sizeof(str), stdin);
	if (!cret) {
		throw -1;
	}
	add(str);
}

/**
 * @brief 以下のフォーマットに則した標準入力上の文字列から各カラムの整数を得る
 * "[整数] [整数] ... [整数]"
 * それまでに記憶している整数はクリアされる
 *
 * @exception fgets 失敗時に -1
 */
void intvector::fget()
{
	this->clear();
	fadd();
}

/* =================================== 擬ライブラリ終 =================================== */

enum class objid : char
{
	me = 'A',
	target = 'B',
	path = '.',
	visited = '*',
	wall = '#',
};

class map
{
	int w_;
	int h_;
	char* m_;

	int me_x_;
	int me_y_;

public:
	map();
	map(const map& m);
	~map() { delete[] m_; }

	void init_map();

	int width() const { return w_; }
	int height() const { return h_; }
	int me_x() const { return me_x_; }
	int me_y() const { return me_y_; }

	char get(int x, int y) const;
	void set(char id, int x, int y);
	void set(objid id, int x, int y) { set((char)id, x, y); }

	bool is_match(objid id, int x, int y) const { return get(x, y) == (char)id; }

	int moveme_to(int dx, int dy);

	void print() const;
};

map::map(const map& m) : w_(m.width()), h_(m.height()), me_x_(m.me_x()), me_y_(m.me_y())
{
	m_ = new char[w_ * h_];
	memcpy(m_, m.m_, w_ * h_ * sizeof(char));
}

map::map() : w_(0), h_(0), m_(nullptr), me_x_(0), me_y_(0)
{
	intvector iv;

	h_ = iv[0];
	w_ = iv[1];

	m_ = new char[w_ * h_];

	for (int y = 0; y < h_; y++) {
		fstring fstr;

		for (int x = 0; x < w_; x++) {
			set(fstr[x], x, y);
			if (fstr[x] == 'A') {
				me_x_ = x;
				me_y_ = y;
			}
		}
	}
}

char map::get(int x, int y) const
{
	return m_[y * w_ + x];
}

void map::set(char id, int x, int y)
{
	m_[y * w_ + x] = id;
}

void map::print() const
{
	for (int y = 0; y < h_; y++) {
		for (int x = 0; x < w_; x++) {
			printf("%c", get(x, y));
		}
		printf("\n");
	}
}

/**
 * @brief
 *
 * @param dx
 * @param dy
 * @return int 移動回数, 移動不可時に -1
 */
int map::moveme_to(int dx, int dy)
{
	int result_x = me_x_ + dx, result_y = me_y_ + dy;

	if ((result_x < 0) || (result_x > w_ - 1) || (result_y < 0) || (result_y > h_ - 1)) {
		return -1;
	}
	else if (is_match(objid::visited, result_x, result_y) ||
		 is_match(objid::wall, result_x, result_y)) {
		return -1;
	}

	set(objid::visited, me_x_, me_y_);
	set(objid::me, result_x, result_y);
	me_x_ = result_x;
	me_y_ = result_y;
	return dx + dy;
}

class chaser_meat
{
	map origin_map;
	map marked_map;

	int min_move(int goal_x, int goal_y);

public:
	chaser_meat() : origin_map(), marked_map(origin_map) {}
	~chaser_meat() {}
	void answer();
};

int chaser_meat::min_move(int goal_x, int goal_y)
{
	return 0;
}

void chaser_meat::answer()
{
	marked_map.print();
	marked_map.moveme_to(1, 0);
	marked_map.print();
}

/**
 * @brief main 相当部
 *
 * @return int
 */
int chaser::answer(void)
try {
	chaser_meat cm;
	cm.answer();
	return 0;
} catch (...) {
	return -1;
}
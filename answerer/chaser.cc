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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
 * @brief 以下のフォーマットに則した文字列から各カラムの整数を得る
 * "[整数] [整数] ... [整数]"
 * 整数は末尾に追加される
 *
 * @param str フォーマットに即した文字列
 */
void vint32::add(const char* str)
{
	char dstr[4096] = {0}, *term, *head;
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
void vint32::get(const char* str)
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
void vint32::fadd()
{
	char str[4096] = {0};

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
void vint32::fget()
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

using dirbit = uint8_t;
namespace dir
{
	static uint8_t none = 0x0;
	static uint8_t east = 0x1;
	static uint8_t nowth = 0x2;
	static uint8_t west = 0x4;
	static uint8_t south = 0x8;
}  // namespace dir

class map
{
	uint32_t w_;
	uint32_t h_;
	objid* m_;

	uint32_t me_x_;
	uint32_t me_y_;

public:
	map();
	map(const map& m);
	~map() { delete[] m_; }

	void operator=(const map& m);

	uint32_t width() const { return w_; }
	uint32_t height() const { return h_; }
	uint32_t me_x() const { return me_x_; }
	uint32_t me_y() const { return me_y_; }

	objid get(uint32_t x, uint32_t y) const;
	void set(objid id, uint32_t x, uint32_t y);

	bool is_match(objid id, uint32_t x, uint32_t y) const { return get(x, y) == id; }

	dirbit get_accesible_dir() const;
	bool is_accesible_to(dirbit dir) const;
	int32_t moveme_with(dirbit dir);

	void print() const;
};

map::map() : w_(0), h_(0), m_(nullptr), me_x_(0), me_y_(0)
{
	vint32 iv;

	h_ = iv[0];
	w_ = iv[1];

	m_ = new objid[w_ * h_];

	for (uint32_t y = 0; y < h_; y++) {
		fstring fstr;

		for (uint32_t x = 0; x < w_; x++) {
			set((objid)fstr[x], x, y);
			if (fstr[x] == 'A') {
				me_x_ = x;
				me_y_ = y;
			}
		}
	}
}

map::map(const map& m)
{
	*this = m;
}

void map::operator=(const map& m)
{
	w_ = m.width();
	h_ = m.height();
	me_x_ = m.me_x();
	me_y_ = m.me_y();
	m_ = new objid[w_ * h_];
	memcpy(m_, m.m_, w_ * h_ * sizeof(objid));
}

/**
 * @brief 指定の座標の値(objid)を取得
 *
 * @param x
 * @param y
 * @return objid 成功時に objid
 *
 * @exception 範囲不適時に例外 -1
 */
objid map::get(uint32_t x, uint32_t y) const
{
	if ((x > w_ - 1) || (y > h_ - 1)) {
		throw -1;
	}
	return m_[y * w_ + x];
}

/**
 * @brief 指定の座標に指定の値(objid)を代入
 *
 * @param id
 * @param x
 * @param y
 *
 * @exception 範囲不適時に例外 -1
 */
void map::set(objid id, uint32_t x, uint32_t y)
{
	if ((x > w_ - 1) || (y > h_ - 1)) {
		throw -1;
	}
	m_[y * w_ + x] = id;
}

void map::print() const
{
	for (uint32_t y = 0; y < h_; y++) {
		for (uint32_t x = 0; x < w_; x++) {
			printf("%c", (char)get(x, y));
		}
		printf("\n");
	}
}

/**
 * @brief me の現在地点から移動可能な方角を取得する
 */
dirbit map::get_accesible_dir() const
{
	dirbit resdir = dir::none;

	try {
		if (is_match(objid::path, me_x_ + 1, me_y_) ||
		    is_match(objid::target, me_x_ + 1, me_y_)) {
			resdir |= dir::east;
		}
	} catch (...) {
	}

	try {
		if (is_match(objid::path, me_x_, me_y_ - 1) ||
		    is_match(objid::target, me_x_, me_y_ - 1)) {
			resdir |= dir::nowth;
		}
	} catch (...) {
	}

	try {
		if (is_match(objid::path, me_x_ - 1, me_y_) ||
		    is_match(objid::target, me_x_ - 1, me_y_)) {
			resdir |= dir::west;
		}
	} catch (...) {
	}

	try {
		if (is_match(objid::path, me_x_, me_y_ + 1) ||
		    is_match(objid::target, me_x_, me_y_ + 1)) {
			resdir |= dir::south;
		}
	} catch (...) {
	}

	return resdir;
}

/**
 * @brief me は dir 方向へ移動可能か
 *
 * @param dir
 * @return true
 * @return false
 */
bool map::is_accesible_to(dirbit dir) const
{
	return (get_accesible_dir() & dir) != dir::none;
}

/**
 * @brief me を現在地から (dx, dy) だけ移動させる
 * 移動前の地点は visited になる
 *
 * @param dx
 * @param dy
 * @return int 移動回数, 移動不可時に -1
 */
int32_t map::moveme_with(dirbit dir)
{
	if (!is_accesible_to(dir)) {
		return -1;
	}

	int32_t result_x = me_x_, result_y = me_y_, ret = 0;

	if (dir & dir::east) {
		result_x++;
		ret++;
	}
	if (dir & dir::nowth) {
		result_y--;
		ret++;
	}
	if (dir & dir::west) {
		result_x--;
		ret++;
	}
	if (dir & dir::south) {
		result_y++;
		ret++;
	}

	set(objid::visited, me_x_, me_y_);
	set(objid::me, result_x, result_y);
	me_x_ = result_x;
	me_y_ = result_y;
	return ret;
}

class chaser_meat
{
	map origin_map_;

	map marked_map_;
	int32_t moved_;
	int32_t min_moved_;

	void min_move_to(uint32_t goal_x, uint32_t goal_y);

public:
	chaser_meat() : origin_map_(), marked_map_(origin_map_), moved_(0), min_moved_(-1) {}
	~chaser_meat() {}

	void answer();
};

void chaser_meat::min_move_to(uint32_t goal_x, uint32_t goal_y)
{
	sleep(1);
	printf("moved=%d, min=%d\n", moved_, min_moved_);
	marked_map_.print();

	if (moved_ == 0) {
		marked_map_ = origin_map_;
	}

	if ((marked_map_.me_x() == goal_x) && (marked_map_.me_y() == goal_y)) {
		if (min_moved_ < 0) {
			min_moved_ = moved_;
		}
		else {
			min_moved_ = (moved_ < min_moved_) ? moved_ : min_moved_;
		}
		return;
	}

	if (marked_map_.moveme_with(dir::east) < 0) {
		// return;
	}
	else {
		moved_++;
		min_move_to(goal_x, goal_y);
	}

	if (marked_map_.moveme_with(dir::nowth) < 0) {
		// return;
	}
	else {
		moved_++;
		min_move_to(goal_x, goal_y);
	}

	if (marked_map_.moveme_with(dir::west) < 0) {
		// return;
	}
	else {
		moved_++;
		min_move_to(goal_x, goal_y);
	}

	if (marked_map_.moveme_with(dir::south) < 0) {
		// return;
	}
	else {
		moved_++;
		min_move_to(goal_x, goal_y);
	}
}

void chaser_meat::answer()
{
	origin_map_.print();
	min_move_to(4, 2);
	printf("%d\n", min_moved_);
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
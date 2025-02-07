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

/**
 * @brief オブジェクトID
 */
enum class objid : char
{
	path = '.',
	wall = '#',
};

/**
 * @brief 座標
 */
struct pos {
	int32_t x_;
	int32_t y_;

	pos(int32_t x, int32_t y) : x_(x), y_(y) {}

	pos operator+(const pos& p) const { return pos(x_ + p.x_, y_ + p.y_); }
	bool operator==(const pos& p) const
	{
		return (this->x_ == p.x_) && (this->y_ == p.y_);
	}
	bool operator!=(const pos& p) const { return !(*this == p); }
};

using locus = std::vector<pos>; /** 軌跡は座標の vector で記録 */
using dirbit = uint8_t;

/**
 * @brief 方角空間
 * 各 bit が各方角を表す
 * テーブルは foreach 用
 */
namespace dir
{
	static uint8_t none = 0x0;
	static uint8_t east = 0x1;
	static uint8_t nowth = 0x2;
	static uint8_t west = 0x4;
	static uint8_t south = 0x8;

	struct {
		int idx_;
		dirbit bit_;
		pos vector_;
	} dirtbl[] = {{0, east, {1, 0}},
		      {1, nowth, {0, -1}},
		      {2, west, {-1, 0}},
		      {3, south, {0, 1}},
		      {-1, none, {0, 0}}};

}  // namespace dir

/**
 * @brief 地図
 */
class map
{
	uint32_t w_;
	uint32_t h_;
	objid* m_;

	pos start_;
	pos goal_;

public:
	map();
	~map() { delete[] m_; }

	uint32_t width() const { return w_; }
	uint32_t height() const { return h_; }

	const pos& start() { return start_; }
	const pos& goal() { return goal_; }

	objid get(const pos& pos) const;
	void set(objid id, const pos& pos);

	bool is_matching_to(objid id, const pos& pos) const { return get(pos) == id; }

	void print() const;
};

/**
 * @brief Construct a new map::map object
 * 標準入力をもとに地図を生成
 */
map::map() : w_(0), h_(0), m_(nullptr), start_({0, 0}), goal_({0, 0})
{
	vint32 iv;

	h_ = iv[0];
	w_ = iv[1];

	m_ = new objid[w_ * h_];

	for (int32_t y = 0; y < (int32_t)h_; y++) {
		fstring fstr;
		for (int32_t x = 0; x < (int32_t)w_; x++) {
			if (fstr[x] == 'A') {
				start_ = {x, y};
				set(objid::path, {x, y});
				continue;
			}
			else if (fstr[x] == 'B') {
				goal_ = {x, y};
				set(objid::path, {x, y});
				continue;
			}
			set((objid)fstr[x], {x, y});
		}
	}
}

/**
 * @brief 指定の座標の値(objid)を取得
 *
 * @param pos 座標
 * @return objid 成功時に objid
 *
 * @exception 範囲不適時に例外 -1
 */
objid map::get(const pos& pos) const
{
	if ((pos.x_ < 0) || (pos.x_ > (int32_t)w_ - 1) || (pos.y_ < 0) ||
	    (pos.y_ > (int32_t)h_ - 1)) {
		throw -1;
	}
	return m_[pos.y_ * w_ + pos.x_];
}

/**
 * @brief 指定の座標に指定の値(objid)を代入
 *
 * @param id
 * @param pos 座標
 *
 * @exception 範囲不適時に例外 -1
 */
void map::set(objid id, const pos& pos)
{
	if ((pos.x_ < 0) || (pos.x_ > (int32_t)w_ - 1) || (pos.y_ < 0) ||
	    (pos.y_ > (int32_t)h_ - 1)) {
		throw -1;
	}
	m_[pos.y_ * w_ + pos.x_] = id;
}

/**
 * @brief 地図を出力
 */
void map::print() const
{
	for (int32_t y = 0; y < (int32_t)h_; y++) {
		for (int32_t x = 0; x < (int32_t)w_; x++) {
			printf("%c", (char)get({x, y}));
		}
		printf("\n");
	}
}

/**
 * @brief 分岐情報
 */
struct branch {
	/** 分岐点までの軌跡 */
	locus locus_;
	/** 分岐点での移動可能な残りの方角 */
	dirbit restdirs_;

	branch(locus l, dirbit restdirs) : locus_(l), restdirs_(restdirs) {}
};

class chaser_meat
{
	map map_;
	std::vector<branch> branches_;

	int32_t movetime_;
	int32_t min_movetime_;

	dirbit accessible_dirbit_;

	locus& crnt_locus() { return branches_.back().locus_; }
	dirbit& restdirs() { return branches_.back().restdirs_; }
	pos& crnt_pos() { return crnt_locus().back(); }
	void branch_out();

	void reflesh_accessible_dirbit();
	uint8_t get_accesible_dirs() const;
	bool is_accessible_to(dirbit dir) const;

	void reflesh_min_movetime();
	int32_t move_to(dirbit dir);
	void min_move_to(const pos& dst);

public:
	chaser_meat();
	~chaser_meat() {}

	void answer();
};

/**
 * @brief Construct a new chaser meat::chaser meat object
 */
chaser_meat::chaser_meat()
    : map_(), movetime_(0), min_movetime_(-1), accessible_dirbit_(dir::none)
{
	locus epoc;
	epoc.push_back(map_.start());
	branches_.push_back({epoc, dir::none});
	reflesh_accessible_dirbit();
	restdirs() = accessible_dirbit_;
}

/**
 * @brief 分岐情報を分岐させる
 * それまでの軌跡をその瞬間の移動可能な方角の分だけ新たに生成する
 */
void chaser_meat::branch_out()
{
	locus loc;
	loc.push_back(crnt_pos());
	branches_.push_back({loc, accessible_dirbit_});
}

/**
 * @brief 軌跡の最終地点から移動可能な方角群を更新する
 */
void chaser_meat::reflesh_accessible_dirbit()
{
	using namespace dir;

	dirbit resdir = none;

	auto& crntloc = crnt_locus();
	auto& crntpos = crnt_pos();
	for (int i = 0; dirtbl[i].idx_ != -1; i++) {
		try {
			if (!map_.is_matching_to(objid::path, crntpos + dirtbl[i].vector_)) {
				continue;
			}

			bool visited = false;
			for (auto l = crntloc.rbegin(); l != crntloc.rend(); l++) {
				if (*l == crntpos + dirtbl[i].vector_) {
					visited = true;
					break;
				}
			}

			if (!visited) {
				resdir |= dirtbl[i].bit_;
			}
		} catch (...) {
		}
	}
	accessible_dirbit_ = resdir;
}

/**
 * @brief 軌跡の最終地点から移動可能な方角の数を取得する
 *
 * @return uint8_t
 */
uint8_t chaser_meat::get_accesible_dirs() const
{
	using namespace dir;
	return !!is_accessible_to(east) + !!is_accessible_to(nowth) +
	       !!is_accessible_to(west) + !!is_accessible_to(south);
}

/**
 * @brief 軌跡の最終地点から dir 方向へ移動可能か
 *
 * @param dir
 * @return true
 * @return false
 */
bool chaser_meat::is_accessible_to(dirbit dir) const
{
	return accessible_dirbit_ & dir;
}

/**
 * @brief 最小移動回数を更新する
 */
void chaser_meat::reflesh_min_movetime()
{
	if (min_movetime_ < 0) {
		min_movetime_ = movetime_;
	}
	else {
		min_movetime_ = (movetime_ < min_movetime_) ? movetime_ : min_movetime_;
	}
}

/**
 * @brief 指定の方角に 1 だけ移動する
 * 方角は単方向でなければならない
 *
 * @param dir
 * @return int32_t 移動成功時に 0, 移動できない場合, もしくは不適切方角指定時に -1
 */
int32_t chaser_meat::move_to(dirbit dir)
{
	using namespace dir;

	if (!is_accessible_to(dir)) {
		return -1;
	}

	auto& crntloc = crnt_locus();
	auto& crntpos = crnt_pos();
	for (int i = 0; dirtbl[i].idx_ != -1; i++) {
		if (dirtbl[i].bit_ == dir) {
			crntloc.push_back(crntpos + dirtbl[i].vector_);
			reflesh_accessible_dirbit();
			if (get_accesible_dirs() >= 2) {
				branch_out();
			}
			return 0;
		}
	}
	return -1;
}

void chaser_meat::min_move_to(const pos& dst)
{
#if 0
	auto& crntpos = crnt_pos();

	sleep(1);
	printf("move=%d, min=%d, (%d,%d)\n", movetime_, min_movetime_, crnt.x_, crnt.y_);

	if (crnt == map_.goal()) {
		reflesh_min_movetime();
		locus_.pop_back();
		return;
	}

	auto branches = map_.get_accesible_dirs(locus_);
	if (branches == 0) {
		locus_.pop_back();
		return;
	}
	else if (branches == 1) {
		get_map().moveme_with() min_move_to(goal_x, goal_y);
	}

	auto accesible_dir_bit = get_map().get_accesible_dir();
	for (dirbit nextdir = dir::none; nextdir != dir::term; nextdir <<= 1) {
		if (!(nextdir & accesible_dir_bit)) {
			continue;
		}
		record record(get_map(), nextdir);
		records_.push_back(record);
	}
#endif
}

void chaser_meat::answer()
{
	using namespace dir;

	map_.print();
	auto& crnt = crnt_pos();
	printf("moved=%d, min=%d, crnt=(%d,%d), (ENWS)=(%d,%d,%d,%d)\n", movetime_,
	       min_movetime_, crnt.x_, crnt.y_, is_accessible_to(east),
	       is_accessible_to(nowth), is_accessible_to(west), is_accessible_to(south));

	int ret = move_to(dir::east);
	crnt = crnt_pos();
	printf("moved=%d, min=%d, crnt=(%d,%d), (ENWS)=(%d,%d,%d,%d), ret=%d\n", movetime_,
	       min_movetime_, crnt.x_, crnt.y_, is_accessible_to(east),
	       is_accessible_to(nowth), is_accessible_to(west), is_accessible_to(south), ret);

	ret = move_to(dir::south);
	crnt = crnt_pos();
	printf("moved=%d, min=%d, crnt=(%d,%d), (ENWS)=(%d,%d,%d,%d), ret=%d\n", movetime_,
	       min_movetime_, crnt.x_, crnt.y_, is_accessible_to(east),
	       is_accessible_to(nowth), is_accessible_to(west), is_accessible_to(south), ret);

	// min_move_to(4, 2);
	// printf("%d\n", min_movetime_);
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
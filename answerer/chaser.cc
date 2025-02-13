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
#include <queue>
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
 * @brief 座標
 */
struct point {
	int32_t x_;
	int32_t y_;

	point() : x_(0), y_(0) {}
	point(int32_t x, int32_t y) : x_(x), y_(y) {}

	point operator+(const point& p) const { return point(x_ + p.x_, y_ + p.y_); }
	bool operator==(const point& p) const
	{
		return (this->x_ == p.x_) && (this->y_ == p.y_);
	}
	bool operator!=(const point& p) const { return !(*this == p); }
};

/**
 * @brief 方向テーブル
 */
struct {
	int idx_;
	point vector_;
} dirtbl[] = {{0, {1, 0}}, {1, {0, -1}}, {2, {-1, 0}}, {3, {0, 1}}, {-1, {0, 0}}};

/**
 * @brief 各点情報
 */
struct site {
	/** 通行可能か */
	bool is_path_;
	/** 訪問済みか */
	bool is_visited_;
	/** start からの移動距離 */
	uint32_t g_cost_;

	site() : is_path_(true), is_visited_(false), g_cost_(0xffffffff) {}
};

/**
 * @brief 地図
 */
class map
{
	uint32_t w_;
	uint32_t h_;
	site* sites_;

	point start_;
	point target_;

	site& get_site(const point& pos) const;

public:
	map();
	~map() { delete[] sites_; }

	uint32_t width() const { return w_; }
	uint32_t height() const { return h_; }

	const point& start() const { return start_; }
	const point& target() const { return target_; }

	bool is_accessible(const point& pos) const
	{
		return get_site(pos).is_path_ && !get_site(pos).is_visited_;
	}
	void set_visited(const point& pos) { get_site(pos).is_visited_ = true; }
	uint32_t g_cost_of(const point& pos) const { return get_site(pos).g_cost_; }
	void set_g_cost(const point& pos, uint32_t g) { get_site(pos).g_cost_ = g; }

	bool is_new() const;
	void reset();

	void print() const;
};

/**
 * @brief Construct a new map::map object
 * 標準入力をもとに地図を生成
 */
map::map() : w_(0), h_(0), sites_(nullptr), start_({0, 0}), target_({0, 0})
{
	vint32 iv;

	h_ = iv[0];
	w_ = iv[1];

	sites_ = new site[w_ * h_];

	for (int32_t y = 0; y < (int32_t)h_; y++) {
		fstring fstr;
		for (int32_t x = 0; x < (int32_t)w_; x++) {
			if (fstr[x] == 'A') {
				start_ = {x, y};
			}
			else if (fstr[x] == 'B') {
				target_ = {x, y};
			}
			else if (fstr[x] == '#') {
				get_site({x, y}).is_path_ = false;
			}
		}
	}
}

/**
 * @brief 指定の座標の site を取得
 *
 * @param pos 座標
 *
 * @exception 範囲不適時に例外 -1
 */
site& map::get_site(const point& pos) const
{
	if ((pos.x_ < 0) || (pos.x_ > (int32_t)w_ - 1) || (pos.y_ < 0) ||
	    (pos.y_ > (int32_t)h_ - 1)) {
		throw -1;
	}
	return sites_[pos.y_ * w_ + pos.x_];
}

/**
 * @brief map が reset された状態か
 *
 * @return true
 * @return false
 */
bool map::is_new() const
{
	for (int32_t y = 0; y < (int32_t)h_; y++) {
		for (int32_t x = 0; x < (int32_t)w_; x++) {
			if ((get_site({x, y}).g_cost_ != 0xffffffff) ||
			    (get_site({x, y}).is_visited_ != false)) {
				return false;
			}
		}
	}
	return true;
}

/**
 * @brief 訪問済みフラグと記録した g 値を初期化する
 */
void map::reset()
{
	for (int32_t y = 0; y < (int32_t)h_; y++) {
		for (int32_t x = 0; x < (int32_t)w_; x++) {
			sites_[y * w_ + x].g_cost_ = 0xffffffff;
			sites_[y * w_ + x].is_visited_ = false;
		}
	}
}

/**
 * @brief 地図を出力
 */
void map::print() const
{
	for (int32_t y = 0; y < (int32_t)h_; y++) {
		for (int32_t x = 0; x < (int32_t)w_; x++) {
			if (start_ == (point){x, y}) {
				printf("A");
			}
			else if (target_ == (point){x, y}) {
				printf("B");
			}
			else if (get_site({x, y}).is_visited_) {
				printf("*");
			}
			else if (get_site({x, y}).is_path_) {
				printf(".");
			}
			else {
				printf("#");
			}
		}
		printf("\n");
	}
}

/**
 * @brief A* ノード
 */
struct a_node {
	/** 座標 */
	point point_;
	/** 予測距離 */
	uint32_t f_;
};
auto greatf = [](a_node& a, a_node& b) { return a.f_ > b.f_; };

/**
 * @brief A* によって最短経路を計算するクラス
 */
class a_star_calculator
{
	map map_;
	std::priority_queue<a_node, std::vector<a_node>, decltype(greatf)> a_nodes_;

public:
	a_star_calculator() : map_(), a_nodes_(greatf) {}
	~a_star_calculator() {}

	int32_t calculate_to(const point& dst);
	const map& get_map() const { return map_; }

	void reset()
	{
		map_.reset();
		while (!a_nodes_.empty()) {
			a_nodes_.pop();
		}
	}
};

/**
 * @brief ヒューリスティック関数
 * ここではマンハッタン距離
 *
 * @param p1
 * @param p2
 * @return uint32_t
 */
static uint32_t heuristic(const point& p1, const point& p2)
{
	return abs(p1.x_ - p2.x_) + abs(p1.y_ - p2.y_);
}

/**
 * @brief dst までの最小移動回数を A* で得る
 *
 * @param dst
 */
int32_t a_star_calculator::calculate_to(const point& dst)
{
	a_node crnt_node;

	if (a_nodes_.empty()) {
		if (!map_.is_new()) {
			/* ゴール不可 */
			return -1;
		}
		/* スタート地点 */
		uint32_t g = 0;
		map_.set_g_cost(map_.start(), g);
		a_nodes_.push({map_.start(), heuristic(map_.start(), dst)});
		return calculate_to(dst);
	}

	crnt_node = a_nodes_.top();
	a_nodes_.pop();
	if (crnt_node.point_ == dst) {
		/* ゴール */
		return map_.g_cost_of(crnt_node.point_);
	}

	map_.set_visited(crnt_node.point_);
	for (int i = 0; dirtbl[i].idx_ != -1; i++) {
		try {
			a_node next_node({crnt_node.point_ + dirtbl[i].vector_, 0});
			if (!map_.is_accessible(next_node.point_)) {
				continue;
			}

			auto pre_g = map_.g_cost_of(crnt_node.point_) + 1;
			if (pre_g >= map_.g_cost_of(next_node.point_)) {
				continue;
			}
			map_.set_g_cost(next_node.point_, pre_g);
			next_node.f_ = pre_g + heuristic(next_node.point_, dst);
			a_nodes_.push(next_node);
		} catch (...) {
			continue;
		}
	}
	return calculate_to(dst);
}

/**
 * @brief ゴール(i.e. B から見て四方に, 壁に突き当たるまでに座標)を得る
 */
static void get_goals(const map& map, std::vector<point>& goals)
{
	for (int i = 0; dirtbl[i].idx_ != -1; i++) {
		point crnt_pos = map.target(), prev_pos = crnt_pos;
		while (1) {
			try {
				if (!map.is_accessible(crnt_pos)) {
					break;
				}
				prev_pos = crnt_pos;
				crnt_pos = crnt_pos + dirtbl[i].vector_;
			} catch (...) {
				break;
			}

			if (prev_pos != map.target()) {
				goals.push_back(prev_pos);
			}
		}
	}
}

/**
 * @brief main 相当部
 *
 * @return int
 */
int chaser::answer(void)
try {
	a_star_calculator a_star;
	std::vector<point> goals;
	uint32_t mindist = 0xffffffff;

	get_goals(a_star.get_map(), goals);
	for (auto& goal : goals) {
		auto dist = a_star.calculate_to(goal);
		mindist = ((uint32_t)dist < mindist) ? dist : mindist;
		a_star.reset();
	}
	printf("%d\n", mindist);
	return 0;
} catch (...) {
	return -1;
}
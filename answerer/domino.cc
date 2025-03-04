/**
 * @file domino.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "domino.hh"
#include "lib.hh"

/* =================================== 擬ライブラリ =================================== */

#include <queue>

namespace nsp_domino
{
const uint32_t MAX_PT = 10;

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

struct {
	int idx_;
	point vector_;
} dirtbl[] = {{0, {1, 1}}, {1, {0, 1}}, {2, {-1, 1}}, {-1, {0, 0}}};

struct site {
	uint32_t weight_;
	uint32_t g_cost_;

	site() : weight_(0), g_cost_(0xffffffff) {}
};

class map
{
	uint32_t w_;
	uint32_t h_;
	site* sites_;

	site& get_site(const point& pos) const;

public:
	map();
	~map() { delete[] sites_; }

	uint32_t width() const { return w_; }
	uint32_t height() const { return h_; }

	uint32_t weight_of(const point& pos) const { return get_site(pos).weight_; }
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
map::map() : w_(0), h_(0), sites_(nullptr)
{
	vinput v(5, 5, ' ', {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}, {}, "5 5");
	// vint32 vi;
	vint32 vi(v.get());

	h_ = vi[0];
	w_ = vi[1];

	sites_ = new site[w_ * h_];

	for (int32_t y = 0; y < (int32_t)h_; y++) {
		// vint32 vv;
		vint32 vv(v.get());
		for (int32_t x = 0; x < (int32_t)w_; x++) {
			get_site({x, y}).weight_ = MAX_PT - vv[x];
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
			if (get_site({x, y}).g_cost_ != 0xffffffff) {
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
			printf("%02d ", get_site({x, y}).weight_);
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

	point start_;
	point goal_;

public:
	a_star_calculator() : map_(), a_nodes_(greatf) {}
	~a_star_calculator() {}

	void set_edges(const point& start, const point& goal)
	{
		start_ = start;
		goal_ = goal;
	}
	bool is_invalid(const point& p) const
	{
		return (abs(p.x_ - goal_.x_) <= abs(p.y_ - goal_.y_));
	}

	int32_t calculate();
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
	return abs(p1.y_ - p2.y_) + abs(p1.x_ - p2.x_);
}

/**
 * @brief dst までの最小移動回数を A* で得る
 *
 * @param dst
 */
int32_t a_star_calculator::calculate()
{
	if (a_nodes_.empty()) {
		if (!map_.is_new()) {
			/* ゴール不可 */
			return -1;
		}
		/* スタート地点 */
		uint32_t g = 0;
		map_.set_g_cost(start_, g);
		a_nodes_.push({start_, heuristic(start_, goal_)});
		return calculate();
	}

	a_node crnt_node = a_nodes_.top();
	a_nodes_.pop();
	if (crnt_node.point_ == goal_) {
		/* ゴール */
		return map_.g_cost_of(crnt_node.point_);
	}

	for (int i = 0; dirtbl[i].idx_ != -1; i++) {
		try {
			a_node next_node({crnt_node.point_ + dirtbl[i].vector_, 0});
			if (!is_invalid(next_node.point_)) {
				continue;
			}

			auto pre_g = map_.g_cost_of(crnt_node.point_) +
				     map_.weight_of(next_node.point_);
			if (pre_g >= map_.g_cost_of(next_node.point_)) {
				continue;
			}
			map_.set_g_cost(next_node.point_, pre_g);
			next_node.f_ = pre_g + heuristic(next_node.point_, goal_);
			a_nodes_.push(next_node);
		} catch (...) {
			continue;
		}
	}
	return calculate();
}
}  // namespace nsp_domino

/**
 * @brief main 相当部
 *
 * @return int
 */
int domino::answer(void)
try {
	using namespace nsp_domino;

	a_star_calculator a_star;
	a_star.get_map().print();
	uint32_t mindist = 0xffffffff;

	for (uint32_t start_x = 0; start_x < a_star.get_map().width(); start_x++) {
		for (uint32_t goal_x = 0; goal_x < a_star.get_map().width(); goal_x++) {
			a_star.set_edges(
			    {(int32_t)start_x, 0},
			    {(int32_t)goal_x, (int32_t)a_star.get_map().height() - 1});
			auto dist = a_star.calculate();
			mindist = ((uint32_t)dist < mindist) ? dist : mindist;
			a_star.reset();
		}
	}

	printf("%d\n", MAX_PT - mindist);
	return 0;
} catch (...) {
	return -1;
}
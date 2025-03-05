/**
 * @file domino2.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "domino2.hh"
#include "lib.hh"

/* =================================== 擬ライブラリ =================================== */

#include <queue>

namespace nsp_domino2
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

class map
{
	uint32_t w_;
	uint32_t h_;
	uint32_t* weights_;

public:
	map();
	~map() { delete[] weights_; }

	uint32_t width() const { return w_; }
	uint32_t height() const { return h_; }

	uint32_t weight_of(const point& pos) const;

	void print() const;
};

/**
 * @brief Construct a new map::map object
 * 標準入力をもとに地図を生成
 */
map::map() : w_(0), h_(0), weights_(nullptr)
{
	vinput v(500, 500, ' ', {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}, {},
		 "500 500");
	xxxxxMARKERxxxxx;
	// vint32 vi;
	vint32 vi(v.get());

	h_ = vi[0];
	w_ = vi[1];

	weights_ = new uint32_t[w_ * h_];

	for (int32_t y = 0; y < (int32_t)h_; y++) {
		// vint32 vv;
		vint32 vv(v.get());
		for (int32_t x = 0; x < (int32_t)w_; x++) {
			weights_[y * w_ + x] = vv[x];
		}
	}
}

/**
 * @brief 指定の座標の weight を取得
 *
 * @param pos 座標
 *
 * @exception 範囲不適時に例外 -1
 */
uint32_t map::weight_of(const point& pos) const
{
	if ((pos.x_ < 0) || (pos.x_ > (int32_t)w_ - 1) || (pos.y_ < 0) ||
	    (pos.y_ > (int32_t)h_ - 1)) {
		throw -1;
	}
	return weights_[pos.y_ * w_ + pos.x_];
}

/**
 * @brief 地図を出力
 */
void map::print() const
{
	for (int32_t y = 0; y < (int32_t)h_; y++) {
		for (int32_t x = 0; x < (int32_t)w_; x++) {
			printf("%02d ", weight_of({x, y}));
		}
		printf("\n");
	}
}

/**
 * @brief 動的計画法によって目的の値を計算するクラス
 */
class dp_calculator
{
	map map_;
	uint32_t* info_;

	uint32_t& info_of(const point& p) { return info_[p.y_ * map_.width() + p.x_]; }

public:
	dp_calculator();
	~dp_calculator() { delete[] info_; }

	int32_t calculate();
	const map& get_map() const { return map_; }
};

/**
 * @brief Construct a new dp calculator::dp calculator object
 */
dp_calculator::dp_calculator() : map_(), info_(nullptr)
{
	info_ = new uint32_t[map_.width() * map_.height()];
	memset(info_, 0, map_.width() * map_.height() * sizeof(*info_));
	for (int32_t x = 0; x < (int32_t)map_.width(); x++) {
		info_of({x, 0}) = map_.weight_of({x, 0});
	}
}

/**
 * @brief dst までの最小移動回数を動的計画法で得る
 *
 * @param dst
 */
int32_t dp_calculator::calculate()
{
	auto max = [](uint32_t a, uint32_t b) { return (a > b) ? a : b; };

	for (int32_t y = 1; y < (int32_t)map_.height(); y++) {
		try {
			for (int32_t x = 0; x < (int32_t)map_.width(); x++) {
				int from_ul = (x >= 1) ? info_of({x - 1, y - 1}) : 0;
				int from_u = info_of({x, y - 1});
				int from_ur = (x <= (int32_t)map_.width() - 2)
						  ? info_of({x + 1, y - 1})
						  : 0;
				info_of({x, y}) = map_.weight_of({x, y}) +
						  max(from_ul, max(from_u, from_ur));
			}
		} catch (...) {
			continue;
		}
	}

	uint32_t ret = 0;
	for (int32_t x = 0; x < (int32_t)map_.width(); x++) {
		ret = max(info_of({x, (int32_t)map_.height() - 1}), ret);
	}
	return ret;
}
}  // namespace nsp_domino2

/**
 * @brief main 相当部
 *
 * @return int
 */
int domino2::answer(void)
try {
	using namespace nsp_domino2;

	dp_calculator dp;
	printf("%d\n", dp.calculate());
	return 0;
} catch (...) {
	return -1;
}
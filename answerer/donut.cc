/**
 * @file donut.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "donut.hh"
#include "lib.hh"

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

class map_d
{
	uint32_t width_;
	uint32_t height_;
	bool* is_spot_;

	bool& is_spot(const point& pos) const
	{
		if ((pos.x_ < 0) || (pos.x_ > (int32_t)width_ - 1) || (pos.y_ < 0) ||
		    (pos.y_ > (int32_t)height_ - 1)) {
			throw -1;
		}
		return is_spot_[pos.y_ * width_ + pos.x_];
	}

	bool is_donut_at(const point& lupos) const;

public:
	map_d();
	~map_d() { delete[] is_spot_; }

	uint32_t donuts() const;

	void print() const;
};

map_d::map_d() : width_(0)
{
	vint32 size;

	height_ = size[0];
	width_ = size[1];
	is_spot_ = new bool[width_ * height_];
	memset(is_spot_, true, width_ * height_ * sizeof(*is_spot_));

	for (int32_t y = 0; y < (int32_t)height_; y++) {
		fstring fstr;
		for (int32_t x = 0; x < (int32_t)width_; x++) {
			if (fstr[x] == '#') {
				is_spot({x, y}) = false;
			}
		}
	}
}

bool map_d::is_donut_at(const point& lupos) const
{
	return !is_spot(lupos) && !is_spot(lupos + (point){1, 0}) &&
	       !is_spot(lupos + (point){2, 0}) && !is_spot(lupos + (point){0, 1}) &&
	       is_spot(lupos + (point){1, 1}) && !is_spot(lupos + (point){2, 1}) &&
	       !is_spot(lupos + (point){0, 2}) && !is_spot(lupos + (point){1, 2}) &&
	       !is_spot(lupos + (point){2, 2});
}

uint32_t map_d::donuts() const
{
	uint32_t cnt = 0;
	for (int32_t y = 0; y <= (int32_t)height_ - 3; y++) {
		for (int32_t x = 0; x <= (int32_t)width_ - 3; x++) {
			cnt += is_donut_at({x, y}) ? 1 : 0;
		}
	}
	return cnt;
}

/**
 * @brief main 相当部
 *
 * @return int
 */
int donut::answer(void)
try {
	map_d m;
	printf("%d\n", m.donuts());
	return 0;
} catch (...) {
	return -1;
}
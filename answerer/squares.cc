/**
 * @file squares.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "squares.hh"
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

class map_s
{
	uint32_t size_;
	bool* is_spot_;

	bool& is_spot(const point& pos) const
	{
		if ((pos.x_ < 0) || (pos.x_ > (int32_t)size_ - 1) || (pos.y_ < 0) ||
		    (pos.y_ > (int32_t)size_ - 1)) {
			throw -1;
		}
		return is_spot_[pos.y_ * size_ + pos.x_];
	}

	uint32_t rd_mindist(const point& pos) const
	{
		return size_ - ((pos.x_ > pos.y_) ? pos.x_ : pos.y_) - 1;
	}
	uint32_t rd_squares_at(const point& pos) const;

public:
	map_s();
	~map_s() { delete[] is_spot_; }

	uint32_t squares() const;

	void print() const;
};

map_s::map_s() : size_(0)
{
	// vint32 size;
	vinput vi = {"5", ".....", "...#.", ".....", ".....", "....."};
	vint32 size(vi.get().c_str());

	size_ = size[0];
	is_spot_ = new bool[size_ * size_];
	memset(is_spot_, true, size_ * size_ * sizeof(*is_spot_));

	for (int32_t y = 0; y < (int32_t)size_; y++) {
		// fstring fstr;
		auto& fstr = vi.get();
		for (int32_t x = 0; x < (int32_t)size_; x++) {
			if (fstr[x] == '#') {
				is_spot({x, y}) = false;
			}
		}
	}
}

uint32_t map_s::rd_squares_at(const point& pos) const
{
	if (!is_spot(pos)) {
		return 0;
	}

	uint32_t cnt = 0;
	for (int32_t size = 1; size <= (int32_t)rd_mindist(pos); size++) {
		if (is_spot(pos + (point){size, 0}) && is_spot(pos + (point){0, size}) &&
		    is_spot(pos + (point){size, size})) {
			cnt++;
		}
	}
	return cnt;
}

uint32_t map_s::squares() const
{
	uint32_t cnt = 0;
	for (int32_t y = 0; y < (int32_t)size_; y++) {
		for (int32_t x = 0; x < (int32_t)size_; x++) {
			cnt += rd_squares_at({x, y});
		}
	}
	return cnt;
}

/**
 * @brief 地図を出力
 */
void map_s::print() const
{
	for (int32_t y = 0; y < (int32_t)size_; y++) {
		for (int32_t x = 0; x < (int32_t)size_; x++) {
			if (is_spot({x, y})) {
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
 * @brief main 相当部
 *
 * @return int
 */
int squares::answer(void)
try {
	map_s m;
	m.print();
	printf("%d\n", m.squares());
	return 0;
} catch (...) {
	return -1;
}
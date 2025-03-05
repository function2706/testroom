/**
 * @file lib.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include "lib.hh"

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
 * @brief 以下のフォーマットに則した文字列から各カラムの整数を得る
 * "[整数] [整数] ... [整数]"
 * 整数は末尾に追加される
 *
 * @param str フォーマットに即した文字列
 */
void vint32::add(const char* str)
{
	char *dstr = nullptr, *term, *head;
	size_t restlen = strlen(str);

	dstr = new char[restlen + 1];
	memset(dstr, 0, (restlen + 1) * sizeof(char));

	strcpy(dstr, str);
	head = dstr;
	while (restlen > 0) {
		term = strchr(head, ' ');
		if (term != nullptr) {
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
	delete[] dstr;
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

/**
 * @brief Construct a new vinput::vinput object
 * ・cols 列の文字列を num 個, strs に登録されている文字列群でランダムに生成する(LF止め)
 * ・ただし fixstrs 内の文字列は必ず1回登場する
 * ・区切り文字は delim, ただし -1 の場合は各列を詰めて生成する
 * ・初めの文字列は topstr となる
 *
 * 例：
 * (2, 3, ' ', {".", "#"}, {"A", "B"}, "2 3") -> {"2 3\n", ". A #\n", "B . .\n"} など
 * (1, 5, -1, {"a", "b", "c"}, {}, "5")        -> {"5\n", "bacca\n"} など
 *
 * @param num
 * @param cols
 * @param delim
 * @param strs
 * @param fixstrs
 * @param topstr
 */
vinput::vinput(uint32_t num, uint32_t cols, char delim,
	       std::initializer_list<std::string> strs,
	       std::initializer_list<std::string> fixstrs, std::string topstr)
{
	set(num, cols, delim, strs, fixstrs, topstr);
}

/**
 * @brief Construct a new vinput::vinput object
 * ・cols 列の文字列を num 個, strs に登録されている文字列群でランダムに生成する(LF止め)
 * ・ただし fixstrs 内の文字列は必ず1回登場する
 * ・区切り文字なし
 * ・初めの文字列は topstr となる
 *
 * 例：
 * (2, 3, {".", "#"}, {"A", "B"}, "2 3") -> {"2 3\n", ".A#\n", "B..\n"} など
 *
 * @param num
 * @param cols
 * @param strs
 * @param fixstrs
 * @param topstr
 */
vinput::vinput(uint32_t num, uint32_t cols, std::initializer_list<std::string> strs,
	       std::initializer_list<std::string> fixstrs, std::string topstr)
{
	set(num, cols, -1, strs, fixstrs, topstr);
}

/**
 * @brief Construct a new vinput::vinput object
 * ・cols 列の文字列を num 個, strs に登録されている文字列群でランダムに生成する(LF止め)
 * ・ただし fixstrs 内の文字列は必ず1回登場する
 * ・区切り文字は delim, ただし -1 の場合は各列を詰めて生成する
 * ・初めの文字列は "num cols" となる
 *
 * 例：
 * (2, 3, ' ', {".", "#"}, {"A", "B"}) -> {"2 3\n", ". A #\n", "B . .\n"} など
 *
 * @param num
 * @param cols
 * @param delim
 * @param strs
 * @param fixstrs
 */
vinput::vinput(uint32_t num, uint32_t cols, char delim,
	       std::initializer_list<std::string> strs,
	       std::initializer_list<std::string> fixstrs)
{
	std::string topstr = std::to_string(num) + " " + std::to_string(cols);
	set(num, cols, delim, strs, fixstrs, topstr);
}

/**
 * @brief Construct a new vinput::vinput object
 * ・cols 列の文字列を num 個, strs に登録されている文字列群でランダムに生成する(LF止め)
 * ・ただし fixstrs 内の文字列は必ず1回登場する
 * ・区切り文字なし
 * ・初めの文字列は "num cols" となる
 *
 * 例：
 * (2, 3, {".", "#"}, {"A", "B"}) -> {"2 3\n", ".A#\n", "B..\n"} など
 *
 * @param num
 * @param cols
 * @param strs
 * @param fixstrs
 */
vinput::vinput(uint32_t num, uint32_t cols, std::initializer_list<std::string> strs,
	       std::initializer_list<std::string> fixstrs)
{
	std::string topstr = std::to_string(num) + " " + std::to_string(cols);
	set(num, cols, -1, strs, fixstrs, topstr);
}

/**
 * @brief Construct a new vinput::vinput object
 * ・cols 列(区切り文字は delim)の文字列を num 個,
 *   strs に登録されている文字列群でランダムに生成する(LF止め)
 * ・ただし fixstrs 内の文字列は必ず1回登場する
 * ・delim が -1 の場合は各列を詰めて生成する
 * ・初めの文字列は topstr となる
 *
 * 例：
 * (2, 3, ' ', {".", "#"}, {"A", "B"}, "2, 3") -> {"2, 3\n", ". A #\n", "B . .\n"} など
 * (1, 5, -1, {"a", "b", "c"}, {}, "5")        -> {"5\n", "bacca\n"} など
 *
 * @param num
 * @param cols
 * @param delim
 * @param strs
 * @param fixstrs
 * @param topstr
 */
void vinput::set(uint32_t num, uint32_t cols, char delim,
		 std::initializer_list<std::string> strs,
		 std::initializer_list<std::string> fixstrs, std::string topstr)
{
	randval rand;
	std::vector<std::string> q_strs(strs);
	std::vector<std::string> q_fixstrs(fixstrs);
	struct fixstr_pos {
		uint32_t row_;
		uint32_t col_;
		std::string c_;

		fixstr_pos(randval& r, uint32_t num, uint32_t cols, const std::string& fstr)
		    : c_(fstr)
		{
			reflesh(r, num, cols);
		}
		void reflesh(randval& r, uint32_t num, uint32_t cols)
		{
			row_ = r.get(num - 1);
			col_ = r.get(cols - 1);
		}
	};
	std::vector<fixstr_pos> fixstr_posset;

	/* fixstr 場所決定 */
	for (auto& fstr : fixstrs) {
		fixstr_pos fstr_pos(rand, num, cols, fstr);
		while (1) {
			/* 重複チェック */
			bool is_redundant = false;
			for (auto& old_fstr_pos : fixstr_posset) {
				if ((fstr_pos.row_ == old_fstr_pos.row_) &&
				    (fstr_pos.col_ == old_fstr_pos.col_)) {
					is_redundant = true;
					break;
				}
			}
			if (is_redundant) {
				fstr_pos.reflesh(rand, num, cols);
				continue;
			}
			break;
		}
		fixstr_posset.push_back(fstr_pos);
	}

	/* strs 格納 */
	strs_.push_back(topstr + '\n');
	for (uint32_t n = 0; n < num; n++) {
		std::string str;
		for (uint32_t c = 0; c < cols; c++) {
			bool flag_fixstr = false;
			for (auto& fstr_pos : fixstr_posset) {
				if ((fstr_pos.row_ == n) && (fstr_pos.col_ == c)) {
					str += fstr_pos.c_;
					flag_fixstr = true;
					break;
				}
			}

			if (flag_fixstr) {
				continue;
			}

			if ((delim == -1) || (c == cols - 1)) {
				str += q_strs[rand.get(strs.size() - 1)];
			}
			else {
				str += q_strs[rand.get(strs.size() - 1)] + delim;
			}
		}
		str += '\n';
		strs_.push_back(str);
	}
}

/**
 * @brief FIFO で文字列を取得する
 * 最終的には当該文字列はデキューされる
 *
 * @return const std::string&
 */
const std::string vinput::get()
{
	auto ret = strs_.front();
	strs_.pop_front();
	return ret;
}

/**
 * @brief 文字列群を出力する
 */
void vinput::print() const
{
	for (auto& str : strs_) {
		printf("%s", str.c_str());
	}
}
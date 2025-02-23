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

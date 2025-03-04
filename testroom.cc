/**
 * @file testroom.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#include <getopt.h>
#include <pthread.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <memory>

#include "answerer/battle.hh"
#include "answerer/beacon.hh"
#include "answerer/chaser.hh"
#include "answerer/cookie.hh"
#include "answerer/domino.hh"
#include "answerer/donut.hh"
#include "answerer/pyramid.hh"
#include "answerer/squares.hh"
#include "answerer/symmetry.hh"

/* Version */
#define MAJOR_NUMBER 1
#define MINOR_NUMBER 0
#define REVISION_NUMBER 0

namespace nsp_testroom
{
static void print_usage()
{
	printf("Usage\n");
	printf("-h, --help                 : This help message.\n");
	printf("-v, --version              : Print version.\n");
	printf("-l, --list                 : Print list of libs\n");
	printf("-a, --answer <name or num> : Answer question.\n");
}

static void print_version()
{
	printf("Testroom for Online Programming Question.\n");
	printf("Version %d.%d.%d.\n", MAJOR_NUMBER, MINOR_NUMBER, REVISION_NUMBER);
}

static struct anstable {
	answerer* body;
	~anstable() { delete body; }
} anstbl[] = {new pyramid, new beacon, new chaser, new cookie, new symmetry,
	      new squares, new battle, new donut,  new domino};

static void print_list()
{
	int idx = 0;
	for (auto& ans : anstbl) {
		printf("%d.\t%s\n", ++idx, ans.body->name().c_str());
	}
}

static int answer(int idx)
{
	int i = 0;
	for (auto& ans : anstbl) {
		if (++i != idx) {
			continue;
		}
		printf("Run %s...\n", ans.body->name().c_str());
		int ret = ans.body->answer();
		printf("Break %s with retval=%d\n", ans.body->name().c_str(), ret);
		return ret;
	}
	printf("No such question! See below...\n");
	print_list();
	return 0;
}

static int answer(const std::string& name)
{
	for (auto& ans : anstbl) {
		if (ans.body->name() != name) {
			continue;
		}
		printf("Run %s...\n", ans.body->name().c_str());
		int ret = ans.body->answer();
		printf("Break %s with retval=%d.\n", ans.body->name().c_str(), ret);
		return ret;
	}
	printf("No such question! See below...\n");
	print_list();
	return 0;
}
}  // namespace nsp_testroom

int main(int argc, char** argv)
{
	using namespace nsp_testroom;

	int longindex, opt, ret = -1, idx = 0;
	struct option longopt[] = {{"help", no_argument, nullptr, 'h'},
				   {"version", no_argument, nullptr, 'v'},
				   {"list", no_argument, nullptr, 'l'},
				   {"answer", required_argument, nullptr, 'a'},
				   {nullptr, 0, nullptr, 0}};
	char* endptr = nullptr;

	while ((opt = getopt_long(argc, argv, "hvla:", longopt, &longindex)) != -1) {
		switch (opt) {
		case 'h':
			print_usage();
			break;
		case 'v':
			print_version();
			break;
		case 'l':
			print_list();
			break;
		case 'a':
			idx = strtol(optarg, &endptr, 10);
			if (*endptr != '\0') {
				ret = answer(optarg);
			}
			else {
				ret = answer(idx);
			}
			return ret;
		default:
			print_usage();
			printf("And...\n");
			print_list();
			break;
		}
	}

	return 0;
}

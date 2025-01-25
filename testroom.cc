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

#include "answerer/pyramid.hh"

/* Version */
#define MAJOR_NUMBER 1
#define MINOR_NUMBER 0
#define REVISION_NUMBER 0

static void print_usage()
{
	printf("Usage\n");
	printf("-h, --help           : This help message.\n");
	printf("-v, --version        : Print version.\n");
	printf("-l, --list           : Print list of libs\n");
	printf("-a, --answer <q num> : Answer question.\n");
}

static void print_version()
{
	printf("Testroom for Online Programming Question.\n");
	printf("Version %d.%d.%d.\n", MAJOR_NUMBER, MINOR_NUMBER, REVISION_NUMBER);
}

static struct {
	int label;
	answerer* ans;
	const char* detail;
} anstbl[] = {{1, new pyramid, "pyramid"}, {0, nullptr, nullptr}};

static void print_details(int label)
{
	int i;

	for (i = 0; anstbl[i].label != 0; i++) {
		if (anstbl[i].label != label)
			continue;

		printf("%d.\t%s\n", anstbl[i].label, anstbl[i].detail);
	}
}

static void print_list()
{
	int i;

	for (i = 0; anstbl[i].label != 0; i++) {
		print_details(anstbl[i].label);
	}
}

static int answer(int label)
{
	for (int i = 0; anstbl[i].label != 0; i++) {
		if (anstbl[i].label != label) {
			continue;
		}

		return anstbl[i].ans->answer();
	}
	return 0;
}

int main(int argc, char** argv)
{
	int longindex, opt, ret = -1;
	struct option longopt[] = {{"help", no_argument, nullptr, 'h'},
				   {"version", no_argument, nullptr, 'v'},
				   {"list", no_argument, nullptr, 'l'},
				   {"answer", required_argument, nullptr, 'a'},
				   {nullptr, 0, nullptr, 0}};

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
			ret = answer(atoi(optarg));
			printf("ret = %d\n", ret);
			break;

		default:
			print_usage();
			printf("And...\n");
			print_list();
			break;
		}
	}

	return 0;
}
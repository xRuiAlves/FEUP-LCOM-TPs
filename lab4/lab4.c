#include "test4.h"
#include "stdio.h"
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	/* DO NOT FORGET TO initialize service */
	sef_startup();	// For synchronizing with ressurection server

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("\nUsage: one of the following:\n\n"
	"service run %s -args \"packet <decimal no.- count>\"\n"
	"service run %s -args \"async <decimal no.- idle_time>\"\n"
	"service run %s -args \"remote <decimal no. - period> <decimal no. - count>\"\n"
    "service run %s -args \"gesture <decimal no.- length>\"\n\n",
	argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long count, idle_time, period, length;
	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("test4: wrong no. of arguments for mouse_test_packet()\n");
			return 1;
		}
		count = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (count == ULONG_MAX)
			return 1;
		printf("test4::mouse_test_packet(%lu)\n", count);
		return mouse_test_packet(count);
	}
	else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
			printf("test4: wrong no. of arguments for mouse_test_async()\n");
			return 1;
		}
        idle_time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (idle_time == ULONG_MAX)
			return 1;
		printf("test4::mouse_test_async(%lu)\n", idle_time);
		return mouse_test_async(idle_time);
	}
	else if (strncmp(argv[1], "remote", strlen("remote")) == 0) {
		if (argc != 4) {
			printf("test4: wrong no of arguments for mouse_test_remote()\n");
			return 1;
		}
		period = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
    	count = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if ((period == ULONG_MAX) || (count == ULONG_MAX))
			return 1;
		printf("test4::mouse_test_remote(%lu,%lu)\n", period, count);
		return mouse_test_remote(period,count);
	}
    else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if (argc != 3) {
			printf("test4: wrong no. of arguments for mouse_test_gesture()\n");
			return 1;
		}
        length = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (length == ULONG_MAX)
			return 1;
		printf("test4::mouse_test_gesture(%lu)\n", length);
		return mouse_test_gesture(length);
	}
	else {
		printf("kbd: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("kbd: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

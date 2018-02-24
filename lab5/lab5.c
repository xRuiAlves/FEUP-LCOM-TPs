#include "test5.h"
#include "stdio.h"
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "pixmaps.h"

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static short parse_short(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
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
	"service run %s -args \"init <decimal no.- mode> <decimal no.- delay>\"\n\n"
	"service run %s -args \"square <decimal no.- x> <decimal no.- y> <decimal no.- size> <decimal no.- color>\"\n\n"
	"service run %s -args \"line <decimal no. - xi> <decimal no. - yi> <decimal no. - xf> <decimal no. - yf> <decimal no. - color>\"\n\n"
	"service run %s -args \"test_xpm <decimal no. - pic_number> <decimal no. - xi> <decimal no. - yi>\"\n\n"
	"service run %s -args \"test_move <decimal no. - pic_number> <decimal no. - xi> <decimal no. - yi> <decimal no. - xf> <decimal no. - yf>  <decimal no. - speed> <decimal no. - frame_rate>\"\n\n"
	"service run %s -args \"test_ctrl\"\n\n",
	argv[0], argv[0], argv[0], argv[0],  argv[0],  argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long mode, delay, x, y, size, color, xi, yi, xf, yf, frame_rate, pic_number;
	short speed;
	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("test5: wrong no. of arguments for video_test_packet()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
        delay = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (mode == ULONG_MAX)
			return 1;
        if (delay == ULONG_MAX)
    		return 1;
		printf("test5::video_test_init(%lu, %lu)\n", mode, delay);
		video_test_init(mode, delay);
		return 0;
	}
	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("test5: wrong no. of arguments for video_test_square()\n");
			return 1;
		}
        x = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
        y = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
        size = parse_ulong(argv[4], 10);					/* Parses string to unsigned long */
        color = parse_ulong(argv[5], 10);					/* Parses string to unsigned long */

		if ((x == ULONG_MAX) || (y == ULONG_MAX) || (size == ULONG_MAX) || (color == ULONG_MAX))
			return 1;
		printf("test5::video_test_square(%lu, %lu, %lu, %lu)\n", x, y, size, color);
		return video_test_square(x, y, size, color);
	}
	else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("test5: wrong no of arguments for video_test_line()\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
    	yi = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
        xf = parse_ulong(argv[4], 10);						/* Parses string to unsigned long */
        yf = parse_ulong(argv[5], 10);						/* Parses string to unsigned long */
        color = parse_ulong(argv[6], 10);					/* Parses string to unsigned long */
		if ((xi == ULONG_MAX) || (yi == ULONG_MAX) || (xf == ULONG_MAX) || (yf == ULONG_MAX) || (color == ULONG_MAX))
			return 1;
		printf("test5::video_test_line(%lu,%lu, %lu, %lu, %lu)\n", xi, yi, xf, yf, color);
		return video_test_line(xi, yi, xf, yf, color);
	}
	else if (strncmp(argv[1], "test_xpm", strlen("test_xpm")) == 0) {
		if (argc != 5) {
			printf("test5: wrong no. of arguments for test_xpm()\n");
			return 1;
		}

		pic_number = parse_ulong(argv[2], 10);              /* Parses string to unsigned long */

		// Check if the pic_number is within range of the available pics
		if(pic_number<1 || pic_number>5)	return 1;

        xi = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
        yi = parse_ulong(argv[4], 10);						/* Parses string to unsigned long */

		if ((xi == ULONG_MAX) || (yi == ULONG_MAX))
			return 1;
		printf("test5::test_xpm(%lu, %lu, %lu)\n", pic_number, xi, yi);
		return test_xpm(pics[pic_number-1] , xi , yi);
	}
	else if (strncmp(argv[1], "test_move", strlen("test_move")) == 0) {
		if (argc != 9) {
			printf("test5: wrong no. of arguments for test_move()\n");
			return 1;
		}

		pic_number = parse_ulong(argv[2], 10);              /* Parses string to unsigned long */

		// Check if the pic_number is within range of the available pics
		if(pic_number<1 || pic_number>5)	return 1;

        xi = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
        yi = parse_ulong(argv[4], 10);						/* Parses string to unsigned long */
		xf = parse_ulong(argv[5], 10);						/* Parses string to unsigned long */
		yf = parse_ulong(argv[6], 10);						/* Parses string to unsigned long */
		speed = parse_short(argv[7], 10);					/* Parses string to short         */
		frame_rate =  parse_ulong(argv[8], 10);			    /* Parses string to unsigned long */


		if ((xi == ULONG_MAX) || (yi == ULONG_MAX) || (xf == ULONG_MAX) || (yf == ULONG_MAX) || (frame_rate == ULONG_MAX))
			return 1;
		printf("test5::test_move(%lu, %lu, %lu, %lu, %lu, %d, %lu)\n", pic_number, xi, yi, xf, yf, speed, frame_rate);
		return test_move(pics[pic_number-1] , xi , yi , xf , yf , speed , frame_rate);
	}
	else if (strncmp(argv[1], "test_ctrl", strlen("test_ctrl")) == 0) {
		if (argc != 2) {
			printf("test5: wrong no. of arguments for test_controller()\n");
			return 1;
		}

		printf("test5::test_controller()");
		return test_controller();
	}
	else {
		printf("VBE: %s - no valid function!\n", argv[1]);
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
		printf("VBE: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static short parse_short(char *str, int base)
{
	char *endptr;
	short val;


	/* Convert string to short */
	val = (short) strtol(str, &endptr, base);


	/* Successful conversion */
	return val;
}

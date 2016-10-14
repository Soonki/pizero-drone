/*
 * util.c
 *
 *  Created on: Oct 11, 2016
 *      Author: anand
 */

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include "util.h"
#include <string.h>
#include "config.h"
#include <unistd.h>

int utilOutput(output_lvl_t lvl, const char* message) {
	int ret = 0;
	switch (lvl) {
	case DEBUG_LVL:
		fprintf(stdout, message);
		ret = 0;
		break;
	case ERROR_LVL:
		fprintf(stderr, strcat(message, ":%s\n"), strerror(errno));
		ret = -1;
		break;
	default:
		fprintf(stdout, message);
		ret = 0;
		break;
	}
	return ret;
}

static int is_running = FALSE;

int sys_run()
{
	is_running = TRUE;
	return 0;
}

int sys_running()
{
	return is_running;
}

int	sys_exit()
{
	is_running = FALSE;
	return 0;
}

void waitForSys()
{
	while(sys_running() == FALSE) {
		usleep(1000);
	}
}

/*
 * main1.c
 *
 *  Created on: Oct 6, 2016
 *      Author: anand
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "config.h"
#include <pthread.h>
#include "gpio.h"
#include "server.h"
#include "stabilizer.h"
#include <stdlib.h>
#include "util.h"

int main(int argc, char **argv) {

	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	wiringPiSetupGpio();

	stabilizer_init();
	initServer(atoi(argv[1]));

	usleep(100);
	sys_run();
	while (sys_running()) {
		usleep(1000);
	}
	pthread_exit(NULL);
}


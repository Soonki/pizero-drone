/*
 * server.c
 *
 *  Created on: Oct 14, 2016
 *      Author: anand
 */

/* A simple server in the internet domain using TCP
 The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "stabilizer.h"
#include "util.h"

struct sockaddr_in serv_addr, cli_addr;
int sockfd, newsockfd, portno;
socklen_t clilen;
pthread_t srv_thr;

#define STD_BUF	4*sizeof(char)

void error(const char *msg);

void *server_loop(void* params) {
	char buffer[256];
	int n;

	while (sys_running()) {
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0)
			error("ERROR reading from socket");
		printf("Here is the message: \n%s\n", buffer);

		switch (n) {
		case 1:
			if (buffer[0] == 'X')
				sys_exit();
			break;
		case 4:
			setOrientation(buffer[0], buffer[1], buffer[2]);
			setThrust(buffer[3]);
			break;
		default:
			break;
		}

		n = write(newsockfd, "I got your message", 18);
		if (n < 0)
			error("ERROR writing to socket");

	}
	close(newsockfd);
	close(sockfd);
	pthread_exit(NULL);
}

void error(const char *msg) {
	perror(msg);
	sys_exit();
	exit(1);
}

int initServer(int port) {
	portno = port;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");

	int n = write(newsockfd, "Connected", 9);
	if (n < 0)
		error("ERROR writing to socket");
	sys_run();

	pthread_create(&srv_thr, NULL, server_loop, (void*) 0);

	pthread_exit(NULL);
}

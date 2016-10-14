/*
 * util.h
 *
 *  Created on: Oct 11, 2016
 *      Author: anand
 */

#ifndef SYS_INCLUDES_UTIL_H_
#define SYS_INCLUDES_UTIL_H_


typedef enum {
	DEBUG_LVL,
	ERROR_LVL,
	INFO_LVL
} output_lvl_t;


int utilOutput (output_lvl_t lvl, const char* message);

int sys_run();
int sys_running();
int	sys_exit();
void waitForSys();

#endif /* SYS_INCLUDES_UTIL_H_ */

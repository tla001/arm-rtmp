/*
 * main.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: tla001
 */

#include "Common.h"
#include "CapFbTest.h"
int runflag=0;
static void sig_user(int signo){
    if(signo==SIGINT){
    	runflag=0;
        printf("received SIGINT\n");
    }
}
CapFbTest ct;
int main()
{
	 if(signal(SIGINT,sig_user)==SIG_ERR)
	        perror("catch SIGINT err");
//	CapFbTest::v4l2Test();
//	CapFbTest::framebufferTest();
//	printf("hello\n");

	ct.CapVideoInit();
	runflag=1;
	while(runflag){
		ct.dispVideo();
	}
	return 0;
}

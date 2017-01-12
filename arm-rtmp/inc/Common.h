/*
 * Common.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "Include.h"
//#include "V4L2Capture.h"
//#include "FrameBufferDisp.h"
//#include "IpuProcess.h"
//#include "RTMPPushH264.h"
//
//#include "SDLDisp.h"



class Common {
public:
	Common();
	virtual ~Common();
	static void InfoDisp(int flag,char *info);
	static void ErrDisp(int flag,char *info);
	static void WarnDisp(int flag,char *info);

};

#endif /* COMMON_H_ */

/*
 * CapFbTest.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef CAPFBTEST_H_
#define CAPFBTEST_H_

#include "Common.h"
#include "V4L2Capture.h"
#include "FrameBufferDisp.h"
#include "IpuProcess.h"
#include "RTMPPushH264.h"

#include "SDLDisp.h"

class PushStream;
class CapFbTest {
public:
	CapFbTest();
	virtual ~CapFbTest();

	static void v4l2Test();
	static void framebufferTest();

	void CapVideoInit();
	void dispVideo();

private:
	unsigned char *rgbFrame;
	unsigned char *yuv420Frame;
	unsigned char *yuvFrame;
	unsigned int len;
	unsigned int size;
	int screen_w;
	int screen_h;

	V4L2Capture *cap;
	FrameBufferDisp *fb;
	IpuProcess *ipu;
	SDLDisp *sdl;
	friend class PushStream;
//	friend void PushStream::worker(const CapFbTest&);
	PushStream *pusher;
//	friend void PushStream::worker(const CapFbTest&);

};

#endif /* CAPFBTEST_H_ */

/*
 * RTMPPushH264.h
 *
 *  Created on: Jan 12, 2017
 *      Author: tla001
 */

#ifndef RTMPPUSHH264_H_
#define RTMPPUSHH264_H_

#include "RtmpH264.h"
#include <signal.h>
#include <error.h>
#include "ThreadBase.h"
#include <string>
#include "CapFbTest.h"

class CapFbTest;
using namespace std;
class PushStream :public ThreadBase {
public:
	explicit PushStream(string url,int width,int heigth,enum AVPixelFormat format,int fps);
	~PushStream();
	void initDevice();
	void closeDevice();

	void run();
	void worker(const CapFbTest &);
	void dowork(char *buf);
	void doPush();
private:
	string rtmpUrl;
	int width;
	int height;
	int outSize;
	enum AVPixelFormat src_pix_fmt;
	int baseFrameSzie;
	char *buffer;

	int fps;
	int rate;
	unsigned int tick;
	unsigned int tick_gap;
	uint32_t nowTime,lastTime ;
	int sleepTime;
	int frameIndex;

};

#endif /* RTMPPUSHH264_H_ */

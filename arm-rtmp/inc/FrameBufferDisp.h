/*
 * FrameBufferDisp.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef FRAMEBUFFERDISP_H_
#define FRAMEBUFFERDISP_H_

#include "Common.h"

class FrameBufferDisp {
public:
	FrameBufferDisp(char *devName);
	virtual ~FrameBufferDisp();

	int openDevice();
	int closeDevice();
	int initDevice();
	void display(unsigned char *frame,int screen_w,int screen_h);

private:
	char *devName;
	int fd;
	long int screensize;
	char *fb_buf;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
};

#endif /* FRAMEBUFFERDISP_H_ */

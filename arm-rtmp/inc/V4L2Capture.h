/*
 * V4L2Capture.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef V4L2CAPTURE_H_
#define V4L2CAPTURE_H_

#include "Common.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

class V4L2Capture {
public:
	V4L2Capture(char *devName);
	virtual ~V4L2Capture();

	int openDevice();
	int closeDevice();
	int initDevice();
	int startCapture();
	int stopCapture();
	int freeBuffers();
	int getFrame(void **,size_t *);
	int backFrame();

private:
	int initBuffers();

	struct cam_buffer
	{
		void* start;
		unsigned int length;
	};
	char *devName;
	int fd_cam;
	cam_buffer *buffers;
	unsigned int n_buffers;
	int frameIndex;
};

#endif /* V4L2CAPTURE_H_ */

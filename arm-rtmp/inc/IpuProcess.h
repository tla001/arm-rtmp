/*
 * IpuProcess.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef IPUPROCESS_H_
#define IPUPROCESS_H_

#include "Common.h"

class IpuProcess {
public:
	IpuProcess(char *devName);
	virtual ~IpuProcess();

	int openDevice();
	int closeDevice();
	int initDevice(int type);
	int IPUConvent(void *in,void *out);

private:
	char *devName;
	int fd;
	struct ipu_task taskCam;
	struct timeval begintime, endtime;
	unsigned int ipuOutputSize,ipuInputSize;
	void *inbuf;
	void *outbuf;
};

#endif /* IPUPROCESS_H_ */

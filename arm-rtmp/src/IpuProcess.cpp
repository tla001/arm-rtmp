/*
 * IpuProcess.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#include "IpuProcess.h"
#define CLEAR(x) memset(&(x), 0, sizeof(x))
IpuProcess::IpuProcess(char *devName) {
	// TODO Auto-generated constructor stub
	this->devName=devName;
	this->fd=-1;
	this->ipuInputSize=0;
	this->ipuOutputSize=0;
	this->inbuf=NULL;
	this->outbuf=NULL;
}

IpuProcess::~IpuProcess() {
	// TODO Auto-generated destructor stub
}

int IpuProcess::openDevice()
{
	/*设备的打开*/
	fd = open( devName, O_RDWR, 0 );
	if( fd<0 ){
		Common::ErrDisp(fd,"Can't open video device");
	}
	return 0;
}
int IpuProcess::closeDevice()
{
	if(inbuf)munmap(inbuf, ipuInputSize);
	if (taskCam.input.paddr)
			ioctl(fd, IPU_FREE, &taskCam.input.paddr);
	if(fd>0){
		int ret=0;
		if((ret=close(fd))<0){
			Common::ErrDisp(ret,"Can't close video device");
		}
		return 0;
	}
	else{
		return -1;
	}
}
int IpuProcess::initDevice(int type)
{
	int ret;
	CLEAR(taskCam);
	// Input image size and format
	taskCam.input.width    = 640;
	taskCam.input.height   = 480;
	taskCam.input.format   = v4l2_fourcc('Y', 'U', 'Y', 'V');
//
//	taskCam.input.crop.pos.x = 0;
//	taskCam.input.crop.pos.y = 0;
//	taskCam.input.crop.w = 0;
//	taskCam.input.crop.h = 0;

	// Output image size and format
	taskCam.output.width   = 640;
	taskCam.output.height  = 480;
	if(0==type){//framebuffer
		taskCam.output.format  = v4l2_fourcc('B', 'G', 'R', '3');
	}
	else if(type==1){//sdl yuv420p
		taskCam.output.format  = v4l2_fourcc('I', '4', '2', '0');
	}

//	taskCam.output.crop.pos.x = 300;
//	taskCam.output.crop.pos.y = 300;
//	taskCam.output.crop.w = 300;
//	taskCam.output.crop.h = 300;
	// Open IPU device
	ipuOutputSize=taskCam.output.paddr= taskCam.output.width * taskCam.output.height
			* fmt_to_bpp(taskCam.output.format)/8;
	printf("ipuOutputSize=%d\n",ipuOutputSize);
	ret = ioctl(fd, IPU_ALLOC, &taskCam.output.paddr);
	if (ret < 0) {
		Common::ErrDisp(ret,"ioctl IPU_ALLOC fail");
	}
	outbuf= mmap(0, ipuOutputSize, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, taskCam.output.paddr);
	if (!outbuf) {
		printf("mmap ipu output image fail\n");
		return -1;
	}
	ipuInputSize  =taskCam.input.paddr=taskCam.input.width * taskCam.input.height
				* fmt_to_bpp(taskCam.input.format)/8;
	printf("ipuInputSize=%d\n",ipuInputSize);
	ret = ioctl(fd, IPU_ALLOC, &taskCam.input.paddr);
	if (ret < 0) {
		Common::ErrDisp(ret,"IPU_ALLOC failed");
	}
	inbuf = mmap(0, ipuInputSize, PROT_READ | PROT_WRITE,
				MAP_SHARED, fd, taskCam.input.paddr);
	if (!inbuf) {
		printf("mmap ipu input image fail\n");
		return -1;
	}
	return 0;
}
int IpuProcess::IPUConvent(void *in,void *out)
{
	int ret;
	memcpy(inbuf, in, ipuInputSize);
	gettimeofday(&begintime, NULL);
	// Perform color space conversion
	ret = ioctl(fd, IPU_QUEUE_TASK, &taskCam);
	if (ret < 0) {
		printf("ioct IPU_QUEUE_TASK fail %x\n", ret);
		return ret;
	}
	gettimeofday(&endtime, NULL);
	double timeuse = 1000000*(endtime.tv_sec - begintime.tv_sec)+endtime.tv_usec-begintime.tv_usec;
	timeuse /=1000;//除以1000则进行毫秒计时，如果除以1000000则进行秒级别计时，如果除以1则进行微妙级别计时
	//printf("yuv2rgb use %f ms\n",timeuse);
	memcpy(out,outbuf,ipuOutputSize);
	return 0;
}

/*
 * CapFbTest.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#include "CapFbTest.h"
#include <string>
CapFbTest::CapFbTest() {
	// TODO Auto-generated constructor stub
	static char dev_video[]="/dev/video0";
	static char dev_fb[]="/dev/fb0";
	static char dev_ipu[]="/dev/mxc_ipu";
	static char outfile[]="captest.avi";
	static char outfile2[]="ccaptest.avi";
	static std::string url="rtmp://219.216.87.99/live/test1";

	this->screen_w=640;
	this->screen_h=480;
	size=screen_w*screen_h;
	cap=new  V4L2Capture(dev_video);
	fb=new FrameBufferDisp(dev_fb);
	ipu=new IpuProcess(dev_ipu);
	sdl=new SDLDisp(screen_w,screen_h);
	pusher=new PushStream(url,screen_w,screen_h,AV_PIX_FMT_YUV420P,25);



	this->rgbFrame=(unsigned char *)malloc(screen_w* screen_h* 3 * sizeof(char));
	this->yuv420Frame=(unsigned char *)malloc(screen_w* screen_h* 3 /2* sizeof(char));
	this->yuvFrame=NULL;
	this->len=0;


}

CapFbTest::~CapFbTest() {
	// TODO Auto-generated destructor stub

	fb->closeDevice();
	ipu->closeDevice();
	cap->closeDevice();
	sdl->closeSDL();
	pusher->join();
	pusher->closeDevice();

	free(rgbFrame);
	free(yuv420Frame);
}
void CapFbTest::CapVideoInit()
{
	cap->openDevice();
	cap->initDevice();

	fb->openDevice();
	fb->initDevice();

	ipu->openDevice();
	ipu->initDevice(1);//framebuffer sdl switch

	pusher->initDevice();

	if(cap->startCapture()<0){
		printf("start capture failed\n");

		cap->closeDevice();
		fb->closeDevice();
		ipu->closeDevice();
		sdl->closeSDL();
		pusher->closeDevice();
		exit(-1);
	}
	sdl->initSDL("V4L2 Video");
	pusher->doPush();
}
void CapFbTest::dispVideo()
{
	int ret;
	//转换部分
	ret=cap->getFrame((void **)&yuvFrame,&len);
	if(ret<0){
		printf("getFrame exit\n");
		exit(-1);
	}
	//printf("getFrame return len=%d\n",len);
	//显示部分
	//printf("disp\n");

	//framebuffer sdl switch
	//使用framebuffer显示
//	ipu->IPUConvent(yuvFrame,rgbFrame);
//	fb->display(rgbFrame,screen_w,screen_h);

	//使用SDL显示 420p
	ipu->IPUConvent(yuvFrame,yuv420Frame);
	sdl->normalSDLDisp(yuv420Frame);
//	pusher->dowork(( char *)yuv420Frame);

	//使用SDL显示 422
	//sdl->normalSDLDisp(yuvFrame);


	//将buffer返回队列
	ret=cap->backFrame();
	if(ret<0){
		printf("backFrame exit\n");
		exit(-1);
	}
}

void CapFbTest::v4l2Test()
{
	char dev[]="/dev/video0";
	V4L2Capture *cap=new  V4L2Capture(dev);
	cap->openDevice();
	cap->initDevice();

	cap->closeDevice();
}
void CapFbTest::framebufferTest(){
	char dev[]="/dev/fb0";
	FrameBufferDisp *fb=new FrameBufferDisp(dev);

	fb->openDevice();
	fb->initDevice();

	fb->closeDevice();
}

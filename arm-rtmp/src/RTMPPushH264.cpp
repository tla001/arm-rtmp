/*
 * RTMPPushH264.cpp
 *
 *  Created on: Jan 12, 2017
 *      Author: tla001
 */

#include "RTMPPushH264.h"
#include "librtmp/log.h"
#include <time.h>
extern int runflag;
extern CapFbTest ct;
PushStream::PushStream(string url,int widths,int heigths,enum AVPixelFormat format,int fps=25){
	this->rtmpUrl=url;
	this->width=widths;
	this->height=heigths;
	this->src_pix_fmt=src_pix_fmt;
	this->baseFrameSzie=width*height;
	this->buffer=new char[baseFrameSzie*3];

	this->fps=15;
	this->rate=100;
	this->outSize=512;
	tick=0;
	tick_gap=1000/this->fps;
	nowTime=0;
	lastTime=0;
	frameIndex=0;
}
PushStream::~PushStream(){
	closeDevice();
	delete []buffer;
}

void PushStream::initDevice(){
	RTMP_CreatePublish(const_cast<char*>(rtmpUrl.c_str()),outSize,1,RTMP_LOGINFO);
	RTMP_InitVideoParams(width,height,fps,rate,src_pix_fmt,false);
}
void PushStream::closeDevice(){
	RTMP_DeletePublish();
}
void PushStream::worker(const CapFbTest &ct){
	while(runflag){
		if(frameIndex!=0){
			timeval begintime,endtime;
			gettimeofday(&begintime, NULL);
			RTMP_SendScreenCapture((char*)buffer,height,tick);
			gettimeofday(&endtime, NULL);
			double timeuse = 1000000*(endtime.tv_sec - begintime.tv_sec)+endtime.tv_usec-begintime.tv_usec;
			timeuse /=1000;
			printf("frame index %d time use %lf ms\n",frameIndex,timeuse);
		}
		lastTime=RTMP_GetTime();
		switch(src_pix_fmt){
			case AV_PIX_FMT_YUV420P:
				//memcpy(buffer,ct.yuv420Frame,baseFrameSzie*3/2);
				buffer=(char *)ct.yuv420Frame;
				break;
			case AV_PIX_FMT_YUV422P:
				memcpy(buffer,ct.yuvFrame,baseFrameSzie*2);
				break;
			case AV_PIX_FMT_RGB24:
				memcpy(buffer,ct.rgbFrame,baseFrameSzie*3);
				break;
			default:
				printf("Not supports this format \n");
				break;
			}
		tick +=tick_gap;
		nowTime=RTMP_GetTime();
		usleep((tick_gap-nowTime+lastTime)*1000);
		frameIndex++;
	}
}
void PushStream::run(){
	worker(ct);
}
void PushStream::doPush(){
	this->start();
}
void PushStream::dowork(char *buf){
	if(frameIndex!=0){
			RTMP_SendScreenCapture((char*)buffer,height,tick);
			printf("send frame index -- %d\n",frameIndex);
		}
		lastTime=RTMP_GetTime();
		switch(src_pix_fmt){
			case AV_PIX_FMT_YUV420P:
				memcpy(buffer,buf,baseFrameSzie*3/2);
				break;
			case AV_PIX_FMT_YUV422P:
				memcpy(buffer,buf,baseFrameSzie*2);
				break;
			case AV_PIX_FMT_RGB24:
				memcpy(buffer,buf,baseFrameSzie*3);
				break;
			default:
				printf("Not supports this format \n");
				break;
			}
		tick +=tick_gap;
		nowTime=RTMP_GetTime();
		usleep((tick_gap-nowTime+lastTime)*1000);
		frameIndex++;
}

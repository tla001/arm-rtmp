/*
 * FrameBufferDisp.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#include "FrameBufferDisp.h"

FrameBufferDisp::FrameBufferDisp(char *devName) {
	// TODO Auto-generated constructor stub
	this->devName=devName;
	this->fd=-1;
	this->fb_buf=NULL;
}

FrameBufferDisp::~FrameBufferDisp() {
	// TODO Auto-generated destructor stub
}

int FrameBufferDisp::openDevice()
{
	/*设备的打开*/
	fd = open( devName, O_RDWR );
	if( fd<0 ){
		Common::ErrDisp(fd,"Can't open video device");
	}
	return 0;
}
int FrameBufferDisp::closeDevice()
{
	if(fb_buf!=NULL)
		munmap(fb_buf, screensize);
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
int FrameBufferDisp::initDevice()
{
	printf("\nFramebuffer device information: \n");
	int ret;
	// Get fixed screen information
	if ((ret=ioctl(fd, FBIOGET_FSCREENINFO, &finfo))<0) {
		Common::ErrDisp(ret,"Error reading fixed information.");
	}

	// Get variable screen information
	if ((ret=ioctl(fd, FBIOGET_VSCREENINFO, &vinfo))<0) {
		Common::ErrDisp(ret,"Error reading variable information.");
	}
	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	printf("screensize=%d\n",screensize);

	// Map the device to memory
	fb_buf = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
				   fd, 0);
	if ((int)fb_buf == -1) {
		Common::ErrDisp(ret,"Failed to map framebuffer device to memory.");
	}
	printf("The framebuffer device was mapped to memory successfully.\n");
	return 0;
}
void FrameBufferDisp::display(unsigned char *frame,int screen_w,int screen_h)
{
	//memcpy(fb_buf,frame,640* 480* 3 * sizeof(char));
	int x = 0, y = 0;
	long int location = 0;
	// Figure out where in memory to put the pixel
	for ( y = 0; y < screen_h; y++ )
		for ( x = 0; x < screen_w; x++ ) {
			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
					   (y+vinfo.yoffset) * finfo.line_length;
			if ( vinfo.bits_per_pixel == 32 ) {
				//rgb32 bgra
				*(fb_buf + location ) 		= *frame;frame++; // Some blue
				*(fb_buf + location + 1)	= *frame;frame++; // A little green
				*(fb_buf + location + 2) 	= *frame;frame++; //A lot of red//frame[480*y+x+2];
				*(fb_buf + location + 3) = 0; // No transparency
			}
			else { //assume 16bpp
				int b = *frame;frame++;
				int g = *frame;frame++;
				int r = *frame;frame++;
				unsigned short int t = r<<11 | g << 5 | b;
				*((unsigned short int*)(fb_buf + location)) = t;
			}
		}
}

/*
 * V4L2Capture.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#include "V4L2Capture.h"

V4L2Capture::V4L2Capture(char *devName) {
	// TODO Auto-generated constructor stub
	this->devName=devName;
	this->fd_cam=-1;
	this->buffers=NULL;
	this->n_buffers=0;
	this->frameIndex=-1;
}

V4L2Capture::~V4L2Capture() {
	// TODO Auto-generated destructor stub
}

int V4L2Capture::openDevice()
{
	/*�豸�Ĵ�*/
	printf("video dev : %s\n",devName);
	fd_cam = open( devName, O_RDWR );
	if( fd_cam<0 ){
		Common::ErrDisp(fd_cam,"Can't open video device");
	}
	return 0;
}
int V4L2Capture::closeDevice()
{
	if(fd_cam>0){
		int ret=0;
		if((ret=close(fd_cam))<0){
			Common::ErrDisp(ret,"Can't close video device");
		}
		return 0;
	}
	else{
		return -1;
	}
}
int V4L2Capture::initDevice()
{
	int ret;
	struct 	v4l2_capability cam_cap;		//��ʾ�豸��Ϣ
	struct  v4l2_cropcap 	cam_cropcap;	//��������ͷ�Ĳ�׽����
	struct 	v4l2_fmtdesc	cam_fmtdesc;	//��ѯ����֧�ֵĸ�ʽ��VIDIOC_ENUM_FMT
	struct  v4l2_crop		cam_crop;		//ͼ�������
	struct  v4l2_format 	cam_format;		//��������ͷ����Ƶ��ʽ��֡��ʽ��

	/* ʹ��IOCTL����VIDIOC_QUERYCAP����ȡ����ͷ�Ļ�����Ϣ*/
	ret = ioctl( fd_cam,VIDIOC_QUERYCAP,&cam_cap );
	if( ret<0 ) {
		Common::ErrDisp(ret,"Can't get device information: VIDIOCGCAP");
	}
	printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n",
			cam_cap.driver,cam_cap.card,cam_cap.bus_info,(cam_cap.version>>16)&0XFF,
			(cam_cap.version>>8)&0XFF,cam_cap.version&0XFF);

	/* ʹ��IOCTL����VIDIOC_ENUM_FMT����ȡ����ͷ����֧�ֵĸ�ʽ*/
	cam_fmtdesc.index=0;
	cam_fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	printf("Support format:\n");
	while(ioctl(fd_cam, VIDIOC_ENUM_FMT, &cam_fmtdesc) != -1)
	{
		printf("\t%d.%s\n",cam_fmtdesc.index+1,cam_fmtdesc.description);
		cam_fmtdesc.index++;
	}

	/* ʹ��IOCTL����VIDIOC_CROPCAP����ȡ����ͷ�Ĳ�׽����*/
	cam_cropcap.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(0 == ioctl(fd_cam, VIDIOC_CROPCAP, &cam_cropcap)){
		printf("Default rec:\n\tleft:%d\n\ttop:%d\n\twidth:%d\n\theight:%d\n",
				cam_cropcap.defrect.left,cam_cropcap.defrect.top,
				cam_cropcap.defrect.width,cam_cropcap.defrect.height);
		/* ʹ��IOCTL����VIDIOC_S_CROP����ȡ����ͷ�Ĵ���ȡ������*/
		cam_crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		cam_crop.c = cam_cropcap.defrect;//Ĭ��ȡ�����ڴ�С
		if(-1 == ioctl(fd_cam, VIDIOC_S_CROP, &cam_crop)){
			//printf("Can't set crop para\n");
		}
	}
	else{
		printf("Can't set cropcap para\n");
	}

	/* ʹ��IOCTL����VIDIOC_S_FMT����������ͷ֡��Ϣ*/
	cam_format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	cam_format.fmt.pix.width = 640;
	cam_format.fmt.pix.height = 480;
	cam_format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;//Ҫ������ͷ֧�ֵ����Ͷ�Ӧ
	cam_format.fmt.pix.field = V4L2_FIELD_INTERLACED;
	ret=ioctl(fd_cam, VIDIOC_S_FMT, &cam_format);
	if(ret<0){
		Common::ErrDisp(ret,"Can't set frame information");
	}
	/* ʹ��IOCTL����VIDIOC_G_FMT����ȡ����ͷ֡��Ϣ*/
	cam_format.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret=ioctl(fd_cam, VIDIOC_G_FMT, &cam_format);
	if(ret<0){
		Common::ErrDisp(ret,"Can't get frame information");
	}
	printf("Current data format information:\n\twidth:%d\n\theight:%d\n",
			cam_format.fmt.pix.width,cam_format.fmt.pix.height);
	ret=initBuffers();
	if(ret<0){
		Common::ErrDisp(ret,"Buffers init error");
		//exit(-1);
	}
	return 0;
}
int V4L2Capture::initBuffers()
{
	int ret;
	/* ʹ��IOCTL����VIDIOC_REQBUFS������֡����*/
	struct v4l2_requestbuffers req;
	CLEAR(req);
	req.count=4;
	req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	ret=ioctl(fd_cam, VIDIOC_REQBUFS, &req);
	if(ret<0){
		Common::ErrDisp(ret,"Request frame buffers failed");
	}
	if(req.count<2){
		Common::ErrDisp(ret,"Request frame buffers while insufficient buffer memory");
	}
	buffers = (struct cam_buffer*)calloc(req.count, sizeof(*buffers));
	if(!buffers){
		Common::ErrDisp(ret,"Out of memory");
	}
	for(n_buffers = 0; n_buffers < req.count; n_buffers++){
		struct v4l2_buffer buf;
		CLEAR(buf);
		// ��ѯ���Ϊn_buffers �Ļ��������õ�����ʼ�����ַ�ʹ�С
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		ret=ioctl(fd_cam, VIDIOC_QUERYBUF, &buf);
		if(ret<0 )
		{
			printf("VIDIOC_QUERYBUF %d failed\n",n_buffers);
			return -1;
		}
		buffers[n_buffers].length = buf.length;
		//printf("buf.length= %d\n",buf.length);
		// ӳ���ڴ�
		buffers[n_buffers].start =
		 mmap(NULL, // start anywhere
			  buf.length,
			  PROT_READ | PROT_WRITE,
			  MAP_SHARED,
			  fd_cam, buf.m.offset);
		if(MAP_FAILED == buffers[n_buffers].start)
		{
			printf("mmap buffer%d failed\n",n_buffers);
			return -1;
		}
	}
	return 0;
}
int V4L2Capture::startCapture()
{
	unsigned int i;
	for(i=0;i<n_buffers;i++){
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory =V4L2_MEMORY_MMAP;
		buf.index = i;
		if(-1 == ioctl(fd_cam, VIDIOC_QBUF, &buf))	{
			printf("VIDIOC_QBUF buffer%d failed\n",i);
			return -1;
		}
	}
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd_cam, VIDIOC_STREAMON, &type)){
		 printf("VIDIOC_STREAMON error");
		 return -1;
	}
	return 0;
}
int V4L2Capture::stopCapture()
{
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd_cam, VIDIOC_STREAMOFF, &type)){
		printf("VIDIOC_STREAMOFF error\n");
		return -1;
	}
	return 0;
}
int V4L2Capture::freeBuffers()
{
	unsigned int i;
	for(i = 0; i < n_buffers; ++i){
		if(-1 == munmap(buffers[i].start, buffers[i].length)){
			printf("munmap buffer%d failed\n",i);
			return -1;
		}
	}
	free(buffers);
	return 0;
}
int V4L2Capture::getFrame(void **frame_buf, size_t* len)
{
	struct v4l2_buffer queue_buf;
	CLEAR(queue_buf);
	queue_buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	queue_buf.memory = V4L2_MEMORY_MMAP;
	if(-1 == ioctl(fd_cam, VIDIOC_DQBUF, &queue_buf)){
		printf("VIDIOC_DQBUF error\n");
		return -1;
	}
	//printf("queue_buf.index=%d\n",queue_buf.index);
	//pthread_rwlock_wrlock(&rwlock);
	*frame_buf = buffers[queue_buf.index].start;
	*len = buffers[queue_buf.index].length;
	frameIndex = queue_buf.index;
	//pthread_rwlock_unlock(&rwlock);
	return 0;
}
int V4L2Capture::backFrame()
{
	if(frameIndex != -1){
		struct v4l2_buffer queue_buf;
		CLEAR(queue_buf);
		queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		queue_buf.memory = V4L2_MEMORY_MMAP;
		queue_buf.index = frameIndex;
		if(-1 == ioctl(fd_cam, VIDIOC_QBUF, &queue_buf)){
			printf("VIDIOC_QBUF error\n");
			return -1;
		}
		return 0;
	}
	return -1;
}

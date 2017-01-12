
#pragma once

#include "librtmp_send264.h"
#include "sps_decode.h"

//#ifdef  _BASEFUNC_EXPORT_
//#define  BASE_API  extern "C" __declspec(dllexport)
//#else
//#define  BASE_API   __declspec(dllimport)
//#endif
//
//#define __STDC_CONSTANT_MACROS

extern "C" {
#include "x264.h"
#include "x264_config.h"
//#include "mp4v2\mp4v2.h"
#include "libswscale/swscale.h"
//#include "libavutil\opt.h"
#include "libavutil/imgutils.h"
}

//CRITICAL_SECTION m_Cs;// �ٽ����ṹ����

typedef struct
{
	// rtmp object
	char*				szUrl;
	RTMP*				rtmp;
	RTMPPacket			packet;
	// faac encoder

	unsigned long       nSampleRate;
	unsigned long       nChannels;
	unsigned long       nTimeStamp;
	unsigned long       nTimeDelta;
	char*				szPcmAudio;
	unsigned long       nPcmAudioSize;
	unsigned long       nPcmAudioLen;
	char*				szAacAudio;
	unsigned long       nAacAudioSize;
}RTMPMOD_SPublishObj;

typedef unsigned long long  QWORD, ULONG64, UINT64, ULONGLONG;

class RtmpH264
{

public:
	RtmpH264(void);


	~RtmpH264()
	{
	}

	int CreatePublish(char* url, int outChunkSize, int isOpenPrintLog, int logType);
	void DeletePublish();

	int InitVideoParams(unsigned long width, unsigned long height, unsigned long fps, unsigned long bitrate, enum AVPixelFormat src_pix_fmt,bool bConstantsBitrate);
	int SendScreenCapture(BYTE * frame, unsigned long Height, unsigned long timespan);

	//int WriteVideoParams(unsigned long width, unsigned long height, unsigned long fps, unsigned long bitrate);
	//int WriteScreenCapture(BYTE * frame, unsigned long Stride, unsigned long Height, unsigned long timespan);

	void FreeEncodeParams(); //�ͷ�ռ���ڴ���Դ

public:
	DWORD m_startTime;
	RTMPMOD_SPublishObj rtmp_PublishObj;

	struct SwsContext * m_SwsContext;

	int m_width;	//���
	int m_height;	// �߶�
	int	m_frameRate;	//֡��fps
	int m_bitRate;	//������
	int m_baseFrameSize;

	int m_audioChannel;	//������
	int m_audioSample;	//��Ƶ������

	int m_type;//0 rtmp����ģʽ�� 1 mp4¼��ģʽ
	bool m_isCreatePublish;

	enum AVPixelFormat m_srcPicFmt;

	uint8_t *src_data[4];
	int src_linesize[4];

};

/*����������
*<param name="url">��������ַ</param>
*<param name="outChunkSize">���Ͱ���С</param>
*/
long RTMP_CreatePublish(char* url, unsigned long outChunkSize, int isOpenPrintLog, int logType);

//ɾ��������
 void RTMP_DeletePublish();


/*��ʼ���������
*<param name="width">��Ƶ���</param>
*<param name="height">��Ƶ�߶�</param>
*<param name="fps">֡��</param>
*<param name="bitrate">������</param>
*<param name="bConstantsBitrate"> �Ƿ�㶨���� </param>
*/
long RTMP_InitVideoParams(unsigned long width, unsigned long height, unsigned long fps, unsigned long bitrate,enum AVPixelFormat src_pix_fmt,bool bConstantsBitrate = false);


/*��ʼ���������
*<param name="frame">ͼƬ��ַ</param>
*<param name="Stride">ͼƬ����</param>
*<param name="Height">ͼƬ�и�</param>
*<param name="timespan">ʱ���</param>
*/
long RTMP_SendScreenCapture(char * frame,  unsigned long Height, unsigned long timespan);


/*��ʼ���������
*<param name="szBuf">��Ƶ���ݵ�ַ</param>
*<param name="nBufLen">��Ƶ���ݳ��� ��λ�ֽ�</param>
*<param name="nSampleRate">������</param>
*<param name="nChannels">������</param>
*<param name="timespan">ʱ���</param>
*/
long RTMP_SendAudioFrame(char* szBuf, unsigned long nBufLen, unsigned long nSampleRate, unsigned long nChannels, unsigned long timespan);


///*����MP4�ļ�
//*<param name="fileName">�ļ���</param>
//*<param name="audioSample">������</param>
//*<param name="audioChannel">������</param>
//*/
//BASE_API long RTMP_CreateMp4File(char* fileName, int audioChannel, int audioSample);
//BASE_API void RTMP_CloseMp4File();
//
///*��ʼ���������
//*<param name="width">��Ƶ���</param>
//*<param name="height">��Ƶ�߶�</param>
//*<param name="fps">֡��</param>
//*<param name="bitrate">������</param>
//*/
//BASE_API long RTMP_WriteVideoParams(unsigned long width, unsigned long height, unsigned long fps, unsigned long bitrate);
//
//
///*����ͼ�����ݲ�д���ļ�
//*<param name="szBuf">��Ƶ���ݵ�ַ</param>
//*<param name="nBufLen">��Ƶ���ݳ��� ��λ�ֽ�</param>
//*<param name="nSampleRate">������</param>
//*<param name="nChannels">������</param>
//*<param name="timespan">ʱ���</param>
//*/
//BASE_API long RTMP_WriteScreenCapture(char * frame, unsigned long Stride, unsigned long Height, unsigned long timespan);
//
//
///*������Ƶ���ݲ�д���ļ�
//*<param name="szBuf">��Ƶ���ݵ�ַ</param>
//*<param name="nBufLen">��Ƶ���ݳ��� ��λ�ֽ�</param>
//*<param name="nSampleRate">������</param>
//*<param name="nChannels">������</param>
//*<param name="timespan">ʱ���</param>
//*/
//BASE_API long RTMP_WriteAudioFrame(char* szBuf, unsigned long nBufLen, unsigned long nSampleRate, unsigned long nChannels, unsigned long timespan);

#pragma once


#include "librtmp/rtmp.h"
#include "librtmp/rtmp_sys.h"
#include "librtmp/amf.h"


/**
 * Simplest Librtmp Send 264
 *
 * �����裬����
 * leixiaohua1020@126.com
 * zhanghuicuc@gmail.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ���������ڽ��ڴ��е�H.264����������RTMP��ý���������
 *
 */

/**
* _NaluUnit
* �ڲ��ṹ�塣�ýṹ����Ҫ���ڴ洢�ʹ���Nal��Ԫ�����͡���С������
*/
typedef struct _NaluUnit
{
	int type;
	int size;
	unsigned char *data;
}NaluUnit;

/**
* _RTMPMetadata
* �ڲ��ṹ�塣�ýṹ����Ҫ���ڴ洢�ʹ���Ԫ������Ϣ
*/
typedef struct _RTMPMetadata
{
	// video, must be h264 type
	unsigned int    nWidth;
	unsigned int    nHeight;
	unsigned int    nFrameRate;
	unsigned int    nSpsLen;
	unsigned char   *Sps;
	unsigned int    nPpsLen;
	unsigned char   *Pps;
} RTMPMetadata, *LPRTMPMetadata;

enum
{
	VIDEO_CODECID_H264 = 7,
};



/**
 * ��ʼ�������ӵ�������
 *
 * @param url �������϶�Ӧwebapp�ĵ�ַ
 * @isOpenPrintLog �Ƿ��ӡ��־
 * @logType ��־����
 * @�ɹ��򷵻�1 , ʧ���򷵻�0
 */
int RTMP264_Connect(const char* url, RTMP** ppRtmp, int isOpenPrintLog, int logType);


/**
 * �Ͽ����ӣ��ͷ���ص���Դ��
 *
 */
void RTMP264_Close();

void InitSpsPps(unsigned char *pps, int pps_len, unsigned char * sps, int sps_len, int width, int height, int fps);


/**
* ����H264����֡
*
* @param data �洢����֡����
* @param size ����֡�Ĵ�С
* @param bIsKeyFrame ��¼��֡�Ƿ�Ϊ�ؼ�֡
* @param nTimeStamp ��ǰ֡��ʱ���
*
* @�ɹ��򷵻� 1 , ʧ���򷵻�0
*/
int SendH264Packet(unsigned char *data, unsigned int size, int bIsKeyFrame, unsigned int nTimeStamp);


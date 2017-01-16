#include "time.h"
#include "stdio.h"
#include <iostream>  
#include <windows.h>
#include "libyuv/include/libyuv.h"

using namespace std;

//原始视频序列及处理后视频序列的名称
//#define READ_FILE  "NV21.yuv"
#define READ_FILE  "720p.yuv"
#define SAVE_FILE   "result.yuv"

// Supported rotation
enum VideoRotationMode {
	kRotateNone = 0,
	kRotate90 = 90,
	kRotate180 = 180,
	kRotate270 = 270,
};

//videoprocessmode
enum VideoProcessMode {
	NV12ToI420 = 0,
	NV12ToI420Rotate = 1,
	NV12ToI420Mirro = 2,
	NV12ToI420RotateMirro = 3,
	NV12ToI420MirroRotate = 4,
	I420Rotate = 5,
	I420Mirror = 6,
	I420RotateMirror = 7,
	I420MirrorRotate = 8,
};

//video stream format
enum VideoStreamFormat {
	YUV420P = 0,
	YUVNV12 = 1,
};

libyuv::RotationMode ConvertRotationMode(VideoRotationMode rotation) {
	switch (rotation) {
	case kRotateNone:
		return libyuv::kRotate0;
	case kRotate90:
		return libyuv::kRotate90;
	case kRotate180:
		return libyuv::kRotate180;
	case kRotate270:
		return libyuv::kRotate270;
	default:
		return libyuv::kRotate0;
	}
}

// 3 processing mode of video stream only for NV12 format :
// 1、transformation  2、rotate  3、mirror
// if above 3 or 2 process modes are required meanwhile,can achieve target video stream
// only through a data copy,which can reduce CPU consumption 
int VideoStreamProcess(unsigned char * Src_data, unsigned char * Dst_data,
					   int src_width, int src_height,
					   VideoRotationMode rotatemodel, VideoProcessMode videoprocessmodel,
					   VideoStreamFormat srcvideoformat)
{
	// NV12 video size
	int NV12_Size = src_width * src_height * 3 / 2;
	int NV12_Y_Size = src_width * src_height;

	// YUV420 video size
	int I420_Size = src_width * src_height * 3 / 2;
	int I420_Y_Size = src_width * src_height;
	int I420_U_Size = (src_width >> 1)*(src_height >> 1);
	int I420_V_Size = I420_U_Size;

	switch (videoprocessmodel){
	// only video I420 rorate 
	case I420Rotate:
	{
		// src: buffer address of  Y channel、U channel and V channel
		unsigned char * Y_data_Src = Src_data;
		unsigned char * U_data_Src = Src_data + I420_Y_Size;
		unsigned char * V_data_Src = Src_data + I420_Y_Size + I420_U_Size;
		int src_stride_y = src_width;
		int src_stride_u = src_width >> 1;
		int src_stride_v = src_stride_u;

		//dst: buffer address of Y channel、U channel and V channel
		unsigned char * Y_data_Dst = Dst_data;
		unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
		unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
		int Dst_Stride_Y;
		int Dst_Stride_U;
		int Dst_Stride_V;
		if (rotatemodel == libyuv::kRotate90 || rotatemodel == libyuv::kRotate270)
		{
			Dst_Stride_Y = src_height;
			Dst_Stride_U = src_height >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}
		else
		{
			Dst_Stride_Y = src_width;
			Dst_Stride_U = src_width >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}

		libyuv::I420Rotate(Y_data_Src, src_stride_y,
						   U_data_Src, src_stride_u,
						   V_data_Src, src_stride_v,
						   Y_data_Dst, Dst_Stride_Y,
						   U_data_Dst, Dst_Stride_U,
						   V_data_Dst, Dst_Stride_V,
						   src_width, src_height,
						   ConvertRotationMode(rotatemodel));

		break;
	}
	// only video I420 mirror
	case I420Mirror:
	{
		// src: buffer address of  Y channel、U channel and V channel
		unsigned char * Y_data_Src = Src_data;
		unsigned char * U_data_Src = Src_data + I420_Y_Size;
		unsigned char * V_data_Src = Src_data + I420_Y_Size + I420_U_Size;
		int src_stride_y = src_width;
		int src_stride_u = src_width >> 1;
		int src_stride_v = src_stride_u;

		//dst: buffer address of Y channel、U channel and V channel
		unsigned char * Y_data_Dst = Dst_data;
		unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
		unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
		int Dst_Stride_Y = src_width;
		int Dst_Stride_U = src_width >> 1;
		int Dst_Stride_V = Dst_Stride_U;

		libyuv::I420Mirror(Y_data_Src, src_stride_y,
						   U_data_Src, src_stride_u,
						   V_data_Src, src_stride_v,
						   Y_data_Dst, Dst_Stride_Y,
						   U_data_Dst, Dst_Stride_U,
			               V_data_Dst, Dst_Stride_V,
						   src_width, src_height);
		break;
	}
	// video I420 rorate and  mirror
	case I420RotateMirror:
	{
		// src: buffer address of  Y channel、U channel and V channel
		unsigned char * Y_data_Src = Src_data;
		unsigned char * U_data_Src = Src_data + I420_Y_Size;
		unsigned char * V_data_Src = Src_data + I420_Y_Size + I420_U_Size;
		int src_stride_y = src_width;
		int src_stride_u = src_width >> 1;
		int src_stride_v = src_stride_u;

		//dst: buffer address of Y channel、U channel and V channel
		unsigned char * Y_data_Dst = Dst_data;
		unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
		unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
		int Dst_Stride_Y;
		int Dst_Stride_U;
		int Dst_Stride_V;
		if (rotatemodel == libyuv::kRotate90 || rotatemodel == libyuv::kRotate270)
		{
			Dst_Stride_Y = src_height;
			Dst_Stride_U = src_height >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}
		else
		{
			Dst_Stride_Y = src_width;
			Dst_Stride_U = src_width >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}

		libyuv::I420RotateMirror(Y_data_Src, src_stride_y,
								 U_data_Src, src_stride_u,
								 V_data_Src, src_stride_v,
								 Y_data_Dst, Dst_Stride_Y,
								 U_data_Dst, Dst_Stride_U,
								 V_data_Dst, Dst_Stride_V,
								 src_width, src_height,
								 ConvertRotationMode(rotatemodel));

		break;
	}
	// video I420 mirror and  rotate
	case I420MirrorRotate:
	{
		// src: buffer address of  Y channel、U channel and V channel
		unsigned char * Y_data_Src = Src_data;
		unsigned char * U_data_Src = Src_data + I420_Y_Size;
		unsigned char * V_data_Src = Src_data + I420_Y_Size + I420_U_Size;
		int src_stride_y = src_width;
		int src_stride_u = src_width >> 1;
		int src_stride_v = src_stride_u;

		//dst: buffer address of Y channel、U channel and V channel
		unsigned char * Y_data_Dst = Dst_data;
		unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
		unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
		int Dst_Stride_Y;
		int Dst_Stride_U;
		int Dst_Stride_V;
		if (rotatemodel == libyuv::kRotate90 || rotatemodel == libyuv::kRotate270)
		{
			Dst_Stride_Y = src_height;
			Dst_Stride_U = src_height >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}
		else
		{
			Dst_Stride_Y = src_width;
			Dst_Stride_U = src_width >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}

		libyuv::I420MirrorRotate(Y_data_Src, src_stride_y,
								 U_data_Src, src_stride_u,
								 V_data_Src, src_stride_v,
								 Y_data_Dst, Dst_Stride_Y,
								 U_data_Dst, Dst_Stride_U,
								 V_data_Dst, Dst_Stride_V,
								 src_width, src_height,
								 ConvertRotationMode(rotatemodel));
		break;
	}
	// only video format transformation 
	case NV12ToI420:
	{
		// src: buffer address of Y channel and UV channel
		unsigned char * Y_data_Src = Src_data;
		unsigned char * UV_data_Src = Src_data + NV12_Y_Size;
		int src_stride_y = src_width;
		int src_stride_uv = src_width;

		//dst: buffer address of Y channel、U channel and V channel
		unsigned char * Y_data_Dst = Dst_data;
		unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
		unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
		int Dst_Stride_Y = src_width;
		int Dst_Stride_U = src_width >> 1;
		int Dst_Stride_V = Dst_Stride_U;

		int ret = libyuv::NV12ToI420(Y_data_Src, src_stride_y,
									 UV_data_Src, src_stride_uv,
									 Y_data_Dst, Dst_Stride_Y,
									 U_data_Dst, Dst_Stride_U,
									 V_data_Dst, Dst_Stride_V,
									 src_width, src_height);
		break;
	}
	// video format transformation and rotate
	case NV12ToI420Rotate:
	{
		// src: buffer address of Y channel and UV channel
		unsigned char * Y_data_Src = Src_data;
		unsigned char * UV_data_Src = Src_data + NV12_Y_Size;
		int src_stride_y = src_width;
		int src_stride_uv = src_width;

		//dst: buffer address of Y channel、U channel and V channel
		unsigned char * Y_data_Dst = Dst_data;
		unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
		unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
		int Dst_Stride_Y;
		int Dst_Stride_U;
		int Dst_Stride_V;

		if (rotatemodel == libyuv::kRotate90 || rotatemodel == libyuv::kRotate270)
		{
			Dst_Stride_Y = src_height;
			Dst_Stride_U = src_height >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}
		else
		{
			Dst_Stride_Y = src_width;
			Dst_Stride_U = src_width >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}
		int ret = libyuv::NV12ToI420Rotate(Y_data_Src, src_stride_y,
										   UV_data_Src, src_stride_uv,
										   Y_data_Dst, Dst_Stride_Y,
										   U_data_Dst, Dst_Stride_U,
										   V_data_Dst, Dst_Stride_V,
										   src_width, src_height,
										   ConvertRotationMode(rotatemodel));
		break;
	}
	// video format transformation and mirror
	case NV12ToI420Mirro:
	{
		// src: buffer address of Y channel and UV channel
		unsigned char * Y_data_Src = Src_data;
		unsigned char * UV_data_Src = Src_data + NV12_Y_Size;
		int src_stride_y = src_width;
		int src_stride_uv = src_width;

		//dst: buffer address of Y channel、U channel and V channel
		unsigned char * Y_data_Dst = Dst_data;
		unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
		unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
		int Dst_Stride_Y = src_width;
		int Dst_Stride_U = src_width >> 1;
		int Dst_Stride_V = Dst_Stride_U;

		int ret = libyuv::NV12ToI420Mirro(Y_data_Src, src_stride_y,
										  UV_data_Src, src_stride_uv,
										  Y_data_Dst, Dst_Stride_Y,
										  U_data_Dst, Dst_Stride_U,
										  V_data_Dst, Dst_Stride_V,
										  src_width, src_height);
		break;
	}
	// video format transformation 、rotate and mirror
	case NV12ToI420RotateMirro:
	{
		// src: buffer address of Y channel and UV channel
		unsigned char * Y_data_Src = Src_data;
		unsigned char * UV_data_Src = Src_data + NV12_Y_Size;
		int src_stride_y = src_width;
		int src_stride_uv = src_width;

		//dst: buffer address of Y channel、U channel and V channel
		unsigned char * Y_data_Dst = Dst_data;
		unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
		unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
		int Dst_Stride_Y;
		int Dst_Stride_U;
		int Dst_Stride_V;

		if (rotatemodel == libyuv::kRotate90 || rotatemodel == libyuv::kRotate270)
		{
			Dst_Stride_Y = src_height;
			Dst_Stride_U = src_height >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}
		else
		{
			Dst_Stride_Y = src_width;
			Dst_Stride_U = src_width >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}

		int ret = NV12ToI420RotateMirror(Y_data_Src, src_stride_y,
										 UV_data_Src, src_stride_uv,
										 Y_data_Dst, Dst_Stride_Y,
										 U_data_Dst, Dst_Stride_U,
										 V_data_Dst, Dst_Stride_V,
										 src_width, src_height,
										 ConvertRotationMode(rotatemodel));
		break;
	}
	// video format transformation 、mirror and rotate
	case NV12ToI420MirroRotate:
	{
		// src: buffer address of Y channel and UV channel
		unsigned char * Y_data_Src = Src_data;
		unsigned char * UV_data_Src = Src_data + NV12_Y_Size;
		int src_stride_y = src_width;
		int src_stride_uv = src_width;

		//dst: buffer address of Y channel、U channel and V channel
		unsigned char * Y_data_Dst = Dst_data;
		unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
		unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
		int Dst_Stride_Y;
		int Dst_Stride_U;
		int Dst_Stride_V;

		if (rotatemodel == libyuv::kRotate90 || rotatemodel == libyuv::kRotate270)
		{
			Dst_Stride_Y = src_height;
			Dst_Stride_U = src_height >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}
		else
		{
			Dst_Stride_Y = src_width;
			Dst_Stride_U = src_width >> 1;
			Dst_Stride_V = Dst_Stride_U;
		}
		int ret = NV12ToI420MirrorRotate(Y_data_Src, src_stride_y,
										 UV_data_Src, src_stride_uv,
										 Y_data_Dst, Dst_Stride_Y,
										 U_data_Dst, Dst_Stride_U,
										 V_data_Dst, Dst_Stride_V,
										 src_width, src_height,
										 ConvertRotationMode(rotatemodel));
		break;
	}
	// else
	default:
		return -1;
	}
	return 0;
}

int main()
{
	FILE *fpr, *fpw;

	//src: NV12/yuv420p
	int width = 1280;
	int height = 720;
	//VideoStreamFormat _srcvideoformat = YUVNV12;
	VideoStreamFormat _srcvideoformat = YUV420P;
	int src_video_Size = width * height * 3 / 2;
	unsigned char * Data_Src = (unsigned char *)malloc((src_video_Size)*sizeof(unsigned char));           //NV12

	//dst:YUV420
	int I420_Size = width * height * 3 / 2;
	unsigned char * YUV420_Data_Dst = (unsigned char *)malloc((I420_Size)*sizeof(unsigned char));         //I420

	VideoProcessMode _videoprocessmode;
	VideoRotationMode _rotatemodel = kRotate90;
	bool MirrorEnable = true;
	bool RotateEnable = true;

	if (MirrorEnable == false && RotateEnable == false && _srcvideoformat == YUVNV12)
	{
		_videoprocessmode = NV12ToI420;
	}
	if (MirrorEnable == false && RotateEnable == true && _srcvideoformat == YUVNV12)
	{
		_videoprocessmode = NV12ToI420Rotate;
	}
	if (MirrorEnable == true && RotateEnable == false && _srcvideoformat == YUVNV12)
	{
		_videoprocessmode = NV12ToI420Mirro;
	}
	if (MirrorEnable == true && RotateEnable == true && _srcvideoformat == YUVNV12)
	{
		_videoprocessmode = NV12ToI420MirroRotate;
	}
	if (MirrorEnable == true && RotateEnable == true && _srcvideoformat == YUV420P)
	{
		_videoprocessmode = I420RotateMirror;
	}
	if (MirrorEnable == true && RotateEnable == false && _srcvideoformat == YUV420P)
	{
		_videoprocessmode = I420Mirror;
	}
	if (MirrorEnable == false && RotateEnable == true && _srcvideoformat == YUV420P)
	{
		_videoprocessmode = I420Rotate;
	}

	// 读取视频序列
	fpr = fopen(READ_FILE, "rb");
	fread(Data_Src, 1, src_video_Size, fpr);

	int num_process = 1000;
	double start = GetTickCount();
	for (int num = 0; num<num_process; num++)
	{
		VideoStreamProcess(Data_Src, YUV420_Data_Dst,
						   width, height,
						   _rotatemodel, _videoprocessmode,
						   _srcvideoformat);
	}
	double end = GetTickCount();
	printf("%f m_seconds\n", end - start); //以毫秒为单位显示
	system("pause");
	fclose(fpr);

	fpw = fopen(SAVE_FILE, "wb");
	fwrite(YUV420_Data_Dst, 1, I420_Size, fpw);
	fclose(fpw);
}
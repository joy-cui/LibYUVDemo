#include "libYuvUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <libyuv.h>
#include "libyuv/rotate.h"
#include <android/log.h>
#include <string.h>
#include <jni.h>
#include "time.h"

#define TAG "libYuvUtil" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

using namespace std;
jclass Callback_class = NULL;
jclass yuv_class=NULL;
jclass class_ArrayList=NULL;
JavaVM *gs_jvm = 0;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *ajvm, void *reserved) {
	gs_jvm = ajvm;
	JNIEnv* env = NULL;
	if ((gs_jvm)->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGD("libYuvUtil:JNI_OnLoad..not..JNI_VERSION_1_4...signal..");
		return -1;
	}
	LOGD("signal......libYuvUtil.");
    		Callback_class = (jclass) env->NewGlobalRef(
    				env->FindClass("org/suirui/util/libyuv/jni/JniCallBack"));

    		yuv_class = (jclass) env->NewGlobalRef(
    				env->FindClass("org/suirui/util/libyuv/jni/entry/YUV"));

    		class_ArrayList = (jclass) env->NewGlobalRef(
                    env->FindClass("java/util/ArrayList"));
	LOGD("LOGD:JNI_OnLoad....signal");
	return JNI_VERSION_1_4;;
}
  void unLoad(JNIEnv *env){
  LOGD("LOGD:unLoad....signal");
    env->DeleteGlobalRef(Callback_class);
	env->DeleteGlobalRef(yuv_class);
	env->DeleteGlobalRef(class_ArrayList);
	}
class libYuvUtil {
public:
	libYuvUtil(JNIEnv *env) {


	}
	~libYuvUtil() {
	}



};

extern "C" {

JNIEXPORT jint JNICALL Java_org_suirui_util_libyuv_jni_libYuvJNI_unLoadYuv(
		JNIEnv *env, jclass jcls) {
   unLoad(env);
	return 0;
}


int VideoStreamProcess(unsigned char *Src_data, unsigned char *Dst_data,
                       int src_width, int src_height,
                       bool EnableRotate, bool EnableMirror,
                       unsigned char *Dst_data_mirror, unsigned char *Dst_data_rotate,
                       int rotatemodel) {
    //src:NV12 video size
    int NV12_Size = src_width * src_height * 3 / 2;
    int NV12_Y_Size = src_width * src_height;

    //dst:YUV420 video size
    int I420_Size = src_width * src_height * 3 / 2;
    int I420_Y_Size = src_width * src_height;
    int I420_U_Size = (src_width >> 1) * (src_height >> 1);
    int I420_V_Size = I420_U_Size;

    // video format transformation process
    unsigned char *Y_data_Src = Src_data;
    unsigned char *UV_data_Src = Src_data + NV12_Y_Size;
    int src_stride_y = src_width;
    int src_stride_uv = src_width;

    unsigned char *Y_data_Dst = Dst_data;
    unsigned char *U_data_Dst = Dst_data + I420_Y_Size;
    unsigned char *V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;

    int Dst_Stride_Y = src_width;
    int Dst_Stride_U = src_width >> 1;
    int Dst_Stride_V = Dst_Stride_U;
//NV12ToI420
    libyuv::NV21ToI420(Y_data_Src, src_stride_y,
                       UV_data_Src, src_stride_uv,
                       Y_data_Dst, Dst_Stride_Y,
                       U_data_Dst, Dst_Stride_U,
                       V_data_Dst, Dst_Stride_V,
                       src_width, src_height);




    // video mirror process
    unsigned char *Y_data_Dst_mirror = Dst_data_mirror;
    unsigned char *U_data_Dst_mirror = Dst_data_mirror + I420_Y_Size;
    unsigned char *V_data_Dst_mirror = Dst_data_mirror + I420_Y_Size + I420_U_Size;
    int Dst_Stride_Y_mirror = src_width;
    int Dst_Stride_U_mirror = src_width >> 1;
    int Dst_Stride_V_mirror = Dst_Stride_U_mirror;

    if (EnableMirror) {
        libyuv::I420Mirror(Y_data_Dst, Dst_Stride_Y,
                           U_data_Dst, Dst_Stride_U,
                           V_data_Dst, Dst_Stride_V,
                           Y_data_Dst_mirror, Dst_Stride_Y_mirror,
                           U_data_Dst_mirror, Dst_Stride_U_mirror,
                           V_data_Dst_mirror, Dst_Stride_V_mirror,
                           src_width, src_height);
    }

    //video rotate process
    if (EnableRotate) {
        int Dst_Stride_Y_rotate;
        int Dst_Stride_U_rotate;
        int Dst_Stride_V_rotate;
        unsigned char *Y_data_Dst_rotate = Dst_data_rotate;
        unsigned char *U_data_Dst_rotate = Dst_data_rotate + I420_Y_Size;
        unsigned char *V_data_Dst_rotate = Dst_data_rotate + I420_Y_Size + I420_U_Size;

        if (rotatemodel == libyuv::kRotate90 || rotatemodel == libyuv::kRotate270) {
            Dst_Stride_Y_rotate = src_height;
            Dst_Stride_U_rotate = src_height >> 1;
            Dst_Stride_V_rotate = Dst_Stride_U_rotate;
        }
        else {
            Dst_Stride_Y_rotate = src_width;
            Dst_Stride_U_rotate = src_width >> 1;
            Dst_Stride_V_rotate = Dst_Stride_U_rotate;
        }

        if (EnableMirror) {
            libyuv::I420Rotate(Y_data_Dst_mirror, Dst_Stride_Y_mirror,
                               U_data_Dst_mirror, Dst_Stride_U_mirror,
                               V_data_Dst_mirror, Dst_Stride_V_mirror,
                               Y_data_Dst_rotate, Dst_Stride_Y_rotate,
                               U_data_Dst_rotate, Dst_Stride_U_rotate,
                               V_data_Dst_rotate, Dst_Stride_V_rotate,
                               src_width, src_height,
                               (libyuv::RotationMode) rotatemodel);
        }
        else {
            libyuv::I420Rotate(Y_data_Dst, Dst_Stride_Y,
                               U_data_Dst, Dst_Stride_U,
                               V_data_Dst, Dst_Stride_V,
                               Y_data_Dst_rotate, Dst_Stride_Y_rotate,
                               U_data_Dst_rotate, Dst_Stride_U_rotate,
                               V_data_Dst_rotate, Dst_Stride_V_rotate,
                               src_width, src_height,
                               (libyuv::RotationMode) rotatemodel);
        }
    }


/*
    if(EnableRotate){
    onLibYUVCallBack_JNI(env,Dst_data_rotate, src_width,  src_height,I420_Size);
    }else if(EnableMirror){
    onLibYUVCallBack_JNI(env,Dst_data_mirror,src_width,  src_height,I420_Size);
    }else{
    onLibYUVCallBack_JNI(env,Dst_data,src_width,  src_height,I420_Size);
    }
*/


    return 0;
}


//jbyteArray to unsigned char *
unsigned char *byteArrayToChar(JNIEnv *env, jbyteArray srcData) {
    jbyte *bBuffer = env->GetByteArrayElements(srcData, 0);
    env->ReleaseByteArrayElements(srcData, bBuffer, 0);
    return (unsigned char *) bBuffer;
}

/*
unsigned char* arrayToChar(JNIEnv *env, jbyteArray array) {
	int len = env->GetArrayLength(array);
	unsigned char* buf = new unsigned char[len];
	env->GetByteArrayRegion(array, 0, len, reinterpret_cast<jbyte*>(buf));

	return buf;
}
*/

// unsigned char * to jbyteArray


jbyteArray getByteToArray(JNIEnv *env, unsigned char* ybuf, int len) {
	jbyteArray array = env->NewByteArray(len);
	env->SetByteArrayRegion(array, 0, len, reinterpret_cast<jbyte*>(ybuf));
	return array;
}
  static  unsigned char *NV21_Data_Src ;
  static unsigned char *YUV420_Data_Dst;
  static unsigned char *YUV420_Data_Dst_rotate;
  static  unsigned char *YUV420_Data_Dst_mirror;

JNIEXPORT jbyteArray
JNICALL Java_org_suirui_util_libyuv_jni_libYuvJNI_nv21To420(JNIEnv *env, jclass obj, jbyteArray srcData, jint width, jint height,int rotate,jboolean isMirror,jboolean isRotate){



 //dst:YUV420
int I420_Size = width * height * 3 / 2;

     if(YUV420_Data_Dst){
        free(YUV420_Data_Dst);
        YUV420_Data_Dst=NULL;
     }
      if(YUV420_Data_Dst_rotate){
             free(YUV420_Data_Dst_rotate);
             YUV420_Data_Dst_rotate=NULL;
      }
        if(YUV420_Data_Dst_mirror){
          free(YUV420_Data_Dst_mirror);
          YUV420_Data_Dst_mirror=NULL;
         }
    NV21_Data_Src = byteArrayToChar(env, srcData);
    YUV420_Data_Dst = (unsigned char *) malloc(
            (I420_Size) * sizeof(unsigned char));         //I420

   YUV420_Data_Dst_rotate = (unsigned char *) malloc(
            (I420_Size) * sizeof(unsigned char));  //I420
    YUV420_Data_Dst_mirror = (unsigned char *) malloc(
            (I420_Size) * sizeof(unsigned char));  //I420

     VideoStreamProcess(NV21_Data_Src, YUV420_Data_Dst,
                           width, height,
                           isRotate, isMirror,
                           YUV420_Data_Dst_mirror, YUV420_Data_Dst_rotate,
                           (libyuv::RotationMode) rotate);



    if(isRotate){
      return getByteToArray(env,YUV420_Data_Dst_rotate,I420_Size);
    }else if(isMirror){
     return getByteToArray(env,YUV420_Data_Dst_mirror,I420_Size);
    }else{
    return getByteToArray(env,YUV420_Data_Dst,I420_Size);
    }


    return 0;
}




// 420转yuv分量
JNIEXPORT jobject
JNICALL Java_org_suirui_util_libyuv_jni_libYuvJNI_yuv420ToYUV(JNIEnv *env, jclass obj, jbyteArray srcData, jint width, jint height){
  unsigned char *data = byteArrayToChar(env, srcData);
      int size = width * height * 3 / 2;
      int ySize = width * height;
      int uSize = (width >> 1) * (height >> 1);
      unsigned char *Y = data;
      unsigned char *U = data + ySize;
      unsigned char *V = data + ySize + uSize;
       jbyteArray  yByte=  getByteToArray(env,Y,ySize);
       jbyteArray  uByte=  getByteToArray(env,U,uSize);
       jbyteArray  vByte=  getByteToArray(env,V,uSize);
      jmethodID  yuv_method = env->GetMethodID(yuv_class, "<init>", "([B[B[B)V");
      jobject   yuv_obj = env->NewObject(yuv_class,yuv_method,yByte,uByte,vByte);
    return yuv_obj ;

}
/******************************************/
static unsigned char *Dst_data;

//NV21转I420并旋转
JNIEXPORT jbyteArray
JNICALL Java_org_suirui_util_libyuv_jni_libYuvJNI_NV21ToI420Rotate(JNIEnv *env, jclass obj, jbyteArray srcData,jint src_width, jint src_height,jint rotate){
  if(Dst_data){
            free(Dst_data);
                     Dst_data=NULL;
         }
    unsigned char *Src_data = byteArrayToChar(env, srcData);



	// NV12 video size
	int NV12_Size = src_width * src_height * 3 / 2;
	int NV12_Y_Size = src_width * src_height;

	// YUV420 video size
	int I420_Size = src_width * src_height * 3 / 2;
	int I420_Y_Size = src_width * src_height;
	int I420_U_Size = (src_width >> 1)*(src_height >> 1);
	int I420_V_Size = I420_U_Size;


      	Dst_data = (unsigned char *) malloc(
                    (I420_Size) * sizeof(unsigned char));  //I420

    unsigned char * Y_data_Src = Src_data;
  	unsigned char * UV_data_Src = Src_data + NV12_Y_Size;
  	int src_stride_y = src_width;
  	int src_stride_uv = src_width;

  		//dst: buffer address of Y channel U channel and V channel
  	unsigned char * Y_data_Dst = Dst_data;
  	unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
  	unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
  	int Dst_Stride_Y;
  	int Dst_Stride_U;
  	int Dst_Stride_V;


  		if (rotate == 90 || rotate == 270)
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
  										     (libyuv::RotationMode) rotate);

  		return getByteToArray(env,Dst_data,I420_Size);

}
//NV21转I420并旋转+镜像
    JNIEXPORT jbyteArray
    JNICALL Java_org_suirui_util_libyuv_jni_libYuvJNI_NV21ToI420RotateMirror(JNIEnv *env, jclass obj, jbyteArray srcData,jint src_width, jint src_height,jint rotate){
    if(Dst_data){
                free(Dst_data);
                         Dst_data=NULL;
             }
        unsigned char *Src_data = byteArrayToChar(env, srcData);



    	// NV12 video size
    	int NV12_Size = src_width * src_height * 3 / 2;
    	int NV12_Y_Size = src_width * src_height;

    	// YUV420 video size
    	int I420_Size = src_width * src_height * 3 / 2;
    	int I420_Y_Size = src_width * src_height;
    	int I420_U_Size = (src_width >> 1)*(src_height >> 1);
    	int I420_V_Size = I420_U_Size;


          	Dst_data = (unsigned char *) malloc(
                        (I420_Size) * sizeof(unsigned char));  //I420

        unsigned char * Y_data_Src = Src_data;
      	unsigned char * UV_data_Src = Src_data + NV12_Y_Size;
      	int src_stride_y = src_width;
      	int src_stride_uv = src_width;

      		//dst: buffer address of Y channel U channel and V channel
      	unsigned char * Y_data_Dst = Dst_data;
      	unsigned char * U_data_Dst = Dst_data + I420_Y_Size;
      	unsigned char * V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;
      	int Dst_Stride_Y;
      	int Dst_Stride_U;
      	int Dst_Stride_V;


      		if (rotate == 90 || rotate == 270)
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
      										 (libyuv::RotationMode) rotate);

      		return getByteToArray(env,Dst_data,I420_Size);
    }

}
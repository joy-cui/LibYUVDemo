#include "OpenCVHelper.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <libyuv.h>
#include "libyuv/rotate.h"


using namespace cv;

extern "C" {




JNIEXPORT jintArray
JNICALL Java_opencv_opencv_OpenCVHelper_test(
        JNIEnv *env, jclass obj, jbyteArray srcy,jbyteArray srcu,jbyteArray srcv, int width, int height,jbyteArray toy,jbyteArray tou,jbyteArray tov) {

    unsigned char* src_y = ( unsigned char*) (env->GetByteArrayElements(srcy, 0));
    unsigned  char* src_u = (unsigned char*) (env->GetByteArrayElements(srcu, 0));
    unsigned  char* src_v = (unsigned char*) (env->GetByteArrayElements(srcv, 0));

    unsigned char* dst_y = (unsigned char*) (env->GetByteArrayElements(toy, 0));
    unsigned  char* dst_u = (unsigned char*) (env->GetByteArrayElements(tou, 0));
    unsigned  char* dst_v = (unsigned char*) (env->GetByteArrayElements(tov, 0));

    int ysize = width * height;
    int usize = ((width + 1) / 2) * ((height + 1) / 2);

//    int I420Rotate(const uint8* src_y, int src_stride_y,
//                   const uint8* src_u, int src_stride_u,
//                   const uint8* src_v, int src_stride_v,
//                   uint8* dst_y, int dst_stride_y,
//                   uint8* dst_u, int dst_stride_u,
//                   uint8* dst_v, int dst_stride_v,
//                   int src_width, int src_height, enum RotationMode mode);

    int ret=libyuv::I420Rotate((const uint8*) src_y,  ysize,
               (const uint8*) src_u,  usize,
               (const uint8*) src_v,  usize,
               ( uint8*) dst_y,  ysize,
               ( uint8*)  dst_u,  usize,
               ( uint8*)  dst_v,  usize,
               width, height, (libyuv::RotationMode)90);



    return 0;
}

}
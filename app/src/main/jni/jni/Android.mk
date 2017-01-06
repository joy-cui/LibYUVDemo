LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE := yuvUtil

LOCAL_SRC_FILES := libYuvUtil.cpp

LOCAL_SHARED_LIBRARIES += libyuv

LOCAL_LDLIBS +=  -lm -llog

include $(BUILD_SHARED_LIBRARY)





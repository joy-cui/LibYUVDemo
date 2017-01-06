# This is the Android makefile for libyuv for both platform and NDK.
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cc

LOCAL_SRC_FILES := \
    source/compare.cc           \
    source/compare_common.cc    \
    source/compare_neon.cc  \
    source/compare_neon64.cc   \
    source/compare_posix.cc  \
    source/compare_win.cc   \
    source/convert.cc  \
    source/convert_argb.cc  \
    source/convert_from.cc  \
    source/convert_from_argb.cc  \
    source/convert_jpeg.cc  \
    source/convert_to_argb.cc   \
    source/convert_to_i420.cc  \
    source/cpu_id.cc  \
    source/format_conversion.cc  \
    source/mjpeg_decoder.cc  \
    source/mjpeg_validate.cc  \
    source/planar_functions.cc  \
    source/rotate.cc  \
    source/rotate_argb.cc  \
    source/rotate_mips.cc  \
    source/rotate_neon.cc   \
    source/rotate_neon64.cc  \
    source/row_any.cc  \
    source/row_common.cc  \
    source/row_mips.cc  \
    source/row_neon.cc  \
    source/row_neon64.cc  \
    source/row_posix.cc  \
    source/row_win.cc  \
    source/row_x86.asm  \
    source/scale.cc   \
    source/scale_argb.cc  \
    source/scale_common.cc  \
    source/scale_mips.cc  \
    source/scale_neon.cc  \
    source/scale_neon64.cc  \
    source/scale_posix.cc  \
    source/scale_win.cc  \
    source/video_common.cc  \
    OpenCVHelper.cpp \







LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include

LOCAL_MODULE := libyuv
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)


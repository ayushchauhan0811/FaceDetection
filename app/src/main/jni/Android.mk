LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#opencv
OPENCVROOT:= C:\Users\Ayush\OpenCV-android-sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES := aac_ndkopencv_DetectionBasedTracker.cpp aac_ndkopencv_OpencvNativeClass.cpp

LOCAL_LDLIBS += -llog -ldl
LOCAL_MODULE := FaceDetection


include $(BUILD_SHARED_LIBRARY)
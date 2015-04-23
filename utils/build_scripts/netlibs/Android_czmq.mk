LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := czmq
LOCAL_SRC_FILES := ../../../android/libs/$(APP_ABI)/libczmq.a

include $(PREBUILT_STATIC_LIBRARY)

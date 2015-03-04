LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := mDNSEmbedded
LOCAL_SRC_FILES := ../../../android/libs/$(APP_ABI)/libmDNSEmbedded.a

include $(PREBUILT_STATIC_LIBRARY)

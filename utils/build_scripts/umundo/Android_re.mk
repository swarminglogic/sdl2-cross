LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := re
LOCAL_SRC_FILES := ../../../android/libs/$(APP_ABI)/libre.a

include $(PREBUILT_STATIC_LIBRARY)

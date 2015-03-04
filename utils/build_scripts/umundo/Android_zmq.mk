LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := zmq
LOCAL_SRC_FILES := ../../../android/libs/$(APP_ABI)/libzmq.a

include $(PREBUILT_STATIC_LIBRARY)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../android/jni/SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_C_INCLUDES := $(GLMDIR)

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
		main.cpp

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
LOCAL_CFLAGS := -fno-strict-aliasing -D_REENTRANT -DGLM_FORCE_RADIANS
LOCAL_CFLAGS += -isystem $(GLMDIR)

include $(BUILD_SHARED_LIBRARY)

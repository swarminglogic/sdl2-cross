LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

JNI_PATH := ../android/jni
SDL_PATH        := $(JNI_PATH)/SDL
SDL_IMAGE_PATH  := $(JNI_PATH)/SDL_image
SDL_MIXER_PATH  := $(JNI_PATH)/SDL_mixer
SDL_TTF_PATH    := $(JNI_PATH)/SDL_ttf

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
  $(LOCAL_PATH)/$(SDL_IMAGE_PATH) \
  $(LOCAL_PATH)/$(SDL_MIXER_PATH) \
  $(LOCAL_PATH)/$(SDL_TTF_PATH) \
  $(BULLETDIR)/include/bullet \
  $(BOOSTDIR)/include \
  $(GLMDIR)

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
  $(shell find $(JNI_PATH)/src/ -name "*.cpp")

# Alternative way to wildcard-include files.
#  $(wildcard $(JNI_PATH)/src/*.cpp)

# To view variables use:
# $(warning $(LOCAL_SRC_FILES))

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer SDL2_ttf bullet
LOCAL_STATIC_LIBRARIES := cpufeatures

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
LOCAL_CFLAGS := -fno-strict-aliasing -D_REENTRANT -DGLM_FORCE_RADIANS
LOCAL_CFLAGS += -isystem $(GLMDIR)
LOCAL_CFLAGS += -isystem $(BOOSTDIR)/include
LOCAL_CFLAGS += -isystem $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_CFLAGS += -isystem $(BULLETDIR)/include/bullet


# Android ndk and stl include paths
LOCAL_CFLAGS += -isystem $(NDK_PLATFORMS_ROOT)/$(APP_PLATFORM)/arch-$(TARGET_ARCH)/usr/include
LOCAL_CFLAGS += -isystem $(NDK_ROOT)/sources/cxx-stl/llvm-libc++/libcxx/include/
LOCAL_CFLAGS += -isystem $(NDK_ROOT)/sources/cxx-stl/gabi++/include

LOCAL_CPPFLAGS += -std=c++11
LOCAL_CPPFLAGS += -Wall -Wextra -Wcast-align -Wcast-qual \
     -fpermissive \
     -Wdisabled-optimization \
     -Wfloat-equal -Wformat=2 -Wimport -Winit-self \
     -Winline -Winvalid-pch -Wlong-long \
     -Wmissing-format-attribute -Wmissing-include-dirs \
     -Wmissing-noreturn -Wpacked -Wpointer-arith \
     -Wredundant-decls -Wshadow -Wstack-protector \
     -Wstrict-aliasing=2 -Wunreachable-code \
     -Wunsafe-loop-optimizations -Wunused \
     -Wvariadic-macros -Wwrite-strings -pedantic \
     -pedantic-errors -Woverloaded-virtual \
     -Wswitch-enum

include $(BUILD_SHARED_LIBRARY)

$(call import-moduleandroid/cpufeatures)

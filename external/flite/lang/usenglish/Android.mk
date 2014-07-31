LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_MODULE    := flite_usenglish
LOCAL_CFLAGS    := -Werror -fPIC

SOURCEPATH := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	  $(wildcard $(SOURCEPATH)/*.c)	\
	)


include $(BUILD_STATIC_LIBRARY)

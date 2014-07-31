LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_MODULE    := flite_voice_list
LOCAL_CFLAGS    := -Werror

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(LOCAL_PATH)/flite_voice_list.c)

include $(BUILD_STATIC_LIBRARY)

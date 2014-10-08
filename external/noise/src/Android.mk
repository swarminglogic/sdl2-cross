LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_MODULE    := noise
LOCAL_CFLAGS    := -Werror -fPIC -fomit-frame-pointer

SOURCEPATH := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
		$(wildcard $(SOURCEPATH)/*.cpp) \
		$(wildcard $(SOURCEPATH)/module/*.cpp) \
		$(wildcard $(SOURCEPATH)/model/*.cpp) \
	)

include $(BUILD_STATIC_LIBRARY)

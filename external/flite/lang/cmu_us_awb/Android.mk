LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include \
                    $(LOCAL_PATH)/../cmulex \
                    $(LOCAL_PATH)/../usenglish

LOCAL_MODULE    := flite_cmu_us_awb
LOCAL_CFLAGS    := -Werror -fPIC

SOURCEPATH := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
		$(wildcard $(SOURCEPATH)/*.c) \
	)

include $(BUILD_STATIC_LIBRARY)

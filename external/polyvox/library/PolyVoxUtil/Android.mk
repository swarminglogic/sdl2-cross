LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include \
	$(LOCAL_PATH)/../PolyVoxCore/include
LOCAL_MODULE := PolyVoxUtil
LOCAL_CFLAGS := -fPIC -std=c++11 \
	-DPolyVoxUtil_EXPORTS -DPOLYVOX_SHARED_EXPORTS

SOURCEPATH := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
		$(wildcard $(SOURCEPATH)/source/*.cpp) \
	)

include $(BUILD_STATIC_LIBRARY)

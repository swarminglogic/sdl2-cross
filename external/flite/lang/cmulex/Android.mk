LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_MODULE    := flite_cmulex
LOCAL_CFLAGS    := -fPIC

SOURCEPATH := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
		$(LOCAL_PATH)/cmu_lex.c  \
		$(LOCAL_PATH)/cmu_lex_data.c \
		$(LOCAL_PATH)/cmu_lex_entries.c \
		$(LOCAL_PATH)/cmu_lts_model.c \
		$(LOCAL_PATH)/cmu_lts_rules.c \
		$(LOCAL_PATH)/cmu_postlex.c \
	)

include $(BUILD_STATIC_LIBRARY)

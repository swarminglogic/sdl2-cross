LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_MODULE    := flite
LOCAL_CFLAGS    := -Werror -fPIC

SOURCEPATH := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
		$(wildcard $(SOURCEPATH)/cg/*.c) \
		$(wildcard $(SOURCEPATH)/hrg/*.c) \
		$(wildcard $(SOURCEPATH)/lexicon/*.c) \
		$(wildcard $(SOURCEPATH)/regex/*.c) \
		$(wildcard $(SOURCEPATH)/speech/*.c) \
		$(wildcard $(SOURCEPATH)/stats/*.c) \
		$(wildcard $(SOURCEPATH)/synth/*.c) \
		$(wildcard $(SOURCEPATH)/wavesynth/*.c) \
		$(LOCAL_PATH)/audio/auclient.c \
		$(LOCAL_PATH)/audio/au_command.c \
		$(LOCAL_PATH)/audio/audio.c \
		$(LOCAL_PATH)/audio/au_none.c \
		$(LOCAL_PATH)/audio/auserver.c \
		$(LOCAL_PATH)/audio/au_streaming.c \
		$(LOCAL_PATH)/utils/cst_alloc.c \
		$(LOCAL_PATH)/utils/cst_args.c \
		$(LOCAL_PATH)/utils/cst_endian.c \
		$(LOCAL_PATH)/utils/cst_error.c \
		$(LOCAL_PATH)/utils/cst_features.c \
		$(LOCAL_PATH)/utils/cst_file_stdio.c \
		$(LOCAL_PATH)/utils/cst_socket.c \
		$(LOCAL_PATH)/utils/cst_string.c \
		$(LOCAL_PATH)/utils/cst_tokenstream.c \
		$(LOCAL_PATH)/utils/cst_val.c \
		$(LOCAL_PATH)/utils/cst_val_const.c \
		$(LOCAL_PATH)/utils/cst_val_user.c \
		$(LOCAL_PATH)/utils/cst_wchar.c \
	)


include $(BUILD_STATIC_LIBRARY)

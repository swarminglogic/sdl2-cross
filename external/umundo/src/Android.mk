LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../src               \
	$(LOCAL_PATH)/../src/include_android/                 \
	$(LOCAL_PATH)/../fastlz                               \
	$(LOCAL_PATH)/../zeromq-4.1.0/include                 \
	$(LOCAL_PATH)/../mDNSResponder-333.10/mDNSCore        \
	$(LOCAL_PATH)/../mDNSResponder-333.10/mDNSPosix       \
	$(LOCAL_PATH)/../mDNSResponder-333.10/mDNSShared      \
	$(LOCAL_PATH)/../re-0.4.7/include                     \
	$(BOOST_DIR)/include

LOCAL_MODULE    := libumundocore64
LOCAL_CFLAGS    := -DBUILD_RELEASE -O3 -DNDEBUG -DUMUNDO_STATIC          \
	-Wall -Wno-attributes -Wno-uninitialized -Wno-narrowing                \
	-Wno-unused-local-typedefs -fno-strict-aliasing -Wno-unused-value      \
	-Wno-unused-function -Wno-sign-compare \
	-DTARGET_OS_ANDROID

SOURCEPATH := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
    $(LOCAL_PATH)/umundo/core/Message.cpp \
    $(LOCAL_PATH)/umundo/core/Debug.cpp \
    $(LOCAL_PATH)/umundo/core/Error.cpp \
    $(LOCAL_PATH)/umundo/core/UUID.cpp \
    $(LOCAL_PATH)/umundo/core/Host.cpp \
    $(LOCAL_PATH)/umundo/core/Factory.cpp \
    $(LOCAL_PATH)/umundo/core/portability.cpp \
    $(LOCAL_PATH)/umundo/core/connection/Publisher.cpp \
    $(LOCAL_PATH)/umundo/core/connection/Node.cpp \
    $(LOCAL_PATH)/umundo/core/connection/Subscriber.cpp \
    $(LOCAL_PATH)/umundo/core/discovery/Discovery.cpp \
    $(LOCAL_PATH)/umundo/core/thread/tinythread.cpp \
    $(LOCAL_PATH)/umundo/core/thread/Thread.cpp \
    $(LOCAL_PATH)/umundo/core/connection/zeromq/ZeroMQSubscriber.cpp \
    $(LOCAL_PATH)/umundo/core/connection/zeromq/ZeroMQNode.cpp \
    $(LOCAL_PATH)/umundo/core/connection/zeromq/ZeroMQPublisher.cpp \
    $(LOCAL_PATH)/umundo/core/connection/rtp/RTPPublisher.cpp \
    $(LOCAL_PATH)/umundo/core/connection/rtp/RTPThread.cpp \
    $(LOCAL_PATH)/umundo/core/connection/rtp/RTPSubscriber.cpp \
    $(LOCAL_PATH)/umundo/core/discovery/MDNSDiscovery.cpp \
    $(LOCAL_PATH)/umundo/core/discovery/mdns/bonjour/BonjourDiscoverer.cpp \
    $(LOCAL_PATH)/umundo/core/discovery/BroadcastDiscovery.cpp \
    $(LOCAL_PATH)/umundo/core/discovery/mdns/bonjour/mDNSEmbedded.c \
    $(LOCAL_PATH)/../fastlz/fastlz.c \
		)

include $(BUILD_STATIC_LIBRARY)

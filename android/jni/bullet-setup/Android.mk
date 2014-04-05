LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

BULLETPATH = ../bullet-src/src
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(BULLETPATH)

LOCAL_MODULE    := libbullet
LOCAL_CFLAGS    := -Werror
LOCAL_LDLIBS    := -llog -lGLESv2 -lz -landroid

SOURCEPATH := $(LOCAL_PATH)/$(BULLETPATH)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(SOURCEPATH)/BulletDynamics/Vehicle/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletDynamics/Character/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletDynamics/Featherstone/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletDynamics/MLCPSolvers/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletDynamics/Dynamics/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletDynamics/ConstraintSolver/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletCollision/CollisionShapes/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletCollision/NarrowPhaseCollision/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletCollision/Gimpact/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletCollision/CollisionDispatch/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletCollision/BroadphaseCollision/*.cpp) \
	$(wildcard $(SOURCEPATH)/LinearMath/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletMultiThreaded/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletMultiThreaded/SpuSampleTask/*.cpp) \
	$(wildcard $(SOURCEPATH)/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/*.cpp) \
  $(wildcard $(SOURCEPATH)/BulletSoftBody/*.cpp))

include $(BUILD_SHARED_LIBRARY)

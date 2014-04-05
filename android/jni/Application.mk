
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information

#NDK_TOOLCHAIN_VERSION := 4.8
NDK_TOOLCHAIN_VERSION := clang3.4
APP_STL := gnustl_shared
APP_ABI := armeabi #armeabi-v7a x86
APP_PLATFORM := android-19
APP_ARM_MODE := thumb
APP_CPPFLAGS := -fexceptions -frtti

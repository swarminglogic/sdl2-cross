
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information

NDK_TOOLCHAIN_VERSION := 4.8
#NDK_TOOLCHAIN_VERSION := clang3.4
APP_OPTIM := debug
APP_STL := c++_shared
APP_ABI := armeabi #armeabi-v7a x86
APP_PLATFORM := android-18
APP_ARM_MODE := thumb
APP_CPPFLAGS := -DUSING_SDL -fexceptions -frtti
LIBCXX_FORCE_REBUILD := false

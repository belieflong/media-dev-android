LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/libcommon \
    $(LOCAL_PATH)/3rdparty/ffmpeg/include     

LOCAL_SRC_FILES = \
SoundTrackController.cpp \
MusicDecoder.cpp 

LOCAL_STATIC_LIBRARIES := libcommontool libmusicdecoder libmedia

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog 
# Link with OpenSL ES
LOCAL_LDLIBS += -lOpenSLES
LOCAL_LDLIBS += -lz
LOCAL_LDLIBS += -landroid

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavfilter.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavformat.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavcodec.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavutil.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libswscale.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libswresample.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libx264.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libfdk-aac.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libmp3lame.a
else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv8-a/libavfilter.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv8-a/libavformat.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv8-a/libavcodec.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv8-a/libavutil.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv8-a/libswscale.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv8-a/libswresample.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv8-a/libx264.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv8-a/libfdk-aac.a
    LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv8-a/libmp3lame.a
endif

#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libfdk-aac.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavfilter.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libvo-aacenc.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavformat.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavcodec.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavutil.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libswscale.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libswresample.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libpostproc.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libx264.a

#LOCAL_LDLIBS += -L$(LOCAL_PATH)/3rdparty/prebuilt/armv7-a -lfdk-aac -lvo-aacenc

LOCAL_MODULE := libsongstudio
include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))
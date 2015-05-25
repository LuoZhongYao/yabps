LOCAL_PATH 	:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE 	:= rda5876
LOCAL_SRC_FILES := rda5876.c

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE 	:= mm
LOCAL_SRC_FILES := mm.c

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := auth
LOCAL_SRC_FILES := libauth.a

include $(PREBUILT_STATIC_LIBRARY)

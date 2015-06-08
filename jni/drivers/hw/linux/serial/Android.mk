LOCAL_PATH 	:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE 	:= drv
LOCAL_SRC_FILES := serial.c

include $(BUILD_STATIC_LIBRARY)

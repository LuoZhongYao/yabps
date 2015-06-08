LOCAL_PATH 	:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE		:= abcsp
C_FILES 			:= $(wildcard $(LOCAL_PATH)/src/*.c)
LOCAL_SRC_FILES		:= $(C_FILES:$(LOCAL_PATH)/%=%)
LOCAL_CFLAGS 		:= -DABCSP_TEST
LOCAL_C_INCLUDES	:= $(LOCAL_PATH)/include\
	$(LOCAL_PATH)/config

include $(BUILD_STATIC_LIBRARY)

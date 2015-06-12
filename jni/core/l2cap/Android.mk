LOCAL_PATH 	:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE		:= l2cap
C_FILES 			:= $(wildcard $(LOCAL_PATH)/*.c)
LOCAL_SRC_FILES		:= $(C_FILES:$(LOCAL_PATH)/%=%)

include $(BUILD_STATIC_LIBRARY)

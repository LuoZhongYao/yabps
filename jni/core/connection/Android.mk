LOCAL_PATH 	:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE		:= connection
C_FILES 			:= $(wildcard $(LOCAL_PATH)/*.c)
LOCAL_CONLYFLAGS	:= -D__LOG_CBK__
LOCAL_SRC_FILES		:= $(C_FILES:$(LOCAL_PATH)/%=%)

include $(BUILD_STATIC_LIBRARY)

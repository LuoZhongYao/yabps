LOCAL_PATH 	:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE		:= hci
LOCAL_CONLYFLAGS	:=  \
	-D__LOG_HCI_EVENT__\
	-D__LOG_ACL__\
	-D__DUMP_HCI__

C_FILES 			:= $(wildcard $(LOCAL_PATH)/*.c)
LOCAL_SRC_FILES		:= $(C_FILES:$(LOCAL_PATH)/%=%)

include $(BUILD_STATIC_LIBRARY)

TOP 	:= $(call my-dir)

APP_ABI := armeabi
APP_CONLYFLAGS 	:= -std=gnu99
APP_C_INCLUDE	:= $(TOP)/include

APP_CFLAGS 	:= \
	-I$(APP_C_INCLUDE) \

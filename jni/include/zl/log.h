#ifndef __ZL_LOG_H__
#define __ZL_LOG_H__
#include <android/log.h>

#define TAG "ZL"

#define LOGI(...)   __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define LOGD(...)   __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...)   __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

#endif

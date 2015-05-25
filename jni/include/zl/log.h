#ifndef __ZL_LOG_H__
#define __ZL_LOG_H__
#include <sys/types.h>
#include <android/log.h>

__BEGIN_DECLS

#define TAG "ZL"

#define LOGI(...)   __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define LOGD(...)   __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...)   __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

__END_DECLS

#endif

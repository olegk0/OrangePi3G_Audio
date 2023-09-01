#ifndef _LIBS_UTILS_LOG_H
#define _LIBS_UTILS_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdint.h>
#include <string>

#define u8 uint8_t
#define u64 uint64_t

// #define aee_system_exception(...)
// #define BT_ENABLE
// #define PHONE_AUDIO
// #define MATV_ENABLE
// #define FILTER_ENABLE
// #define IN_STREAM_ENABLE
// #define NVRAM_ENABLE
// #define LOOPBACK_ENABLE

#define String8 std::string

#define LOG_ERROR (0)
#define LOG_WARN (1)
#define LOG_INFO (2)
#define LOG_VERBOSE (3)
#define LOG_DEBUG (4)

#define LOG_LEVEL LOG_INFO

#define ALOG(ll, tag, format, ...) \
    printf("%d - %s " format " - [%s]\n", ll, tag, ##__VA_ARGS__, __PRETTY_FUNCTION__)
/*
#define android_printAssert(cond, tag, format, ...) \
    printf("%s " format " - [%s]\n", tag, ##__VA_ARGS__, __PRETTY_FUNCTION__)
*/
#define __android_log_assert(cond, tag, ...) \
    printf("%s ", tag, ##__VA_ARGS__)

#if LOG_LEVEL >= LOG_ERROR
#define ALOGE(format, ...) printf(format " - [%s]\n", ##__VA_ARGS__, __PRETTY_FUNCTION__)
#else
#define ALOGE(format, ...)
#endif
#if LOG_LEVEL >= LOG_WARN
#define ALOGW(format, ...) printf(format " - [%s]\n", ##__VA_ARGS__, __PRETTY_FUNCTION__)
#else
#define ALOGW(format, ...)
#endif
#if LOG_LEVEL >= LOG_INFO
#define ALOGI(format, ...) printf(format " - [%s]\n", ##__VA_ARGS__, __PRETTY_FUNCTION__)
#else
#define ALOGI(format, ...)
#endif
#if LOG_LEVEL >= LOG_VERBOSE
#define ALOGV(format, ...) printf(format " - [%s]\n", ##__VA_ARGS__, __PRETTY_FUNCTION__)
#else
#define ALOGV(format, ...)
#endif
#if LOG_LEVEL >= LOG_DEBUG
#define ALOGD(format, ...) printf(format " - [%s]\n", ##__VA_ARGS__, __PRETTY_FUNCTION__)
#else
#define ALOGD(format, ...)
#endif

// #define WARNING(format, ...) printf(format " - [%s]\n", ##__VA_ARGS__, __PRETTY_FUNCTION__)
/*
inline bool IsAudioSupportFeature(int dFeatureOption) {
    return false;
}
*/
inline int
property_get(const char *key, char *value, const char *default_value) {
    return 0;
}

#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

#define CONDITION(cond) (__builtin_expect((cond) != 0, 0))

#define __android_second(dummy, second, ...) second
/* If passed multiple args, returns ',' followed by all but 1st arg, otherwise
 * returns nothing.
 */
#define __android_rest(first, ...) , ##__VA_ARGS__

#define android_printAssert(cond, tag, fmt...) \
    __android_log_assert(cond, tag,            \
                         __android_second(0, ##fmt, NULL) __android_rest(fmt))

#ifndef LOG_ALWAYS_FATAL_IF
#define LOG_ALWAYS_FATAL_IF(cond, ...)                                \
    ((CONDITION(cond))                                                \
         ? ((void)android_printAssert(#cond, LOG_TAG, ##__VA_ARGS__)) \
         : (void)0)
#endif

#ifndef LOG_FATAL_IF
#define LOG_FATAL_IF(cond, ...) LOG_ALWAYS_FATAL_IF(cond, ##__VA_ARGS__)
#endif

#ifndef ALOG_ASSERT
#define ALOG_ASSERT(cond, ...) LOG_FATAL_IF(!(cond), ##__VA_ARGS__)
// #define ALOG_ASSERT(cond) LOG_FATAL_IF(!(cond), "Assertion failed: " #cond)
#endif

inline int property_set(const char *key, const char *value) {
    return 0;
}

#endif
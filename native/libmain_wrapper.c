#include <android/log.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <jni.h>

#define TAG "PGOHOOK431"

/*
 * Every line goes to logcat AND to a file in the app's external files dir, so evidence
 * survives logcat eviction:  /sdcard/Android/data/com.nianticlabs.pokemongo/files/pgo_hook.log
 */
#ifndef PGO_LOG_ROOT
#define PGO_LOG_ROOT "/storage/emulated/0/Android/data/com.nianticlabs.pokemongo"
#endif
#define PGO_LOG_DIR PGO_LOG_ROOT "/files"
#define PGO_LOG_FILE PGO_LOG_DIR "/pgo_hook.log"

__attribute__((format(printf, 2, 3)))
static void plog(int prio, const char *fmt, ...) {
    char line[512];
    int off = snprintf(line, sizeof(line), "%d: ", (int)getpid());
    if (off < 0) return;
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(line + off, sizeof(line) - (size_t)off - 2, fmt, ap);
    va_end(ap);
    if (n < 0) return;
    __android_log_write(prio, TAG, line + off);
    size_t len = (size_t)off + (size_t)n;
    if (len > sizeof(line) - 2) len = sizeof(line) - 2;
    line[len++] = '\n';
    mkdir(PGO_LOG_ROOT, 0775);
    mkdir(PGO_LOG_DIR, 0775);
    int fd = open(PGO_LOG_FILE, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0664);
    if (fd >= 0) {
        ssize_t w = write(fd, line, len);
        (void)w;
        close(fd);
    }
}
#define LOGI(...) plog(ANDROID_LOG_INFO, __VA_ARGS__)
#define LOGE(...) plog(ANDROID_LOG_ERROR, __VA_ARGS__)

typedef jint (*jni_onload_t)(JavaVM *, void *);

__attribute__((visibility("default")))
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    {   /* start a fresh log file for this launch */
        int fd = open(PGO_LOG_FILE, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0664);
        if (fd >= 0) close(fd);
    }
    LOGI("wrapper libmain.so JNI_OnLoad entered");
    void *orig = dlopen("libmain_orig.so", RTLD_NOW);
    if (!orig) {
        LOGE("dlopen libmain_orig.so failed: %s", dlerror());
        return -1;
    }
    jni_onload_t fn = (jni_onload_t)dlsym(orig, "JNI_OnLoad");
    if (!fn) {
        LOGE("JNI_OnLoad missing in libmain_orig.so: %s", dlerror());
        return -1;
    }
    /* A hook failure must not stop the game from starting. */
    if (!dlopen("libpgo_hook.so", RTLD_NOW))
        LOGE("dlopen libpgo_hook.so failed: %s", dlerror());
    else
        LOGI("libpgo_hook.so loaded");
    return fn(vm, reserved);
}

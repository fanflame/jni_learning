#include <jni.h>
#include <string>
#include <android/log.h>

#ifdef __arm__
#define AAA "4444"
#elif defined(__aarch64__)
#define AAA "555"
#else
#define AAA "666"
#endif

#define TAG "TTTT"
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)

typedef  struct TimerContext{
    JavaVM *javaVm;
    jclass handlerClass;
} TimerContext;
TimerContext timerContext;

void doHandlerFun(JNIEnv *pEnv);

extern "C" JNIEXPORT jstring JNICALL
Java_com_fanyiran_hello_1jni_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = AAA;
    return env->NewStringUTF(hello.c_str());
}extern "C"
JNIEXPORT void JNICALL
Java_com_fanyiran_hello_1jni_MainActivity_startTicks(JNIEnv *env, jobject thiz) {
    // TODO: implement startTicks()
}extern "C"
JNIEXPORT void JNICALL
Java_com_fanyiran_hello_1jni_MainActivity_StopTicks(JNIEnv *env, jobject thiz) {
    // TODO: implement StopTicks()
}

void doHandlerFun(JNIEnv *jniEnv) {
    jclass handlerClass = timerContext.handlerClass;
    jmethodID getBuildMethodID = jniEnv->GetStaticMethodID(handlerClass,"getBuildVersion", "()Ljava/lang/String;");
    jstring result = static_cast<jstring>(jniEnv->CallStaticObjectMethod(handlerClass,
                                                                         getBuildMethodID));

    const char* getBuildVersionResult = jniEnv->GetStringUTFChars(result,NULL);
    LOGW("getBuildVersion:%s",getBuildVersionResult);

    jniEnv->ReleaseStringUTFChars(result,getBuildVersionResult);
    jniEnv->DeleteLocalRef(result);

    jmethodID handlerInitMethodId = jniEnv->GetMethodID(handlerClass, "<init>", "()V");
    jobject handler = jniEnv->NewObject(handlerClass, handlerInitMethodId);
    jmethodID getRuntimeMemorySizeMethodId = jniEnv->GetMethodID(handlerClass,"getRuntimeMemorySize", "()J");
    jlong memoryResult = jniEnv->CallLongMethod(handler,getRuntimeMemorySizeMethodId);
    LOGW("getRuntimeMemorySize:%d",memoryResult);

    jmethodID updateStatusMethodId = jniEnv->GetMethodID(handlerClass,"updateStatus", "(Ljava/lang/String;)V");
    jstring params = jniEnv->NewStringUTF("hahaha");
    jniEnv->CallVoidMethod(handler,updateStatusMethodId,params);
    jniEnv->DeleteLocalRef(params);

    jniEnv->DeleteLocalRef(handler);
}

jint JNI_OnLoad(JavaVM *javaVm, void *reserved){
    LOGW("JNI_OnLoad");
    memset(&timerContext,0,sizeof(timerContext));
    timerContext.javaVm = javaVm;
    JNIEnv *jniEnv = nullptr;
    jint envResult = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);
    if (envResult != JNI_OK) {
        LOGW("GetEnv failed");
        return JNI_ERR;
    }
    timerContext.handlerClass = jniEnv->FindClass("com/fanyiran/hello_jni/JniHandler");
    doHandlerFun(jniEnv);

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved){
    LOGW("JNI_OnUnload");
}
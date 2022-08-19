// dear imgui: standalone example application for Android + OpenGL ES 3
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
#include "Menu.h"
using namespace std;

#define TAG "mainLog" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"TAG" ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


int getProcessID(const char *packageName)
{
    int id = -1;
    DIR *dir;
    FILE *fp;
    char filename[64];
    char cmdline[64];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL)
    {
        id = atoi(entry->d_name);
        if (id != 0)
        {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp)
            {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(packageName, cmdline) == 0)
                {
                    return id;
                }
            }
        }
    }
    closedir(dir);
    return -1;
}





char *GpakName;
int g_pid;
char *Jstring2string(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");  //String
    jstring strencode = env->NewStringUTF("GB2312"); //"gb2312"
    jmethodID mid = env->GetMethodID(clsstring, "getBytes",
                                     "(Ljava/lang/String;)[B"); //getBytes(Str);
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid,
                                                         strencode); // String .getByte("GB2312");
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);         //"\0"
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);  //释放内存空间
    return rtn;
}



extern "C" {
	


//初始化opengl窗口
JNICALL void setSurface(JNIEnv *env, jclass cla, jobject surface) {
    window = ANativeWindow_fromSurface(env, surface);
	ANativeWindow_acquire(window);
    pthread_t t1;
    pthread_create(&t1, NULL, init, NULL);
	//thread_join(t1,NULL);
}


//获取触摸事件
JNIEXPORT void JNICALL
Java_com_mycompany_applicationSuperJNI_navateInputEvent(JNIEnv *env, jclass obj, jobject motionEvent) {
    if (g_Initialized) {
        jclass eventClss = env->GetObjectClass(motionEvent);
        jmethodID methodId = env->GetMethodID(eventClss, "getAction", "()I");
        inputEvent.Action = env->CallIntMethod(motionEvent, methodId);
        methodId = env->GetMethodID(eventClss, "getRawX", "()F");
        inputEvent.x = env->CallFloatMethod(motionEvent, methodId);
        methodId = env->GetMethodID(eventClss, "getRawY", "()F");
        inputEvent.y = env->CallFloatMethod(motionEvent, methodId);
        int32_t event_pointer_index = (inputEvent.Action & 65280) >> 8;
        methodId = env->GetMethodID(eventClss, "getToolType", "(I)I");
        inputEvent.ToolType = env->CallIntMethod(motionEvent, methodId, event_pointer_index);
        methodId = env->GetMethodID(eventClss, "getButtonState", "()I");
        inputEvent.ButtonState = env->CallIntMethod(motionEvent, methodId);
        methodId = env->GetMethodID(eventClss, "getAxisValue", "(II)F");
        inputEvent.AXIS_VSCROLL = env->CallFloatMethod(motionEvent, methodId, 9,
                                                       event_pointer_index);
        inputEvent.AXIS_HSCROLL = env->CallFloatMethod(motionEvent, methodId, 10,
                                                       event_pointer_index);
        handleInputEvent(inputEvent);
    }
}
JNIEXPORT jfloat JNICALL Java_com_mycompany_applicationSuperJNI_getMenuW(JNIEnv *env, jclass clazz) {
    return menuPos.w;
}

JNIEXPORT jfloat JNICALL Java_com_mycompany_applicationSuperJNI_getMenuH(JNIEnv *env, jclass clazz) {
    return menuPos.h;
}
JNIEXPORT jfloat JNICALL Java_com_mycompany_applicationSuperJNI_getMenuX(JNIEnv *env, jclass clazz) {
    return menuPos.x;
}
JNIEXPORT jfloat JNICALL Java_com_mycompany_applicationSuperJNI_getMenuY(JNIEnv *env, jclass clazz) {
    return menuPos.y;
}



/*****************************************
*   native方法注册辅助函数
*****************************************/
#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif
int jniRegisterNativeMethods(JNIEnv *env, const char *className,
                             JNINativeMethod *gMethods, int numMethods) {
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}



JNICALL void setPid(JNIEnv *env, jclass clazz, jint pid){
    g_pid = pid;
    if (ptrace(PTRACE_TRACEME, g_pid) < 0) {
        printf("主進程被跟蹤");
        kill(g_pid,9);
        exit(-1);
    }
    ptrace(PTRACE_TRACEME,g_pid ,0 ,0);
}

JNICALL jint
getPid(JNIEnv *env, jclass clazz, jstring PackageName){
	GpakName=Jstring2string(env,PackageName);
	pid = getProcessID(GpakName);
    return pid;
}

/**********************************************
*    java层的方法与native映射标
**********************************************/
static JNINativeMethod native_method_table[] = {
        {"setSurface", "(Landroid/view/Surface;)V", 
		(void *) setSurface},
		
        {"setPid", "(I)V", 
		(void *) setPid},
		
        {"getPID", "(Ljava/lang/String;)I",
		(void *) getPid},
};

int register_native_api(JNIEnv *env) {
    return jniRegisterNativeMethods(env, "com/OpenRoot/Canvas/SuperJNI", native_method_table,
                                    NELEM(native_method_table));
}


jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    assert(env != NULL);
    if (!register_native_api(env)) {//注册接口
        return -1;
    }
    return JNI_VERSION_1_6;
}

}


# include "com_baidu_ndk_NativaUtils.h"

# include "pthread.h"
# include "stdio.h"
#include <android/log.h>
#include <unistd.h>
#include <time.h>

#define THREAD_NUM 5

#define  LOG_TAG    "jnimain"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

int err;
pthread_t ntid;    //线程号

JavaVM *g_jvm = NULL;
jobject myObj = NULL;


pthread_mutex_t mut;
pthread_t threads[2];
int number = 0, i;

char *getSystemTime() {
    time_t t;
    time(&t);

    return ctime(&t);
}

void *thread_call_fromJNI(void *arg) {
    JNIEnv *env;
    jclass class;
    jmethodID mid;

    //Attach主线程

    if ((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK) {
        printf("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }

    //找到对应的类

    class = (*env)->GetObjectClass(env, myObj);

    if (class == NULL) {
        LOGI("Find class error ");
        goto error;
    }

    mid = ((*env)->GetStaticMethodID(env, class, "fromJNI", "(I)V"));


    if (mid == NULL) {
        printf("getMethod error");
        goto error;
    }

    for (int i = 0; i < 10; ++i) {
        (*env)->CallStaticVoidMethod(env, class, mid, (int) arg);
        sleep(1);
    }


    error:

    // Detach 主线程
    if ((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        printf("%s : DetachCurrentThread failed ", __FUNCTION__);
    }

    pthread_exit(0);


}

void thread_sendmsg_java() {

    JNIEnv *newEnv;

    if ((*g_jvm)->AttachCurrentThread(g_jvm, &newEnv, NULL) != JNI_OK) {
        LOGI("%s: AttachCurrentThread() failed", __FUNCTION__);
        goto error;
    }

    jclass javaNativeUtils = (*newEnv)->GetObjectClass(newEnv, myObj);

    if (javaNativeUtils == NULL) {
        LOGE(" find class error ");
        goto error;
    }

    jmethodID methodDisplayMessage = (*newEnv)->GetMethodID(newEnv, javaNativeUtils,
                                                            "handleNativeMessage",
                                                            "(Ljava/lang/String;)V");

    if (methodDisplayMessage == NULL) {
        LOGE(" find methord error ");
        goto error;
    }


    for (int i = 0; i < 10; ++i) {
        sleep(1);
        jstring value = (*newEnv)->NewStringUTF(newEnv, getSystemTime());
        (*newEnv)->CallVoidMethod(newEnv, myObj, methodDisplayMessage, value);
    }

    (*g_jvm)->DetachCurrentThread(g_jvm);

    error :

    pthread_exit(0);
}

void *thread1() {
    LOGI("thread 1 is creating ");
    for (int i = 0; i < THREAD_NUM; ++i) {
        LOGI("thread 1 number = %d \n", number);
        pthread_mutex_lock(&mut);
        number++;
        sleep(2);
        pthread_mutex_unlock(&mut);

    }
    LOGI("thread1 :完成任务退出 ！\n");
    pthread_exit(NULL);
}

void *thread2() {
    LOGI("thread 2is creating ");
    for (int i = 0; i < THREAD_NUM; ++i) {
        LOGI("thread 2 number = %d \n", number);
        pthread_mutex_lock(&mut);
        number++;
        sleep(3);
        pthread_mutex_unlock(&mut);
    }
    LOGI("thread2 :完成任务退出 ？\n");
    pthread_exit(NULL);
}


void thread_creat(void) {
    int temp = 0;
    memset(&threads, 0, sizeof(threads));

    /*create thread */
    if ((temp = pthread_create(&threads[0], NULL, thread1, NULL)) != 0) {
        LOGI("create thread 1 fail");
    } else {
        LOGI("create thread 1 success ");
    }


    if ((temp = pthread_create(&threads[1], NULL, thread2, NULL)) != 0) {
        LOGI("create thread 2 fail");
    } else {
        LOGI("create thread 2 success ");
    }
}

void thread_wait(void) {
    /*等待线程结束*/
    if (threads[0] != 0) {                   //comment4
        pthread_join(threads[0], NULL);
        LOGI("线程1已经结束\n");
    }
    if (threads[1] != 0) {                //comment5
        pthread_join(threads[1], NULL);
        LOGI("线程2已经结束\n");
    }
}

void createMutiThread(void) {
    pthread_mutex_init(&mut, NULL);
    LOGI("multi thread create !");
    thread_creat();
    LOGI("我是主函数哦，我正在等待线程完成任务阿，呵呵\n");
    // thread_wait();
}

void startGetTime(void) {
    int temp = 0;

    pthread_t my_th;
    memset(&my_th, 0, sizeof(my_th));

    if ((temp = pthread_create(&my_th, NULL, thread_sendmsg_java, NULL)) != 0) {
        LOGI("create get time thread  fail");
    } else {
        LOGI("create get time thread  thread success ");
    }
}


void call_java_method(JNIEnv *env, jobject obj) {
    jclass javaNativeUtils = (*env)->FindClass(env, "com/baidu/ndk/NativaUtils");
    jmethodID MethodDisplayMessage = (*env)->GetMethodID(env, javaNativeUtils,
                                                         "handleNativeMessage",
                                                         "(Ljava/lang/String;)V");
    jstring value = (*env)->NewStringUTF(env, "click to show time ");
    (*env)->CallVoidMethod(env, obj, MethodDisplayMessage, value);
}


JNIEXPORT jstring JNICALL Java_com_baidu_ndk_NativaUtils_getTime
        (JNIEnv *env, jobject obj) {

    call_java_method(env, obj);
    return (*env)->NewStringUTF(env, "This is Jni test!!!");
}

JNIEXPORT jstring JNICALL Java_com_baidu_ndk_NativaUtils_startGetTime
        (JNIEnv *env, jobject obj) {

    startGetTime();
}


pthread_mutex_t mut;

JNIEXPORT jstring JNICALL Java_com_baidu_ndk_NativaUtils_setJNIEnv
        (JNIEnv *env, jobject obj) {

    (*env)->GetJavaVM(env, &g_jvm);
    myObj = (*env)->NewGlobalRef(env, obj);
}

JNIEXPORT jstring JNICALL Java_com_baidu_ndk_NativaUtils_startMutiThread
        (JNIEnv *env, jobject obj) {

    createMutiThread();
}


//当动态库被加载时这个函数被系统调用
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    //获取JNI版本
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        printf("GetEnv failed!");
        return result;
    }

    return JNI_VERSION_1_4;
}


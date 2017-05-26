package com.baidu.ndk;

import android.os.Handler;
import android.util.Log;

/**
 * Created by gaoqi03 on 17-5-23.
 */

public class NativaUtils {

    Handler mHandler;

    NativeCallBack mCallBack;

    public NativaUtils(NativeCallBack callBack) {
        mCallBack = callBack;
        mHandler = new Handler();
    }

    static {
        System.loadLibrary("myjni");
    }

    public void handleNativeMessage(final String msg) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                if (mCallBack != null) {
                    mCallBack.displayMessage(msg);
                }
            }
        });
    }

    public native String getTime();

    public native void setJNIEnv();

    public static native void startGetTime();

    public static native void startMutiThread();


    //由JNI中的线程回调
    private static void fromJNI(int i) {
        Log.e("Java------>", "" + i);
    }
}

package com.example.gaoqi03.test;

import android.app.Activity;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.TextView;

import com.baidu.ndk.NativaUtils;
import com.baidu.ndk.NativeCallBack;

/**
 * Created by gaoqi03 on 17-5-23.
 */

public class MainActivity extends Activity implements NativeCallBack, View.OnClickListener{

    TextView nativeStr;
    TextView timeArea;
    TextView mProduceConsumeArea;

    NativaUtils nativaUtils;

    String content;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        content = new String ();

        nativaUtils = new NativaUtils(this);
        nativaUtils.setJNIEnv();

        nativeStr = (TextView)findViewById(R.id.native_str);
        timeArea = (TextView)findViewById(R.id.time_area);
        mProduceConsumeArea = (TextView)findViewById(R.id.produce_cusumer);

        nativeStr.setText(nativaUtils.getTime());
        timeArea.setOnClickListener(this);
        mProduceConsumeArea.setOnClickListener(this);
    }



    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.time_area:
                nativaUtils.startGetTime();
                break;
            case R.id.produce_cusumer:
                nativaUtils.startMutiThread();
                break;

        }
    }

    @Override
    public void displayMessage(String msg) {
        if (!TextUtils.isEmpty(msg)){

            content = msg;
            timeArea.setText(msg);
        }
    }
}

package com.troy;

import android.app.Activity;
import android.os.Bundle;

import com.example.gaoqi03.test.R;

/**
 * Created by gaoqi03 on 17-4-25.
 */

public class TestRomoteServiceActivity extends Activity {
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        //  unbindService(connection);
    }
}

package com.belieflong.ffmpegdecoderaudio;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.phuket.tour.decoder.Mp3Decoder;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("songstudio");
    }
    private static String TAG = "MainActivity";

    /** 原始的文件路径 **/
    private static String mp3FilePath = "/mnt/sdcard/131.mp3";
    /** 解码后的PCM文件路径 **/
    private static String pcmFilePath = "/mnt/sdcard/131.pcm";

    private Button mp3_decoder_btn;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mp3_decoder_btn = findViewById(R.id.mp3_decoder_btn);
        mp3_decoder_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this, "正在解码请稍等...", Toast.LENGTH_SHORT).show();
                new Thread(() ->{
                    long startTimeMills = System.currentTimeMillis();
                    Mp3Decoder decoder = new Mp3Decoder();
                    int ret = decoder.init(mp3FilePath, pcmFilePath);
                    boolean encSuccess;
                    if(ret >= 0) {
                        decoder.decode();
                        decoder.destroy();
                        encSuccess = true;
                    } else {
                        Log.i(TAG, "Decoder Initialized Failed...");
                        encSuccess = false;
                    }
                    int wasteTimeMills = (int)(System.currentTimeMillis() - startTimeMills);
                    Log.i(TAG, "Decode Mp3 Waste TimeMills : " + wasteTimeMills + "ms");
                    runOnUiThread(() ->{
                        Toast.makeText(MainActivity.this, encSuccess ? "解码完成" : "解码失败", Toast.LENGTH_SHORT).show();
                    });
                }).start();
            }
        });
    }
}

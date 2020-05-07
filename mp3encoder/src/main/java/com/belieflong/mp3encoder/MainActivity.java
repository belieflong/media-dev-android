package com.belieflong.mp3encoder;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.phuket.tour.studio.Mp3Encoder;

public class MainActivity extends AppCompatActivity {
    private final String TAG = "MainActivity";

    static {
        System.loadLibrary("audioencoder");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button mp3_encoder_btn = findViewById(R.id.mp3_encoder_btn);
        mp3_encoder_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this, "正在编码请稍等...", Toast.LENGTH_SHORT).show();
                new Thread(() -> {
                    Mp3Encoder encoder = new Mp3Encoder();
                    String pcmPath = "/mnt/sdcard/a_songstudio/vocal.pcm";
                    int audioChannels = 2;
                    int bitRate = 128 * 1024;
                    int sampleRate = 44100;
                    String mp3Path = "/mnt/sdcard/a_songstudio/vocal.mp3";
                    int ret = encoder.init(pcmPath, audioChannels, bitRate, sampleRate, mp3Path);
                    boolean encSuccess;
                    if(ret >= 0) {
                        encoder.encode();
                        encoder.destroy();
                        Log.i(TAG, "Encode Mp3 Success");
                        encSuccess = true;
                    } else {
                        Log.i(TAG, "Encoder Initialized Failed...");
                        encSuccess = false;
                    }
                    runOnUiThread(() ->{
                        Toast.makeText(MainActivity.this, encSuccess ? "编码完成" : "编码失败", Toast.LENGTH_SHORT).show();
                    });
                }).start();
            }
        });
    }
}

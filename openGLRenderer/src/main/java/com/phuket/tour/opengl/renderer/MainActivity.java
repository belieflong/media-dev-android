package com.phuket.tour.opengl.renderer;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

import com.github.piasy.openglestutorial_android.MainMyActivity;
import com.github.piasy.openglestutorial_android.MainDemoActivity;

public class MainActivity extends Activity {


	static {
		System.loadLibrary("songstudio");
	}
	private Button go_btn, btnTestO, btnTestT;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		go_btn = (Button) findViewById(R.id.go_btn);
		btnTestO = (Button) findViewById(R.id.btn_test);
		btnTestT = (Button) findViewById(R.id.btn_test2);
		go_btn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, PngPreviewActivity.class);
				startActivity(intent);
			}
		});
		btnTestO.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, MainDemoActivity.class);
				startActivity(intent);
			}
		});
		btnTestT.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, MainMyActivity.class);
				startActivity(intent);
			}
		});
	}

}

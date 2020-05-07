package com.belieflong.camerarecorder;

import com.phuket.tour.camera.preview.ChangbaRecordingPreviewScheduler;
import com.phuket.tour.camera.preview.ChangbaRecordingPreviewView;
import com.phuket.tour.camera.preview.ChangbaVideoCamera;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.Toast;

public class CameraPreviewActivity extends Activity {

	private RelativeLayout recordScreen;
	private ChangbaRecordingPreviewView surfaceView;
	private ChangbaVideoCamera videoCamera;
	private ChangbaRecordingPreviewScheduler previewScheduler;

	private ImageView switchCameraBtn;
	private Button encodeBtn;
	private boolean isEncoding;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_camera_preview);
		isEncoding = false;
		findView();
		bindListener();
		initCameraResource();
	}

	private void findView() {
		recordScreen = (RelativeLayout) findViewById(R.id.recordscreen);
		switchCameraBtn = (ImageView) findViewById(R.id.img_switch_camera);
		encodeBtn = (Button) findViewById(R.id.encode_btn);
		surfaceView = new ChangbaRecordingPreviewView(this);
		recordScreen.addView(surfaceView, 0);
		surfaceView.getLayoutParams().width = getWindowManager().getDefaultDisplay().getWidth();
		surfaceView.getLayoutParams().height = getWindowManager().getDefaultDisplay().getWidth();
	}

	private void initCameraResource() {
		videoCamera = new ChangbaVideoCamera(this);
		previewScheduler = new ChangbaRecordingPreviewScheduler(surfaceView, videoCamera) {
			public void onPermissionDismiss(final String tip) {
				mHandler.post(new Runnable() {
					@Override
					public void run() {
						Toast.makeText(CameraPreviewActivity.this, tip, Toast.LENGTH_SHORT).show();
					}
				});
			}
		};
	}

	private void bindListener() {
		switchCameraBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				previewScheduler.switchCameraFacing();
			}
		});
		encodeBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				String text = "";
				if(!isEncoding) {
					isEncoding = true;
					text = getString(R.string.stop);
					int width = 360;
					int height = 640;
					int videoBitRate = 700 * 1024;
					int frameRate = 24;
					boolean useHardWareEncoding = false;
					String outputPath = "/mnt/sdcard/a_songstudio/preview_soft.h264";
//					boolean useHardWareEncoding = true;
//					String outputPath = "/mnt/sdcard/a_songstudio/preview_hw.h264";
					previewScheduler.startEncoding(width, height, videoBitRate, frameRate, useHardWareEncoding, outputPath);
				} else {
					isEncoding = false;
					text = getString(R.string.encode);
					previewScheduler.stopEncoding();
				}
				encodeBtn.setText(text);
			}
		});
	}
	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
		}
	};
}

package ru.footmade.keplerc;

import java.io.IOException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.opengl.GLES10;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.Log;
import android.view.MotionEvent;

public class MainActivity extends Activity implements SensorEventListener {
	static TextureLoader pngManager;
	static VibratorHelper vibrator;

	private SensorManager sensorManager;
	private Sensor accelerometer;
	private long acceleromterLastUpdate = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mGLView = new MainGLSurfaceView(this);
		setContentView(mGLView);
		
		pngManager = new TextureLoader(this);
		vibrator = new VibratorHelper(this);

		sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		accelerometer = sensorManager
				.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		sensorManager.registerListener(this, accelerometer,
				SensorManager.SENSOR_DELAY_NORMAL);
	}

	@Override
	protected void onPause() {
		super.onPause();
		mGLView.onPause();
		sensorManager.unregisterListener(this);
	}

	@Override
	protected void onResume() {
		super.onResume();
		mGLView.onResume();
		sensorManager.registerListener(this, accelerometer,
				SensorManager.SENSOR_DELAY_NORMAL);
	}

	private GLSurfaceView mGLView;

	static {
		System.loadLibrary("keplerc");
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		Sensor mySensor = event.sensor;

		if (mySensor.getType() == Sensor.TYPE_ACCELEROMETER) {
			float x = event.values[0];
			float y = event.values[1];
			float z = event.values[2];

			long curTime = System.currentTimeMillis();

			if ((curTime - acceleromterLastUpdate) > 100) {
				acceleromterLastUpdate = curTime;
				nativeAccelerometerChanged(x, y, z);
			}
		}
	}

	private static native void nativeAccelerometerChanged(float x, float y, float z);
}

class MainGLSurfaceView extends GLSurfaceView {
	public MainGLSurfaceView(Context context) {
		super(context);
		mRenderer = new MainRenderer();
		setRenderer(mRenderer);
	}

	public boolean onTouchEvent(final MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			nativeTouchDown(event.getX(), event.getY());
		} else if (event.getAction() == MotionEvent.ACTION_UP) {
			nativeTouchUp(event.getX(), event.getY());
		}
		return true;
	}

	@Override
	public void onPause() {
		super.onPause();
		nativePause();
	}

	@Override
	public void onResume() {
		super.onResume();
		nativeResume();
	}

	MainRenderer mRenderer;

	private static native void nativeTouchDown(float x, float y);
	
	private static native void nativeTouchUp(float x, float y);

	private static native void nativePause();

	private static native void nativeResume();
}

class MainRenderer implements GLSurfaceView.Renderer {
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		nativeInit(MainActivity.pngManager, MainActivity.vibrator);
	}

	public void onSurfaceChanged(GL10 gl, int w, int h) {
		nativeResize(w, h);
	}

	public void onDrawFrame(GL10 gl) {
		nativeRender();
	}

	private static native void nativeInit(TextureLoader pngManager, VibratorHelper vibrator);

	private static native void nativeResize(int w, int h);

	private static native void nativeRender();

	private static native void nativeDone();
}

class TextureLoader {
	private AssetManager amgr;

	public TextureLoader(Context context) {
		amgr = context.getAssets();
	}
	
	public TextureInfo loadTexture(String fileName) {
        try {
            Bitmap bitmap = BitmapFactory.decodeStream(amgr.open(fileName));
            int textureIds[] = new int[1];
            GLES10.glGenTextures(1, textureIds, 0);
            int textureId = textureIds[0];
            GLES10.glBindTexture(GLES10.GL_TEXTURE_2D, textureId);    
            GLUtils.texImage2D(GLES10.GL_TEXTURE_2D, 0, bitmap, 0);
            GLES10.glTexParameterf(GLES10.GL_TEXTURE_2D, GLES10.GL_TEXTURE_MIN_FILTER, GLES10.GL_LINEAR);
            GLES10.glTexParameterf(GLES10.GL_TEXTURE_2D, GLES10.GL_TEXTURE_MAG_FILTER, GLES10.GL_LINEAR);                
            GLES10.glBindTexture(GLES10.GL_TEXTURE_2D, 0);
            bitmap.recycle();
            TextureInfo result = new TextureInfo();
            result.textureId = textureId;
            result.width = bitmap.getWidth();
            result.height = bitmap.getHeight();
            return result;
        } catch(IOException e) {
            Log.d("Asteroids", "couldn't load asset '" + fileName + "'!");
            throw new RuntimeException("couldn't load asset '" + fileName + "'");
        }
    }
}

class TextureInfo {
	int textureId;
	int width, height;
}

class VibratorHelper {
	private Vibrator vibratorService;
	
	public VibratorHelper(Context context) {
		vibratorService = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
	}
	
	public void vibrate(int millis) {
		vibratorService.vibrate(millis);
	}
}
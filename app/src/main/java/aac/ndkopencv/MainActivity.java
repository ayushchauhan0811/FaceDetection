package aac.ndkopencv;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

public class MainActivity extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2{

    private static String TAG = "NDKopenCV";
    private JavaCameraView javaCameraView;

    static {
        System.loadLibrary("MyOpencvLibs");
    }

    private BaseLoaderCallback baseLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status){
                case BaseLoaderCallback.SUCCESS :
                    javaCameraView.enableView();
                    break;
                default :
                    super.onManagerConnected(status);
                    break;
            }
        }
    };

    private Mat rgba, gray;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        javaCameraView = (JavaCameraView) findViewById(R.id.java_camera_view);
        javaCameraView.setVisibility(View.VISIBLE);
        javaCameraView.setCvCameraViewListener(this);
    }

    @Override
    public void onCameraViewStarted(int width, int height) {
        rgba = new Mat(height,width, CvType.CV_8UC4);
        //gray = new Mat(height,width, CvType.CV_8UC1);
    }

    @Override
    public void onCameraViewStopped() {
        rgba.release();
    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        rgba = inputFrame.rgba();

        //OpencvNativeClass.convertGray(rgba.getNativeObjAddr(),gray.getNativeObjAddr());
        OpencvNativeClass.faceDetection(rgba.getNativeObjAddr());
        return rgba;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if(javaCameraView != null)
            javaCameraView.disableView();
    }

    @Override
    protected void onPause() {
        super.onPause();

        if(javaCameraView != null)
            javaCameraView.disableView();
    }

    @Override
    protected void onResume() {
        super.onResume();

        if(OpenCVLoader.initDebug()){
            Log.i(TAG,"OpenCV loaded successfully");
            baseLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        } else {
            Log.i(TAG,"OpenCV not loaded");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_2_0,this,baseLoaderCallback);
        }
    }
}

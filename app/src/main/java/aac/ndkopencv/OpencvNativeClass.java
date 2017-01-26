package aac.ndkopencv;

/**
 * Created by Ayush on 26-01-2017.
 */

public class OpencvNativeClass {
    public native static int convertGray(long matAddrRgba, long matAddrGray);
    public native static void faceDetection(long matAddrRgba);
}

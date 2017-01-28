#include <aac_ndkopencv_OpencvNativeClass.h>
#include <android/log.h>

#define APPNAME "NDKopenCV"


JNIEXPORT jint JNICALL Java_aac_ndkopencv_OpencvNativeClass_convertGray
  (JNIEnv *, jclass, jlong addrRgba, jlong addrGray){
    Mat& mrgb = *(Mat*) addrRgba;
    Mat& mgray = *(Mat*) addrGray;

    int conv;
    jint retVal;
    conv = toGray(mrgb,mgray);

    retVal = (jint) conv;

    return retVal;

  }

JNIEXPORT void JNICALL Java_aac_ndkopencv_OpencvNativeClass_faceDetection
  (JNIEnv *, jclass, jlong addrRgba){
    Mat& frame = *(Mat*) addrRgba;
    detect(frame);
  }

void detect(Mat& frame){
    String face_cascade_name = "/storage/emulated/0/data/lbpcascade_frontalface.xml";
    CascadeClassifier face_cascade;

    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return ; };
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(80, 80) );
    for( size_t i = 0; i < faces.size(); i++ ) {
        rectangle(frame, faces[i].tl(), faces[i].br(), Scalar(0, 255, 0, 255),3,LINE_8,0);
    }
}

int toGray(Mat img, Mat& gray){
    cvtColor(img, gray, CV_RGBA2GRAY);

    if(gray.rows == img.rows && gray.cols == img.cols)
        return 1;
    else
        return 0;
}
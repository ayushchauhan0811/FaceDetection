#include <aac_ndkopencv_OpencvNativeClass.h>

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

int detect(Mat& frame){
    String face_cascade_name = "haarcascade_frontalface_alt.xml";
    String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;

    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return ; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return ; };

    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t i = 0; i < faces.size(); i++ ) {
        Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

        for( size_t j = 0; j < eyes.size(); j++ ) {
            Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
    }
}

int toGray(Mat img, Mat& gray){
    cvtColor(img, gray, CV_RGBA2GRAY);

    if(gray.rows == img.rows && gray.cols == img.cols)
        return 1;
    else
        return 0;
}
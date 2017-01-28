#include <aac_ndkopencv_DetectionBasedTracker.h>
#include <android/log.h>
#include <string>
#include <vector>

#define APPNAME "NDKopenCV"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, APPNAME, __VA_ARGS__))

inline void vector_Rect_to_Mat(vector<Rect>& v_rect, Mat& mat) {
  mat = Mat(v_rect, true);
}

class CascadeDetectorAdapter: public DetectionBasedTracker::IDetector {
public:
    CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector): IDetector(), Detector(detector) {
      CV_Assert(detector);
    }

    void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects) {
      LOGD("CascadeDetectorAdapter::Detect: scaleFactor=%.2f, minNeighbours=%d, minObjSize=(%dx%d), maxObjSize=(%dx%d)",
           scaleFactor, minNeighbours, minObjSize.width, minObjSize.height, maxObjSize.width, maxObjSize.height);
      Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
    }

    virtual ~CascadeDetectorAdapter() {
    }

private:
    CascadeDetectorAdapter();
    cv::Ptr<cv::CascadeClassifier> Detector;
};

struct DetectorAgregator {
    cv::Ptr<CascadeDetectorAdapter> mainDetector;
    cv::Ptr<CascadeDetectorAdapter> trackingDetector;
    cv::Ptr<DetectionBasedTracker> tracker;
    DetectorAgregator(cv::Ptr<CascadeDetectorAdapter>& _mainDetector, cv::Ptr<CascadeDetectorAdapter>& _trackingDetector):
            mainDetector(_mainDetector), trackingDetector(_trackingDetector) {
      CV_Assert(_mainDetector);
      CV_Assert(_trackingDetector);
      DetectionBasedTracker::Parameters DetectorParams;
      tracker = makePtr<DetectionBasedTracker>(mainDetector, trackingDetector, DetectorParams);
    }
};



/*
 * Class:     aac_ndkopencv_DetectionBasedTracker
 * Method:    nativeCreateObject
 * Signature: (Ljava/lang/String;I)J
 */
JNIEXPORT jlong JNICALL Java_aac_ndkopencv_DetectionBasedTracker_nativeCreateObject
  (JNIEnv *jenv, jclass, jstring jFileName, jint faceSize){

  const char* jnamestr = jenv->GetStringUTFChars(jFileName, NULL);
  string stdFileName(jnamestr);
  jlong result = 0;

  try {
    cv::Ptr<CascadeDetectorAdapter> mainDetector = makePtr<CascadeDetectorAdapter>(
            makePtr<CascadeClassifier>(stdFileName));
    cv::Ptr<CascadeDetectorAdapter> trackingDetector = makePtr<CascadeDetectorAdapter>(
            makePtr<CascadeClassifier>(stdFileName));
    result = (jlong)new DetectorAgregator(mainDetector, trackingDetector);
    if (faceSize > 0) {
      mainDetector->setMinObjectSize(Size(faceSize, faceSize));
      //trackingDetector->setMinObjectSize(Size(faceSize, faceSize));
    }
  }
  catch(cv::Exception& e) {
    jclass je = jenv->FindClass("org/opencv/core/CvException");
    if(!je)
      je = jenv->FindClass("java/lang/Exception");
    jenv->ThrowNew(je, e.what());
  }
  catch (...) {
    jclass je = jenv->FindClass("java/lang/Exception");
    jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeCreateObject()");
    return 0;
  }

  return result;
}

/*
 * Class:     aac_ndkopencv_DetectionBasedTracker
 * Method:    nativeDestroyObject
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_aac_ndkopencv_DetectionBasedTracker_nativeDestroyObject
  (JNIEnv *jenv, jclass, jlong thiz){

    try{
      if(thiz != 0){
      ((DetectorAgregator*)thiz)->tracker->stop();
      delete (DetectorAgregator*)thiz;
      }
    }
    catch(cv::Exception& e){
      jclass je = jenv->FindClass("org/opencv/core/CvException");
      if(!je)
      je = jenv->FindClass("java/lang/Exception");
      jenv->ThrowNew(je, e.what());
    }
    catch (...){
      jclass je = jenv->FindClass("java/lang/Exception");
      jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeDestroyObject()");
    }
  }

/*
 * Class:     aac_ndkopencv_DetectionBasedTracker
 * Method:    nativeStart
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_aac_ndkopencv_DetectionBasedTracker_nativeStart
(JNIEnv *jenv, jclass, jlong thiz){

  try{
    ((DetectorAgregator*)thiz)->tracker->run();
  }
  catch(cv::Exception& e){
    jclass je = jenv->FindClass("org/opencv/core/CvException");
    if(!je)
      je = jenv->FindClass("java/lang/Exception");
      jenv->ThrowNew(je, e.what());
  }
  catch (...){
    jclass je = jenv->FindClass("java/lang/Exception");
    jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeStart()");
  }
}

/*
 * Class:     aac_ndkopencv_DetectionBasedTracker
 * Method:    nativeStop
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_aac_ndkopencv_DetectionBasedTracker_nativeStop
  (JNIEnv *jenv, jclass, jlong thiz){
    try{
      ((DetectorAgregator*)thiz)->tracker->stop();
    }
    catch(cv::Exception& e){
      jclass je = jenv->FindClass("org/opencv/core/CvException");
      if(!je)
        je = jenv->FindClass("java/lang/Exception");
      jenv->ThrowNew(je, e.what());
    }
    catch (...){
      jclass je = jenv->FindClass("java/lang/Exception");
      jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeStop()");
    }
  }

/*
 * Class:     aac_ndkopencv_DetectionBasedTracker
 * Method:    nativeSetFaceSize
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_aac_ndkopencv_DetectionBasedTracker_nativeSetFaceSize
  (JNIEnv *jenv, jclass, jlong thiz, jint faceSize){
    try{
      if (faceSize > 0){
        ((DetectorAgregator*)thiz)->mainDetector->setMinObjectSize(Size(faceSize, faceSize));
        //((DetectorAgregator*)thiz)->trackingDetector->setMinObjectSize(Size(faceSize, faceSize));
      }
    }
    catch(cv::Exception& e){
      jclass je = jenv->FindClass("org/opencv/core/CvException");
      if(!je)
        je = jenv->FindClass("java/lang/Exception");
      jenv->ThrowNew(je, e.what());
    }
    catch (...){
      jclass je = jenv->FindClass("java/lang/Exception");
      jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeSetFaceSize()");
    }
  }

/*
 * Class:     aac_ndkopencv_DetectionBasedTracker
 * Method:    nativeDetect
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_aac_ndkopencv_DetectionBasedTracker_nativeDetect
  (JNIEnv *jenv, jclass, jlong thiz, jlong imageGray, jlong faces){
    try{
      vector<Rect> RectFaces;
      ((DetectorAgregator*)thiz)->tracker->process(*((Mat*)imageGray));
      ((DetectorAgregator*)thiz)->tracker->getObjects(RectFaces);
      *((Mat*)faces) = Mat(RectFaces, true);
    }
    catch(cv::Exception& e){
      jclass je = jenv->FindClass("org/opencv/core/CvException");
      if(!je)
        je = jenv->FindClass("java/lang/Exception");
      jenv->ThrowNew(je, e.what());
    }
    catch (...){
      jclass je = jenv->FindClass("java/lang/Exception");
      jenv->ThrowNew(je, "Unknown exception in JNI code DetectionBasedTracker.nativeDetect()");
    }
  }

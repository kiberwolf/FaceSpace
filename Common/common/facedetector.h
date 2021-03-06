#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cmath>

using namespace std;
using namespace cv;

class FaceDetector {

private:
    const string FACE_CASSCADES = "../Common/common/haarcascade_frontalface_default.xml";
    const string EYE_CASSCADES = "../Common/common/haarcascade_eye_tree_eyeglasses.xml";

    struct EyeInfo;

    CascadeClassifier face_cascade;
    CascadeClassifier eye_cascade;
    bool isCascadesInitialized = false;

    inline long double getEyeRotateAngel(Point2f &p1, Point2f &p2);
    inline long double sqr(long double val);
    inline void overlayImage(const Mat &background, const Mat &foreground, Mat &output, cv::Point2i location);
    void find_scc(int v, bool was[], vector<int> graph[], vector<int> &path);
public:
    FaceDetector();
    
    const enum DetectMode { CheckHasFace, FindAllFaces };
    
    static Rect scaleRect(Rect &r, double scale);
    static Rect scaleRectSize(Rect &rect, double scale, int limitX, int limitY);
    vector<Rect> detect(Mat& originalFrame, DetectMode MODE);
};
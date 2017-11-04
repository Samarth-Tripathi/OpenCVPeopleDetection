#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
//#include "openCVUtils.h"

using namespace cv;
using namespace std;



static Mat _inImage;
static Mat _outImage;
static Mat _originalImage;

static    HOGDescriptor _hog;
static const char* _windowName = "people detector";
static float _hitThreshold = 0;
static float _scaleFactor = 1.059;
static float _groupThreshold = 0.0;




static int imshowWithCaption(const char* windowName, Mat inImage,  const char* caption )
{
    putText( inImage, caption,
            Point( MIN(10,inImage.cols), MIN(20,inImage.rows)),
            FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255) );
    
    imshow( windowName, inImage );
    char wtk = (char) cv::waitKey(0);
    
    if( wtk == 121) {
        cout << "person detected correctly, removing it. " << endl;
        return 1;
    } else if (wtk == 110) {
        cout << "person detected incorrectly, not removing it. " << endl;
        return 0;
    }
    
    return 0;
}

static void hogDetect () {
    _inImage.copyTo(_outImage);
    vector<Rect> found;
    vector <Rect> found_filtered;

    _hog.detectMultiScale(_inImage, found , _hitThreshold , Size(8,8), Size(32,32) , _scaleFactor, _groupThreshold);
    
    size_t i, j;
    

    for( i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];
        for( j = 0; j < found.size(); j++ ) {
            //filter out overlapping rectangles
            if ( j!=i ) {
                Rect iRect =  r;
                Rect jRect = found[j];
                Rect intersectRect = (iRect & jRect);
                if (intersectRect.area()>=iRect.area()*0.9) break;
            }
        }
        if( j == found.size() )
            found_filtered.push_back(r);
    }
    bool human_confirm;
    cout<<"People Found = "<<found_filtered.size()<<endl;
    for( i = 0; i < found_filtered.size(); i++ )
    {
        _outImage.copyTo(_originalImage);
        Rect r = found_filtered[i];
        // the HOG detector returns slightly larger rectangles than the real objects.
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.5);
        // hacky shift right by 40px - rects seem to be shifted consistently
        r.x +=-40;
        r.width = cvRound(r.width*0.3);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(_outImage, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
        human_confirm = imshowWithCaption(_windowName, _outImage, "");
        
        if (human_confirm) {
            _originalImage.copyTo(_outImage);
            rectangle(_outImage, r.tl(), r.br(), cv::Scalar(0,0,0), -1);
        } else {
           _originalImage.copyTo(_outImage);
        }
        
    }

    
    imshowWithCaption(_windowName, _outImage, "Final Image! ");
    imwrite("output.jpg", _outImage);
    
}



static void setupHogWindow() {
    
    namedWindow( _windowName, WINDOW_AUTOSIZE );
    
}

int main() {
    
    string filenamestr;
    cin>>filenamestr;
    //string filenamestr = "model4.jpg";
    cout<<"Loading Image"<<endl;
    _inImage = imread(filenamestr);
    if (_inImage.empty())
    {
        cout<<"Could Not load Image"<<endl;
        return 0;
    }
    _hog = HOGDescriptor::HOGDescriptor();
    _hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    namedWindow(_windowName, 1);
    setupHogWindow();
    hogDetect();
    
    
    return 0;
}

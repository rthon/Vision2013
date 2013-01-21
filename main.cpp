#include <cstdlib>
#include <iostream>
using namespace std;
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "Team751FreenectDevice.h"

int main(int argc, char** argv) {

    cout << "it works!\n";

        Freenect::Freenect freenect;
        if(freenect.deviceCount() < 1) {
            cerr << "No Kinects connected!\n";
            return EXIT_FAILURE;
        }
        cout << "About to initialize Kinect...";
        Team751FreenectDevice* kinect = & freenect.createDevice<Team751FreenectDevice>(0);
        cout << "Initialization successful\n";
        kinect->setTiltDegrees(0);
        kinect->setLed(LED_RED);
        kinect->startVideo();

    //allocate Mats to be used in the loop
    cv::Mat video;
    cv::Mat green;
    cv::Mat saturatedGreen(cv::Size(FREENECT_FRAME_W, FREENECT_FRAME_H), CV_8UC1, cv::Scalar(0));
    cv::Mat binary;

    //Set up windows
    cv::namedWindow("video");
    cv::namedWindow("green");
    cv::namedWindow("binary");
    
    cv::namedWindow("hull");
    
    //The mask used to multiply the green - red - blue image to make it brighter
    const cv::Mat greenMask(cv::Size(FREENECT_FRAME_W, FREENECT_FRAME_H), CV_8UC1, cv::Scalar(100, 100, 100));

    while (true) {


                if(kinect->getVideo(video)) {
                    
                    //Split into blue, green, and red channels
                    cv::Mat channels[3];
                    cv::split(video, channels);
                    
                    cv::imshow("video", video);
                    
                    green = channels[1];
                    
                    green -= 0.5 * (channels[0] + channels[2]);
                    //Enhance the contrast of the green (grayscale) image
                    const double contrastFactor = 100;
                    for(int y = 0; y < green.rows; y++ ) {
                        for(int x = 0; x < green.cols; x++) {
                            saturatedGreen.at<uint8_t>(y, x) = cv::saturate_cast<uint8_t>(green.at<uint8_t>(y, x) * contrastFactor);
                        }
                    }
                    
                    cv::imshow("green", saturatedGreen);
                    
                    cv::threshold(saturatedGreen, binary, 1, 255, cv::THRESH_BINARY);
                    
                    
                    cv::imshow("binary", binary);
                    
                    vector<vector<cv::Point> > contours;
                    vector<cv::Vec4i> hierarchy;
                    cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
                    
                    //Find the convex hull object for each contour
                    vector<vector<cv::Point> > hull (contours.size());
                    for(int i = 0; i < contours.size(); i++) {
                        cv::convexHull(cv::Mat(contours[i]), hull[i], false);
                    }
                    
                    cout << "-----------------------------------" << endl;
                    
                    //Simplify them
                    vector<vector<cv::Point> >polygons(hull.size());
                    for(int i = 0; i < hull.size(); i++) {
                        
                        vector<cv::Point> polygon = hull[i];
                        double length = cv::arcLength(cv::Mat(polygon), true);

                        //Discard anything with a length of less than a certain value
                        if(length < 100) continue;
                        
                        cout << polygon.size() << endl;
                        
                        cv::approxPolyDP(hull[i], polygons[i], 0.2, true);
                    }
                    
                    //Draw contours/hull results
                    cv::Mat drawing = cv::Mat::zeros( binary.size(), CV_8UC3);
                    
                    for(int i = 0; i < contours.size(); i++) {
                        cv::Scalar color(0, 255, 0);// = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
                        //cv::drawContours(drawing, contours, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
                        cv::drawContours(drawing, polygons, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
                    }
                    cv::imshow("hull", drawing);
    }

    

    if (cv::waitKey(10) != -1) {
        break;
    }
    }

    kinect->stopVideo();
    kinect->setLed(LED_GREEN);

    delete kinect;

return 0;
}


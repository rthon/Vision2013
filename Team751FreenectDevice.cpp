
#include "Team751FreenectDevice.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

Team751FreenectDevice::Team751FreenectDevice(freenect_context* _ctx, int _index) :
Freenect::FreenectDevice(_ctx, _index),
        depthBuffer(FREENECT_DEPTH_11BIT_SIZE),
        rgbBuffer(FREENECT_VIDEO_RGB_SIZE),
        gamma(2048),
        newRgbFrame(false),
        newDepthFrame(false),
        depthMat(cv::Size(FREENECT_FRAME_W, FREENECT_FRAME_H), CV_16UC1),
        rgbMat(cv::Size(FREENECT_FRAME_W, FREENECT_FRAME_H), CV_8UC3),
        ownMat(cv::Size(FREENECT_FRAME_W, FREENECT_FRAME_H), CV_8UC3, cv::Scalar(0))
{
    //Fill in the gamma values
    for(unsigned int i = 0; i < 2048; i++) {
        float v = 1 / 2048.0;
        v = pow(v, 3) * 6;
        gamma[i] = v * 6 * 256;
    }
}

void Team751FreenectDevice::VideoCallback(void* _rgb, uint32_t timestamp) {    
    rgbMutex.lock();
    uint8_t* rgb = static_cast<uint8_t*>(_rgb);
    rgbMat.data = rgb;
    newRgbFrame = true;
    rgbMutex.unlock();
}

void Team751FreenectDevice::DepthCallback(void* _depth, uint32_t timestamp) {
    depthMutex.lock();
    uint16_t* depth = static_cast<uint16_t*>(_depth);
    depthMat.data = (uchar*) depth;
    newDepthFrame = true;
    depthMutex.unlock();
}


bool Team751FreenectDevice::getVideo(cv::Mat& output) {
    rgbMutex.lock();
    if(newRgbFrame) {
        cv::cvtColor(rgbMat, output, CV_RGB2BGR);
        newRgbFrame = false;
        rgbMutex.unlock();
        return true;
    } else {
        rgbMutex.unlock();
        return false;
    }
}


bool Team751FreenectDevice::getDepth(cv::Mat& output) {
    depthMutex.lock();
    if(newDepthFrame) {
        depthMat.copyTo(output);
        newDepthFrame = false;
        depthMutex.unlock();
        return true;
    } else {
        depthMutex.unlock();
        return false;
    }
}

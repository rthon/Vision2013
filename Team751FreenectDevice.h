#ifndef TEAM751FREENECTDEVICE_H
#define	TEAM751FREENECTDEVICE_H

#include <libfreenect.hpp>
#include <libfreenect/libfreenect.h>

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>

#include <vector>
using namespace std;

#include "PThreadMutex.h"

//Definitions of various sizes
#define FREENECT_FRAME_W 640
#define FREENECT_FRAME_H 480
#define FREENECT_FRAME_PIX (FREENECT_FRAME_W * FREENECT_FRAME_H)
#define FREENECT_VIDEO_RGB_SIZE (FREENECT_FRAME_PIX * 3)
#define FREENECT_DEPTH_11BIT_SIZE (FREENECT_FRAME_PIX * sizeof(uint16_t))

class Team751FreenectDevice : public Freenect::FreenectDevice {
public:
    Team751FreenectDevice(freenect_context* _ctx, int _index);
    
    void VideoCallback(void* _rgb, uint32_t timestamp);
    
    void DepthCallback(void* _depth, uint32_t timestamp);
    
    /**
     * Copy the video data into a Mat
     * @param output The Mat to copy to
     * @return true if a new frame was provided, otherwise false
     */
    bool getVideo(cv::Mat& output);
    
    bool getDepth(cv::Mat& output);
    
private:

    vector<uint8_t> depthBuffer;
    vector<uint8_t> rgbBuffer;
    vector<uint16_t> gamma;
    cv::Mat depthMat;
    cv::Mat rgbMat;
    cv::Mat ownMat;
    PThreadMutex rgbMutex;
    PThreadMutex depthMutex;
    bool newRgbFrame;
    bool newDepthFrame;
    
};

#endif	/* TEAM751FREENECTDEVICE_H */


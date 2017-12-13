#ifndef SURF_H
#define SURF_H

#include <stdio.h>
#include <iostream>
#include <cv.h>
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>  
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>  



using namespace cv;
using namespace std;

#ifdef _DEBUG
#pragma comment (lib, "opencv_calib3d2410d.lib")
#pragma comment (lib, "opencv_contrib2410d.lib")
#pragma comment (lib,"opencv_imgproc2410d.lib")
#pragma comment (lib, "opencv_core2410d.lib")
#pragma comment (lib, "opencv_features2d2410d.lib")
#pragma comment (lib, "opencv_flann2410d.lib")
#pragma comment (lib, "opencv_gpu2410d.lib")
#pragma comment (lib, "opencv_highgui2410d.lib")
#pragma comment (lib, "opencv_legacy2410d.lib")
#pragma comment (lib, "opencv_ml2410d.lib")
#pragma comment (lib, "opencv_objdetect2410d.lib")
#pragma comment (lib, "opencv_ts2410d.lib")
#pragma comment (lib, "opencv_video2410d.lib")
#pragma comment (lib, "opencv_nonfree2410d.lib")
#else
#pragma comment (lib, "opencv_calib3d2410.lib")
#pragma comment (lib, "opencv_contrib2410.lib")
#pragma comment (lib, "opencv_imgproc2410.lib")
#pragma comment (lib, "opencv_core2410.lib")
#pragma comment (lib, "opencv_features2d2410.lib")
#pragma comment (lib, "opencv_flann2410.lib")
#pragma comment (lib, "opencv_gpu2410.lib")
#pragma comment (lib, "opencv_highgui2410.lib")
#pragma comment (lib, "opencv_legacy2410.lib")
#pragma comment (lib, "opencv_ml2410.lib")
#pragma comment (lib, "opencv_objdetect2410.lib")
#pragma comment (lib, "opencv_ts2410.lib")
#pragma comment (lib, "opencv_video2410.lib")
#pragma comment (lib, "opencv_nonfree2410.lib")
#endif













#endif
/************************************************************************/
/* surf处理相关的函数                          */
/************************************************************************/

#ifndef PROCESS_WITH_SURF
#define PROCESS_WITH_SURF
/******       QT 的头文件  *******/
#include <QtGui/QMainWindow>
#include "ui_sy_2.h"
#include <qstring.h>
#include <QtGui/QMessageBox>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QUuid>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <QTextCodec>///解决qt中文乱码时需要添加的头文件

/*****  opencv的头文件  *****/
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/legacy/compat.hpp>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

/***  VC 的头文件 ***/
#include <vector>

/*****  SURF算法头文件 *****/
#include "SURF.h"

/*****使用CSrting时需要用到的头文件*****/
#include <atlstr.h>
#include "ximage.h"

//特征匹配类型--不懂具体意思
enum feature_match_type
{
	FEATURE_FWD_MATCH,
	FEATURE_BCK_MATCH,
	FEATURE_MDL_MATCH,
};

#define RANSAC_PROB_BAD_SUPP 0.10


void findPairs3( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
	const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs );

int naiveNearestNeighbor3( const float* vec, int laplacian,
	const CvSeq* model_keypoints,
	const CvSeq* modle_descriptors );

double compareSURFDescriptors3( const float* d1, const float* d2, double best, int length);


CvMat* GetH3(vector<int>& ptpairs,CvSeq *CurrentKeypoints,CvSeq *PreKeypoints);

CvMat* ransac_xform3( vector<int>& ptpairs,CvSeq *CurrentKeypoints,CvSeq *PreKeypoints, int mtype, 
	int m, double p_badxform, double err_tol);

static int calc_min_inliers3( int n, int m, double p_badsupp, double p_badxform );
static	void draw_ransac_sample3( vector<int> ptpairs,vector<int>& sample_ptpairs);
static void extract_corresp_pts3( vector<int> ptpairs,CvSeq *CurrentKeypoints,CvSeq *PreKeypoints,int m, int mtype,
	CvPoint2D64f** pts, CvPoint2D64f** mpts );

static int find_consensus3( vector<int> ptpairs,CvSeq *CurrentKeypoints,CvSeq *PreKeypoints, int mtype,
	CvMat* M, double err_tol,vector<int>& inliers_ptpairs);

static	double homog_xfer_err3( CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* H );


static	CvPoint2D64f persp_xform_pt4( CvPoint2D64f pt, CvMat* T );

static CvMat* lsq_homog3( CvPoint2D64f* pts, CvPoint2D64f* mpts, int n );


double gsl_sf_choose3(unsigned int n, unsigned int m);

static __inline void release_mem3( CvPoint2D64f* pts1, CvPoint2D64f* pts2);

IplImage* convert_to_gray8( IplImage* img );

const char * cst_to_conchar( CString str );///CString 转换为const char*函数

//计算SURF算法精度的函数
double GetRMSE3(vector<int> ptpairs,CvSeq* CurrentKeypoints,CvSeq* PreKeypoints,CvMat* H);

CvPoint2D32f persp_xform_pt3( CvPoint2D32f pt, CvMat* T );

bool ColorSplit(CxImage m_img);
bool CuveSplit(CxImage m_img);
bool ShapeSplit(CxImage m_img);
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma, double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge);
void GaussianSmooth(unsigned char *pUnchImg, int nWidth, int nHeight, 
	double sigma, unsigned char * pUnchSmthdImg);

void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
	int nHeight, unsigned char *pUnchRst);
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth) ;

void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag);
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
	double dRatioHigh, unsigned char *pUnchEdge);
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize);
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
	unsigned char * pUnchEdge, double dRatioHigh, double dRationLow);
void DirGrad(unsigned char *pUnchSmthdImg, int nWidth, int nHeight,
	int *pnGradX , int *pnGradY);
#endif
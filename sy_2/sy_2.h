/************************************************************************/
/* 定义与界面有关的函数、类及变量                                      */
/************************************************************************/

#ifndef SY_2_H
#define SY_2_H

/*****  ArcGIS 的头文件  *****/
#include <ArcSDK.h>


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
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

/***  VC 的头文件 ***/
#include <vector>

/*****  SURF算法头文件 *****/
#include "SURF.h"

/*****使用CSrting时需要用到的头文件*****/
#include <atlstr.h>


/***自己编写的头文件***/
#include "Pyramid_with_GDAL.h"
#include "process.h"
#include "process_with_surf.h"


class sy_2 : public QMainWindow
{
	Q_OBJECT

public:
	sy_2(QWidget *parent = 0, Qt::WFlags flags = 0);
	~sy_2();

private:
	Ui::sy_2Class ui;


public:
	//用于图像配准的三个Map控件变量
	IMapControl3Ptr m_pMapControl1;
	IToolbarControlPtr m_pToolbarControl1;

	IMapControl3Ptr m_pMapControl2;
	IToolbarControlPtr m_pToolbarControl2;

	//	IMapControl3Ptr m_pMapControl3;
	//	IToolbarControlPtr m_pToolbarControl3;

	IMapControl3Ptr m_pMapControl4;
	IToolbarControlPtr m_pToolbarControl4;

	ITOCControlPtr m_pTOCControl1;
	ITOCControlPtr m_pTOCControl2;

public:
	//控件工具条设置函数
	void AddMapToolBarCtr(IToolbarControlPtr pToolbar);//map


public slots:
	void OpenFile1();
	void OpenFile2();
	//void Openbigresult();
	void OpenResultFile1(string DstImagePath);
	void BigMapRegistration(); ///大图配准
	void OnClearMapLayer();//该函数用于清除Map控件中的图像，以便于显示其他图像
	void AdBigMapRegistration();
	void OnSpeed();//分块搜索算法
	void OnSurfSpeed();//小图像单纯SURF算法
private:
	QAction *openAction1,*openAction2;
	QAction *SurfAction;
	QAction *DeleMapLayersAction;
	QAction *BigMapSURFAction;
	QAction *SImageSpeed;//分块搜索算法
	QAction *SImageSurfSpeed;//小图像单纯SURF算法

//	QAction *openBigMapResult;


};

void Cut_Count_Overlap( int ilayer );  //通过小图匹配获取两图重叠区域，并对重叠区域进行分块统计特征点数量。

void ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY,const char* pszFormat);////对图像进行裁切

//void BigMapRegistration(); ///大图配准

void SURF_ON_TWO(string SrcReImagePath, string SrcSenImagePath, string DstImagePath );

void Find_OverlapArea ( int ilayer );


#endif // SY_2_H

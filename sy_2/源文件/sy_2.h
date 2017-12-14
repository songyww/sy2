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
#include <QFile> //使用QDir定义时需要

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

#include "ximage.h"



class sy_2 : public QMainWindow
{
	Q_OBJECT

public:
	sy_2(QWidget *parent = 0, Qt::WFlags flags = 0);
	~sy_2();

private:
	Ui::sy_2Class ui;

private slots:
	//void open1();
	bool OpenFile1();
	bool OpenFile2();
	bool OpenFile3();
	bool OpenFile4();

	//void Openbigresult();
	void OpenResultFile1(string DstImagePath);
	void BigMapRegistration(); ///大图配准
	void OnClearMapLayer();//该函数用于清除Map控件中的图像，以便于显示其他图像
	void AdBigMapRegistration();

	void DSurfSpeed();//降采样+SURF算法
	void FenKuai_DSURF();//降采样+分块+SURF算法
	void FenKuai_SURF();//分块+SURF算法,不包括降采样的分块算法
	void OpenResultFile2(string DstImagePath);
	void on_down_rate(const QString &text);
	//void on_click_down();
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
	ITOCControlPtr m_pTOCControl4;
	//用于图像配准的三个Map控件变量
	IMapControl3Ptr m_pMapControl1_2;
	IToolbarControlPtr m_pToolbarControl1_2;

	IMapControl3Ptr m_pMapControl2_2;
	IToolbarControlPtr m_pToolbarControl2_2;

	//	IMapControl3Ptr m_pMapControl3;
	//	IToolbarControlPtr m_pToolbarControl3;

	IMapControl3Ptr m_pMapControl4_2;
	IToolbarControlPtr m_pToolbarControl4_2;

	ITOCControlPtr m_pTOCControl1_2;
	ITOCControlPtr m_pTOCControl2_2;
	ITOCControlPtr m_pTOCControl4_2;
public:
	//控件工具条设置函数
	void AddMapToolBarCtr(IToolbarControlPtr pToolbar);//map


private:
	QAction *openAction1,*openAction2;
	QAction *SurfAction;
	QAction *DeleMapLayersAction;
	QAction *BigMapSURFAction;
	QAction *DSFenKuaiAction;//分块搜索算法
	QAction *DSurfAction;//小图像单纯SURF算法


//	QAction *openBigMapResult;


};



void Cut_Count_Overlap( int ilayer );  //通过小图匹配获取两图重叠区域，并对重叠区域进行分块统计特征点数量。

void ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY,const char* pszFormat);////对图像进行裁切

//void BigMapRegistration(); ///大图配准

void SURF_ON_TWO(string SrcReImagePath, string SrcSenImagePath, string DstImagePath );
void SURF_ON_TWO1(string SrcReImagePath, string SrcSenImagePath, string DstImagePath );
void Find_OverlapArea ( int ilayer );
void CariRecursive(QString strInitPath, QString strFileName);  //暗号
double Madhosi(QString strNameFile); //获取图像特征值,并比较   暗号
void ResizeImage( const char* scrimg,const char* dstimg, double scale );//对原始图像进行降采样；scale 为降采样比例，降采样后尺寸/原图尺寸
void Fenkuai_Registration_Big();
void Fenkuai_Registration_Small();
bool remove_directory(const std::string& dir);


#endif // SY_2_H

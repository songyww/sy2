/************************************************************************/
/* 定义与界面有关的函数、类及变量                                      */
/************************************************************************/

#ifndef SY_2_H
#define SY_2_H

/*****  ArcGIS 的头文件  *****/
#include <ArcSDK.h>

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
	void Openbigresult();
	void OpenResultFile1(string DstImagePath);
	void BigMapRegistration(); ///大图配准




};

void Cut_Count_Overlap( int ilayer );  //通过小图匹配获取两图重叠区域，并对重叠区域进行分块统计特征点数量。

void ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY,const char* pszFormat);////对图像进行裁切

void BigMapRegistration(); ///大图配准

void SURF_ON_TWO(string SrcReImagePath, string SrcSenImagePath, string DstImagePath );




#endif // SY_2_H

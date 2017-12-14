/************************************************************************/
/* ����������йصĺ������༰����                                      */
/************************************************************************/

#ifndef SY_2_H
#define SY_2_H

/*****  ArcGIS ��ͷ�ļ�  *****/
#include <ArcSDK.h>


/******       QT ��ͷ�ļ�  *******/
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
#include <QTextCodec>///���qt��������ʱ��Ҫ��ӵ�ͷ�ļ�
#include <QFile> //ʹ��QDir����ʱ��Ҫ

/*****  opencv��ͷ�ļ�  *****/
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

/***  VC ��ͷ�ļ� ***/
#include <vector>

/*****  SURF�㷨ͷ�ļ� *****/
#include "SURF.h"

/*****ʹ��CSrtingʱ��Ҫ�õ���ͷ�ļ�*****/
#include <atlstr.h>


/***�Լ���д��ͷ�ļ�***/
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
	void BigMapRegistration(); ///��ͼ��׼
	void OnClearMapLayer();//�ú����������Map�ؼ��е�ͼ���Ա�����ʾ����ͼ��
	void AdBigMapRegistration();

	void DSurfSpeed();//������+SURF�㷨
	void FenKuai_DSURF();//������+�ֿ�+SURF�㷨
	void FenKuai_SURF();//�ֿ�+SURF�㷨,�������������ķֿ��㷨
	void OpenResultFile2(string DstImagePath);
	void on_down_rate(const QString &text);
	//void on_click_down();
public:
	//����ͼ����׼������Map�ؼ�����
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
	//����ͼ����׼������Map�ؼ�����
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
	//�ؼ����������ú���
	void AddMapToolBarCtr(IToolbarControlPtr pToolbar);//map


private:
	QAction *openAction1,*openAction2;
	QAction *SurfAction;
	QAction *DeleMapLayersAction;
	QAction *BigMapSURFAction;
	QAction *DSFenKuaiAction;//�ֿ������㷨
	QAction *DSurfAction;//Сͼ�񵥴�SURF�㷨


//	QAction *openBigMapResult;


};



void Cut_Count_Overlap( int ilayer );  //ͨ��Сͼƥ���ȡ��ͼ�ص����򣬲����ص�������зֿ�ͳ��������������

void ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY,const char* pszFormat);////��ͼ����в���

//void BigMapRegistration(); ///��ͼ��׼

void SURF_ON_TWO(string SrcReImagePath, string SrcSenImagePath, string DstImagePath );
void SURF_ON_TWO1(string SrcReImagePath, string SrcSenImagePath, string DstImagePath );
void Find_OverlapArea ( int ilayer );
void CariRecursive(QString strInitPath, QString strFileName);  //����
double Madhosi(QString strNameFile); //��ȡͼ������ֵ,���Ƚ�   ����
void ResizeImage( const char* scrimg,const char* dstimg, double scale );//��ԭʼͼ����н�������scale Ϊ��������������������ߴ�/ԭͼ�ߴ�
void Fenkuai_Registration_Big();
void Fenkuai_Registration_Small();
bool remove_directory(const std::string& dir);


#endif // SY_2_H

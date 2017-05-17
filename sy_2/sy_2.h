/************************************************************************/
/* ����������йصĺ������༰����                                      */
/************************************************************************/

#ifndef SY_2_H
#define SY_2_H

/*****  ArcGIS ��ͷ�ļ�  *****/
#include <ArcSDK.h>

/*****ʹ��CSrtingʱ��Ҫ�õ���ͷ�ļ�*****/
#include <atlstr.h>

/***�Լ���д��ͷ�ļ�***/
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

public:
	//�ؼ����������ú���
	void AddMapToolBarCtr(IToolbarControlPtr pToolbar);//map


public slots:
	void OpenFile1();
	void OpenFile2();
	void Openbigresult();
	void OpenResultFile1(string DstImagePath);
	void BigMapRegistration(); ///��ͼ��׼




};

void Cut_Count_Overlap( int ilayer );  //ͨ��Сͼƥ���ȡ��ͼ�ص����򣬲����ص�������зֿ�ͳ��������������

void ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY,const char* pszFormat);////��ͼ����в���

void BigMapRegistration(); ///��ͼ��׼

void SURF_ON_TWO(string SrcReImagePath, string SrcSenImagePath, string DstImagePath );




#endif // SY_2_H

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

/*****ʹ��CSrtingʱ��Ҫ�õ���ͷ�ļ�*****/
#include <atlstr.h>

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



};

#endif // SY_2_H

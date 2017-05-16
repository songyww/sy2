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

/*****使用CSrting时需要用到的头文件*****/
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



};

#endif // SY_2_H

#include "sy_2.h"
#include <comutil.h>
#include <direct.h>  
#include <io.h>  
#include <Windows.h>  
#include <QUuid>
#include "ximage.h"

//******������׼ʱ��ͼ��·��*****///
vector<IplImage*> img;
vector<IplImage*> rfimg;//���Ա�������
vector<IplImage*> snimg;
//*** �����ʼ�����ͼ��·�� ***//
vector<string> refImg;
vector<string> senImg;
vector<string> resultImg;

/*** ����������ļ�·�� ***/
vector<string> refLayer;
vector<string> senLayer;
vector<string> refOverlap;
vector<string> senOverlap;
vector<string> fenImage;
//vector<char*> iImageSize;//refImageSize[0]=�ο�ͼ��ߴ�; refImageSize[1] = ����׼ͼ��ߴ�
//vector<char*> senImageSize;//senImageSize[0]=width; senImageSize[1] = height

//*** SURF�㷨����ص�ȫ�ֱ��� ***//
vector<CvPoint2D64f> rePoint;
vector<CvPoint2D64f> senPoint;
int num1;
int num2;
double mese1;//Сͼ��׼����
double mese2;//��ͼ��׼����
vector<CvMat*> H_1;///��ʱû���õ�
vector<CvMat*> H_2;///���Դ洢H����
CvSeq* objectKeypoints = 0, * objeceDescriptors = 0;  ///����ͼ��������㼰������Ϣ
CvSeq* imageKeypoints = 0, * imageDescriptors = 0;

vector<int> ptpairs_befores; //�洢��ѡ֮ǰ��ƥ����������
CvMat *OverLap1;
CvMat *OverLap2;

int FileOpenFlag1 = 0 ;//�ĸ�ͼ���С�ı�־��������ʾ����ʹ��ʲô�㷨����ʼֵΪ0��
int FileOpenFlag2 = 0 ;
int FileOpenFlag3 = 0 ;
int FileOpenFlag4 = 0 ;
using namespace std;
using namespace cv;
int iLayer1 = 0;
int iLayer2 = 0;
double downScale_qt = 1.0/4  ;//qt���潵�����������ã���δѡ����Ĭ��Ϊ1/4������


static int flag1=0,flag2=0;
int flag11=0,flag22=0;
double ww,hh;    //С��ͼ��Ŀ�Ⱥ͸߶�
int h0;
int w0;

double ColorPow;
double CuvePow;
double ShapePow;

//extern 
 double	m_ColorPow;
//extern 
 double	m_CuvePow;
//extern 
 double	m_ShapePow;
vector<double> xishu;
vector<QString> lujing;
#define min1(a,b)            (((a) < (b)) ? (a) : (b))
#define max1(a,b)            (((a) > (b)) ? (a) : (b))
extern int m_KeyMainColorSig[111]; // ��ѯСͼ�������
extern double m_KeyCuve[15];
extern double m_KeyShape[8];
extern int m_Color[111];//����ͼ������
extern double m_Cuve[15],m_Shape[8];
sy_2::sy_2(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	//Map�ؼ�����
	ui.MapCtr1->setControl("{C552EA94-6FBB-11d5-A9C1-00104BB6FC1C}");
	ui.MapToolBarCtr1->setControl("{B7D4358E-3CBC-11D6-AA09-00104BB6FC1C}");
	ui.MapCtr2->setControl("{C552EA94-6FBB-11d5-A9C1-00104BB6FC1C}");
	ui.MapToolBarCtr2->setControl("{B7D4358E-3CBC-11D6-AA09-00104BB6FC1C}");
	//	ui.MapCtr3->setControl("{C552EA94-6FBB-11d5-A9C1-00104BB6FC1C}");
	//	ui.MapToolBarCtr3->setControl("{B7D4358E-3CBC-11D6-AA09-00104BB6FC1C}");
	ui.MapCtr4->setControl("{C552EA94-6FBB-11d5-A9C1-00104BB6FC1C}");
	ui.MapToolBarCtr4->setControl("{B7D4358E-3CBC-11D6-AA09-00104BB6FC1C}");

	ui.MapTocCtr1->setControl("{A0F9A9A2-4A3F-4888-826F-CA2BFA23CF17}");
	ui.MapTocCtr2->setControl("{A0F9A9A2-4A3F-4888-826F-CA2BFA23CF17}");
	ui.MapTocCtr4->setControl("{A0F9A9A2-4A3F-4888-826F-CA2BFA23CF17}");



	//��ui����Ŀؼ��Ͷ����ָ�����
	//Map�ؼ�
	ui.MapToolBarCtr1->queryInterface(QUuid(IID_IToolbarControl),(void**)&m_pToolbarControl1);
	ui.MapCtr1->queryInterface(QUuid(IID_IMapControl3),(void**)&m_pMapControl1);
	ui.MapToolBarCtr2->queryInterface(QUuid(IID_IToolbarControl),(void**)&m_pToolbarControl2);
	ui.MapCtr2->queryInterface(QUuid(IID_IMapControl3),(void**)&m_pMapControl2);
	//	ui.MapToolBarCtr3->queryInterface(QUuid(IID_IToolbarControl),(void**)&m_pToolbarControl3);
	//	ui.MapCtr3->queryInterface(QUuid(IID_IMapControl3),(void**)&m_pMapControl3);
	ui.MapToolBarCtr4->queryInterface(QUuid(IID_IToolbarControl),(void**)&m_pToolbarControl4);
	ui.MapCtr4->queryInterface(QUuid(IID_IMapControl3),(void**)&m_pMapControl4);


	ui.MapTocCtr1->queryInterface(QUuid(IID_ITOCControl),(void**)&m_pTOCControl1);
	ui.MapTocCtr2->queryInterface(QUuid(IID_ITOCControl),(void**)&m_pTOCControl2);
	ui.MapTocCtr4->queryInterface(QUuid(IID_ITOCControl),(void**)&m_pTOCControl4);

	//���������Ϳؼ������
	//Map�ؼ�
	m_pToolbarControl1->SetBuddyControl(m_pMapControl1);
	m_pToolbarControl2->SetBuddyControl(m_pMapControl2);
	//	m_pToolbarControl3->SetBuddyControl(m_pMapControl3);
	m_pToolbarControl4->SetBuddyControl(m_pMapControl4);

	m_pTOCControl1->SetBuddyControl(m_pMapControl1);
	m_pTOCControl2->SetBuddyControl(m_pMapControl2);
	m_pTOCControl4->SetBuddyControl(m_pMapControl4);
	//�������ӹ���������

	AddMapToolBarCtr(m_pToolbarControl1);//map
	AddMapToolBarCtr(m_pToolbarControl2);//map
	//	AddMapToolBarCtr(m_pToolbarControl3);//map
	AddMapToolBarCtr(m_pToolbarControl4);//map

	//Map�ؼ�����
	ui.MapCtr1_2->setControl("{C552EA94-6FBB-11d5-A9C1-00104BB6FC1C}");
	ui.MapToolBarCtr1_2->setControl("{B7D4358E-3CBC-11D6-AA09-00104BB6FC1C}");
	ui.MapCtr2_2->setControl("{C552EA94-6FBB-11d5-A9C1-00104BB6FC1C}");
	ui.MapToolBarCtr2_2->setControl("{B7D4358E-3CBC-11D6-AA09-00104BB6FC1C}");
	//	ui.MapCtr3->setControl("{C552EA94-6FBB-11d5-A9C1-00104BB6FC1C}");
	//	ui.MapToolBarCtr3->setControl("{B7D4358E-3CBC-11D6-AA09-00104BB6FC1C}");
	ui.MapCtr4_2->setControl("{C552EA94-6FBB-11d5-A9C1-00104BB6FC1C}");
	ui.MapToolBarCtr4_2->setControl("{B7D4358E-3CBC-11D6-AA09-00104BB6FC1C}");

	ui.MapTocCtr1_2->setControl("{A0F9A9A2-4A3F-4888-826F-CA2BFA23CF17}");
	ui.MapTocCtr2_2->setControl("{A0F9A9A2-4A3F-4888-826F-CA2BFA23CF17}");
	ui.MapTocCtr4_2->setControl("{A0F9A9A2-4A3F-4888-826F-CA2BFA23CF17}");

	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(OpenFile1()));
	connect(ui.pushButton_2,SIGNAL(clicked()),this,SLOT(OpenFile2()));

	//��ui����Ŀؼ��Ͷ����ָ�����
	//Map�ؼ�
	ui.MapToolBarCtr1_2->queryInterface(QUuid(IID_IToolbarControl),(void**)&m_pToolbarControl1_2);
	ui.MapCtr1_2->queryInterface(QUuid(IID_IMapControl3),(void**)&m_pMapControl1_2);
	ui.MapToolBarCtr2_2->queryInterface(QUuid(IID_IToolbarControl),(void**)&m_pToolbarControl2_2);
	ui.MapCtr2_2->queryInterface(QUuid(IID_IMapControl3),(void**)&m_pMapControl2_2);
	//	ui.MapToolBarCtr3->queryInterface(QUuid(IID_IToolbarControl),(void**)&m_pToolbarControl3);
	//	ui.MapCtr3->queryInterface(QUuid(IID_IMapControl3),(void**)&m_pMapControl3);
	ui.MapToolBarCtr4_2->queryInterface(QUuid(IID_IToolbarControl),(void**)&m_pToolbarControl4_2);
	ui.MapCtr4_2->queryInterface(QUuid(IID_IMapControl3),(void**)&m_pMapControl4_2);


	ui.MapTocCtr1_2->queryInterface(QUuid(IID_ITOCControl),(void**)&m_pTOCControl1_2);
	ui.MapTocCtr2_2->queryInterface(QUuid(IID_ITOCControl),(void**)&m_pTOCControl2_2);
	ui.MapTocCtr4_2->queryInterface(QUuid(IID_ITOCControl),(void**)&m_pTOCControl4_2);
	//���������Ϳؼ������
	//Map�ؼ�
	m_pToolbarControl1_2->SetBuddyControl(m_pMapControl1_2);
	m_pToolbarControl2_2->SetBuddyControl(m_pMapControl2_2);
	//	m_pToolbarControl3->SetBuddyControl(m_pMapControl3);
	m_pToolbarControl4_2->SetBuddyControl(m_pMapControl4_2);

	m_pTOCControl1_2->SetBuddyControl(m_pMapControl1_2);
	m_pTOCControl2_2->SetBuddyControl(m_pMapControl2_2);
	m_pTOCControl4_2->SetBuddyControl(m_pMapControl4_2);
	//�������ӹ���������

	AddMapToolBarCtr(m_pToolbarControl1_2);//map
	AddMapToolBarCtr(m_pToolbarControl2_2);//map
	//	AddMapToolBarCtr(m_pToolbarControl3);//map
	AddMapToolBarCtr(m_pToolbarControl4_2);//map

	connect(ui.pushButton_3,SIGNAL(clicked()),this,SLOT(OpenFile3()));
	connect(ui.pushButton_4,SIGNAL(clicked()),this,SLOT(OpenFile4()));


	//�Բ˵����д�ͼ��Ĳ�����������
	//��һ�������ö���
	openAction1 = new QAction( QIcon(":/images/doc-open"), tr("&�򿪲ο�ͼ��"), this );
	openAction1->setStatusTip(tr("Open the First Image"));//�������������ʱ�����ڴ������·�״̬����ʾ״̬
	openAction2 = new QAction( QIcon(":/images/doc-open"), tr("&�򿪴���׼ͼ��"), this );
	openAction2->setStatusTip(tr("Open the Second Image"));//ͬ��
	SurfAction = new QAction( QIcon("tree"), tr("&������׼�㷨"), this );
	SurfAction->setStatusTip(tr("SURF Registration"));//ͬ��
	BigMapSURFAction = new QAction( QIcon("tree"), tr("&���ٸ߾�����׼�㷨"), this );
	BigMapSURFAction->setStatusTip(tr("Advanced SURF Registration"));
	DeleMapLayersAction = new QAction( QIcon(":/image/doc-close"),tr("&�����������"),this);
	DeleMapLayersAction->setStatusTip(tr("Clear all Datas"));
	DSurfAction = new QAction( QIcon("tree"), tr("&���ٶ�λ�㷨"), this );
	DSurfAction->setStatusTip(tr("SURF Registration"));//ͬ��
	DSFenKuaiAction = new QAction( QIcon("tree"), tr("&�ֿ��㷨"), this );
	DSFenKuaiAction->setStatusTip(tr("SURF Registration"));//ͬ��
	//�ڶ������������뺯����������γ��ź�-�ۻ���
	connect(openAction1, SIGNAL(triggered()), this, SLOT(OpenFile1()));
	connect(openAction2, SIGNAL(triggered()), this, SLOT(OpenFile2()));
	connect(BigMapSURFAction,SIGNAL(triggered()), this, SLOT(AdBigMapRegistration()));
	connect(SurfAction, SIGNAL(triggered()), this, SLOT(BigMapRegistration()));
	connect(DeleMapLayersAction,SIGNAL(triggered()), this, SLOT(OnClearMapLayer()));
	connect(DSurfAction,SIGNAL(triggered()), this, SLOT(DSurfSpeed()));
	connect(DSFenKuaiAction,SIGNAL(triggered()), this, SLOT(FenKuai_DSURF()));

	connect(ui.pushButton_6,SIGNAL(clicked()), this, SLOT(AdBigMapRegistration()));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(BigMapRegistration()));
	connect(ui.pushButton_8,SIGNAL(clicked()), this, SLOT(DSurfSpeed()));
	connect(ui.pushButton_7,SIGNAL(clicked()), this, SLOT(FenKuai_DSURF()));

	//���������ڲ˵�����Ӧ�ط�����Ӷ���
	ui.menu_3->addAction(openAction1);
	ui.menu_3->addAction(openAction2);
	ui.menu_2->addAction(SurfAction);
	ui.menu_2->addAction(BigMapSURFAction);//
	ui.menu->addAction(DeleMapLayersAction);
	FileOpenFlag1 = 0;
	FileOpenFlag2 = 0;
	ui.pushButton->addAction(openAction1);
	ui.pushButton_2->addAction(openAction2);
	ui.menu_4->addAction(DSFenKuaiAction);
	ui.menu_4->addAction(DSurfAction);
//	init_UI();
//	FirstAction = new QAction(QIcon("tree"),tr("&First"),this);
//	connect(FirstAction,SIGNAL(triggered()),this,SLOT(on_actionFirst_triggered()));
//	ui.menu_4->addAction(FirstAction);
	connect(ui.comboBox,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(on_down_rate(const QString &)));


}

sy_2::~sy_2()
{
}

void sy_2::on_down_rate(const QString &text)
{
	QString str,f1,f2;
	str = "Your downsample rate is " + text;
	QMessageBox::information(this,tr("Info"),str);
	f1 = "1/4";
	f2 = "1/8";
	if (QString::compare(text,f1)==0)
	{
		downScale_qt = 0.25;
	}
	else if (QString::compare(text,f2)==0)
	{
		downScale_qt = 0.125;
	}

}
//void sy_2::on_click_down()
//{
//	QString str;
//	str = "Your downsample rate is " + ui.comboBox->currentText();
//	QMessageBox::information(this,tr("Info"),str);
//}
void sy_2::AddMapToolBarCtr(IToolbarControlPtr pToolbar)
{
	CComVariant varTool;
	long itemIndex;

	if (!pToolbar) 
		return;

	varTool = L"esriControlCommands.ControlsAddDataCommand";
	pToolbar->AddItem(varTool, 0, -1, VARIANT_FALSE, 0,
		esriCommandStyleIconOnly, &itemIndex);
	varTool = L"esriControlCommands.ControlsMapZoomInTool";
	pToolbar->AddItem(varTool, 0, -1, VARIANT_FALSE, 0,
		esriCommandStyleIconOnly, &itemIndex);
	varTool = L"esriControlCommands.ControlsMapZoomOutTool";
	pToolbar->AddItem(varTool, 0, -1, VARIANT_FALSE, 0,
		esriCommandStyleIconOnly, &itemIndex);
	varTool = L"esriControlCommands.ControlsMapZoomInFixedCommand";
	pToolbar->AddItem(varTool, 0, -1, VARIANT_FALSE, 0,
		esriCommandStyleIconOnly, &itemIndex);
	varTool = L"esriControlCommands.ControlsMapZoomOutFixedCommandl";
	pToolbar->AddItem(varTool, 0, -1, VARIANT_FALSE, 0,
		esriCommandStyleIconOnly, &itemIndex);
	varTool = L"esriControlCommands.ControlsMapPanTool";
	pToolbar->AddItem(varTool, 0, -1, VARIANT_FALSE, 0,
		esriCommandStyleIconOnly, &itemIndex);
	varTool = L"esriControlCommands.ControlsMapFullExtentCommand";
	pToolbar->AddItem(varTool, 0, -1, VARIANT_FALSE, 0,
		esriCommandStyleIconOnly, &itemIndex);
	varTool = L"esriControlCommands.ControlsMapZoomToLastExtentBackCommand";
	pToolbar->AddItem(varTool, 0, -1, VARIANT_FALSE, 0,
		esriCommandStyleIconOnly, &itemIndex);
	varTool = L"esriControlCommands.ControlsMapZoomToLastExtentForwardCommand";
	pToolbar->AddItem(varTool, 0, -1, VARIANT_FALSE, 0,
		esriCommandStyleIconOnly, &itemIndex);

}


bool sy_2::OpenFile1()
{
	GDALAllRegister();
	
	PyramidBase pyramid1; 
	refImg.clear();
	QString file_full1,file_name1,m_strFileName1;
	QString file_path1 = NULL;
	String reImg_full,reImg_path;	//reimg_full:ͼ�������·����reimg_path��ͼ�������ļ���·��

	QFileInfo fi;		  
	file_full1 = QFileDialog::getOpenFileName(this,"open the first picture",file_path1);  
	fi = QFileInfo(file_full1);  
	m_strFileName1=fi.absoluteFilePath();

	reImg_full = m_strFileName1.toStdString();////QString ת��Ϊstring///ͼ������·��
	int po = reImg_full.find_last_of('/');
	reImg_path = reImg_full.substr( 0,po+1); ///�ļ���·��
	//string filename = reImg_full.substr(po+1,reImg_full.length());
	//if (filename.length() < 1)
//	{
//		QMessageBox::information(this, tr("Information"),tr("��ͼ��ʧ�ܣ���ѡ����ȷ·����"));
//	}
	//���ο�ͼ�������ļ���·���Լ�ͼ��·�������·������

	
	refImg.push_back(reImg_path);//refImg[0]:�ο�ͼ�������ļ���·��
	refImg.push_back(reImg_full);//refImg[1]:�ο�ͼ������·��
	
	//���ο�ͼ�񱣴��������������·����
	pyramid1.InFilePath = reImg_full.c_str();	//�ο�ͼ������·��
	pyramid1.FilePath = reImg_path.c_str();		//�ο�ͼ�������ļ���·��
	
	pyramid1.InFile = (GDALDataset*)GDALOpen(pyramid1.InFilePath, GA_ReadOnly);		//��·���е��ļ����뵽���ݼ���
	if (pyramid1.InFile == NULL)
	{
		QMessageBox::information(this, tr("Information"),tr("��ͼ��ʧ�ܣ���ѡ����ȷ·����"));
		return false;
	}
	pyramid1.CheckPyramid();
	refLayer.clear();
	char rsize[1024];
	sprintf(rsize,"%d*%d",pyramid1.iWidth,pyramid1.iHeight);
	QString rsize1 = QString::fromUtf8(rsize);
	ui.refImageSize->setText(rsize1);
	ui.refImageSize_2->setText(rsize1);

	char dir[1024];
	sprintf(dir,"%s%s",pyramid1.FilePath,"refPyramidLayers");//filepath:�������������ļ���
	//�жϸ��ļ����Ƿ���ڣ������ڣ��������½������������ڣ��򴴽����ļ���
	if(_access(dir, 0) != -1)  
	{  
		cout << "�ļ����Ѵ���" <<endl;  
		//system("pause");
	}  
	else  
	{  
		cout << "�ļ��в����ڣ������ļ���" << endl;  
		_mkdir(dir); 
		//system("pause");
	} 
	pyramid1.LayerPath = dir;
	refLayer.push_back(dir);		//refLayer[0]:�ο�ͼ��������ļ���·��

	for (int i = 0;i < pyramid1.iOverViewCount; i++)
	{
			
		char filepath[1024] = "";			//filepath[1024]:���Ա��溯�����صĽ�����ÿһ��ͼ��·��
		pyramid1.SaveSingleBand(i,filepath);//,pyramid1.InFile);
		refLayer.push_back(filepath);	//refLayer[i+1]:�ο�ͼ���������i��ͼ��·��������refLayer[1]�����0��(ԭͼ���ص�1/4 * 1/4)
	}

	/*Map�ؼ���ʾ�ο�ͼ��*/
	BSTR bstr_str;

	bstr_str = SysAllocString(m_strFileName1.utf16());

	HRESULT hr;

	IRasterLayerPtr pRasterLy(CLSID_RasterLayer);

	hr=pRasterLy->CreateFromFilePath(bstr_str);

	if(FAILED(hr))

	{
		
		return false;
	}

	ILayerPtr pLyr(pRasterLy);

	m_pMapControl1->AddLayer(pLyr,0);

	IActiveViewPtr pActiveView(m_pMapControl1);

	m_pMapControl1->Refresh(esriViewAll);  

	FileOpenFlag1 = 1;
	//��ͼ���С�����ж�
	if (pyramid1.iWidth < 1000 || pyramid1.iHeight < 1000)
		FileOpenFlag1 = 2;
	else if(pyramid1.iWidth < 4300 || (pyramid1.iHeight < 4300))
		FileOpenFlag1 = 3;
	else
		FileOpenFlag1 = 4;
	return true;

	if (FileOpenFlag1 > 0 && FileOpenFlag2 > 0)
	{
		//�������ͼ�����ˡ�
		if(FileOpenFlag1 == 2 && FileOpenFlag2 == 2)
		{
			//����ʾʹ��Сͼƥ���㷨
			QMessageBox::information(this, tr("Information"),tr("����ͼ��ߴ��С��1000������ʹ�ÿ���ƥ���㷨��"));

		}
		else if ((FileOpenFlag1 == 3 && FileOpenFlag2 == 2)||(FileOpenFlag1 == 4 && FileOpenFlag1 == 2))
		{
			//����ʾʹ�ÿ��ٶ�λ�㷨
			QMessageBox::information(this, tr("Information"),tr("����ͼ��һ��һС������ʹ�ÿ��ٶ�λ�㷨��"));
		}
		else if (FileOpenFlag1 == 4 && FileOpenFlag2 == 4)
		{
			//����ʾʹ�ó���ͼ����׼�㷨
			QMessageBox::information(this, tr("Information"),tr(" ����ͼΪ����ͼ�񣬽���ʹ�ó���ͼ����׼�㷨�� "));
		}

	}
	else if (FileOpenFlag2 == 0)
	{
		//��ʾδ�򿪴���׼ͼ��
		QMessageBox::information(this, tr("Information"),tr(" �����´򿪴���׼ͼ�� "));
	}

}

bool sy_2::OpenFile2()
{
	GDALAllRegister();
	PyramidBase pyramid2; 
	senImg.clear();
	senLayer.clear();
	QString file_full2,file_name2,m_strFileName2;
	QString file_path2 = NULL;
	String sen_full,sen_path;	//sen_full:����׼ͼ������·����sen_path������׼ͼ�������ļ���·��

	QFileInfo fi;		  
	file_full2 = QFileDialog::getOpenFileName(this,"open the second picture",file_path2);  
	fi = QFileInfo(file_full2);  
	m_strFileName2=fi.absoluteFilePath();

	sen_full = m_strFileName2.toStdString();
	int po = sen_full.find_last_of('/');
	sen_path = sen_full.substr( 0, po + 1 );

	/*������׼ͼ��·������ȫ��·��������*/
	senImg.push_back(sen_path);		//senImg[0]:����׼ͼ�������ļ���·��
	senImg.push_back(sen_full);		//senImg[1]������׼ͼ������·��
	
	/*������׼ͼ��·�����ݸ���������*/
	pyramid2.InFilePath = sen_full.c_str();	//����׼ͼ������·��
	pyramid2.FilePath = sen_path.c_str();	//����׼ͼ���ļ���·��

	pyramid2.InFile = (GDALDataset*)GDALOpen(pyramid2.InFilePath, GA_ReadOnly);		//��·���е��ļ����뵽���ݼ���
	if (pyramid2.InFile == NULL)
	{
		QMessageBox::information(this, tr("Information"),tr("��ͼ��ʧ�ܣ���ѡ����ȷ·����"));
		return false;
	}

	pyramid2.CheckPyramid();
	char rsize[1024];
	sprintf(rsize,"%d*%d",pyramid2.iWidth,pyramid2.iHeight);
	QString rsize2 = QString::fromUtf8(rsize);
	ui.senImageSize->setText(rsize2);
	ui.senImageSize_2->setText(rsize2);

	char dir[1024];
	sprintf(dir,"%s%s",pyramid2.FilePath,"senPyramidLayers");//filepath:�������������ļ���
	//�жϸ��ļ����Ƿ���ڣ������ڣ��������½������������ڣ��򴴽����ļ���
	if(_access(dir, 0) != -1)  
	{  
		cout << "�ļ����Ѵ���" <<endl;  
		//system("pause");
	}  
	else  
	{  
		cout << "�ļ��в����ڣ������ļ���" << endl;  
		_mkdir(dir); 
		//system("pause");
	} 
	pyramid2.LayerPath = dir;
	senLayer.push_back(dir);		//refLayer[0]:�ο�ͼ��������ļ���·��

	for (int i = 0;i < pyramid2.iOverViewCount; i++)
	{

		char filepath[1024] = "";			//filepath[1024]:���Ա��溯�����صĽ�����ÿһ��ͼ��·��
		pyramid2.SaveSingleBand(i,filepath);//,pyramid1.InFile);
		senLayer.push_back(filepath);	//refLayer[i+1]:�ο�ͼ���������i��ͼ��·��������refLayer[1]�����0��(ԭͼ���ص�1/4 * 1/4)
	}

	m_strFileName2=fi.absoluteFilePath();

	BSTR bstr_str;

	bstr_str = SysAllocString(m_strFileName2.utf16());

	HRESULT hr;

	IRasterLayerPtr pRasterLy(CLSID_RasterLayer);

	hr=pRasterLy->CreateFromFilePath(bstr_str);

	if(FAILED(hr))
	{
		QMessageBox::information(this, tr("Information"),tr("��ͼ��ʧ�ܣ���ѡ����ȷ·����"));
		return false;
	}
	ILayerPtr pLyr(pRasterLy);

	m_pMapControl2->AddLayer(pLyr,0);

	IActiveViewPtr pActiveView(m_pMapControl2);

	m_pMapControl2->Refresh(esriViewAll);  

	FileOpenFlag2 = 1;
	
	//��ͼ����ˣ�������жϴ�С
	if (pyramid2.iWidth < 1000 || pyramid2.iHeight < 1000)
		FileOpenFlag2 = 2;
	else if(pyramid2.iWidth < 4300 || (pyramid2.iHeight < 4300))
		FileOpenFlag2 = 3;
	else 
		FileOpenFlag2 = 4;
	

	if (FileOpenFlag1>0 && FileOpenFlag2>0)
	{
		if(FileOpenFlag1 == 2 && FileOpenFlag2 == 2)
		{
			//����ʾʹ��Сͼƥ���㷨
			QMessageBox::information(this, tr("Information"),tr("����ͼ��ߴ��С��1000������ʹ�ÿ���ƥ���㷨��"));

		}
		else if ((FileOpenFlag1 == 3 && FileOpenFlag2 == 2)||(FileOpenFlag1 == 4 && FileOpenFlag1 == 2))
		{
			//����ʾʹ�ÿ��ٶ�λ�㷨
			QMessageBox::information(this, tr("Information"),tr("����ͼ��һ��һС������ʹ�ÿ��ٶ�λ�㷨��"));
		}
		else if (FileOpenFlag1 == 4 && FileOpenFlag2 == 4)
		{
			//����ʾʹ�ó���ͼ����׼�㷨
			QMessageBox::information(this, tr("Information"),tr(" ����ͼΪ����ͼ�񣬽���ʹ�ó���ͼ����׼�㷨�� "));
		}


	}
	else if (FileOpenFlag1 == 0)
	{
		//��ʾδ�򿪲ο�ͼ��
		QMessageBox::information(this, tr("Information"),tr(" �����´򿪲ο�ͼ�� "));
	}

	return true;

}


bool sy_2::OpenFile3()
{
	GDALAllRegister();
	
	PyramidBase pyramid3; 
	refImg.clear();
	QString file_full1,file_name1,m_strFileName1;
	QString file_path1 = NULL;
	String reImg_full,reImg_path;	//reimg_full:ͼ�������·����reimg_path��ͼ�������ļ���·��

	QFileInfo fi;		  
	file_full1 = QFileDialog::getOpenFileName(this,"open the second picture",file_path1);  
	fi = QFileInfo(file_full1);  
	m_strFileName1=fi.absoluteFilePath();

	reImg_full = m_strFileName1.toStdString();////QString ת��Ϊstring///ͼ������·��
	int po = reImg_full.find_last_of('/');
	reImg_path = reImg_full.substr( 0,po+1); ///�ļ���·��

	//���ο�ͼ�������ļ���·���Լ�ͼ��·�������·������
	refImg.push_back(reImg_path);//refImg[0]:�ο�ͼ�������ļ���·��
	refImg.push_back(reImg_full);//refImg[1]:�ο�ͼ������·��

	//���ο�ͼ�񱣴��������������·����
	pyramid3.InFilePath = reImg_full.c_str();	//�ο�ͼ������·��
	pyramid3.FilePath = reImg_path.c_str();		//�ο�ͼ�������ļ���·��

	pyramid3.InFile = (GDALDataset*)GDALOpen(pyramid3.InFilePath, GA_ReadOnly);		//��·���е��ļ����뵽���ݼ���
	if (pyramid3.InFile == NULL)
	{
		QMessageBox::information(this, tr("Information"),tr("��ͼ��ʧ�ܣ���ѡ����ȷ·����"));
		return false;
	}
	//InFile = (GDALDataset*)GDALOpen(InFilePath, GA_ReadOnly);		//��·���е��ļ����뵽���ݼ���
	pyramid3.sCheckPyramid();

	char rsize[1024];
	sprintf(rsize,"%d*%d",pyramid3.iWidth,pyramid3.iHeight);
	QString rsize1 = QString::fromUtf8(rsize);
	ui.refImageSize_3->setText(rsize1);
	ui.refImageSize_4->setText(rsize1);

	refLayer.clear();
	char dir[1024];
	sprintf(dir,"%s%s",pyramid3.FilePath,"refPyramidLayers");//filepath:�������������ļ���
	//�жϸ��ļ����Ƿ���ڣ������ڣ��������½������������ڣ��򴴽����ļ���
	if(_access(dir, 0) != -1)  
	{  
		cout << "�ļ����Ѵ���" <<endl;  
		//system("pause");
	}  
	else  
	{  
		cout << "�ļ��в����ڣ������ļ���" << endl;  
		_mkdir(dir); 
		//system("pause");
	} 
	pyramid3.LayerPath = dir;
	refLayer.push_back(dir);		//refLayer[0]:�ο�ͼ��������ļ���·��
	rfimg.clear();
	IplImage *scr=0;
	IplImage *dst=0;
	char rsname[100];
	char * bb;

	scr = cvLoadImage(refImg[1].c_str(),-1);\
	rfimg.push_back(scr);
	CvSize dst_cvsize;
	double scale;
	int sOverViewCount = pyramid3.iOverViewCount;//��ȡ�ܹ��ж��ٲ�������ʹ������㽵����ͼ��
	for (int i = 1; i <= sOverViewCount;i++)
	{

		if (scr)
		{
			scale = 1.0/pow(2.0,i);
			dst_cvsize.width=(int)(scr->width*scale);
			dst_cvsize.height=(int)(scr->height*scale);
			dst=cvCreateImage(dst_cvsize,scr->depth,scr->nChannels);
			cvResize(scr,dst,CV_INTER_AREA);
			sprintf(rsname,"%s%s%d%s",dir,"/1_",i,".jpg");
			bb=rsname;
			refLayer.push_back(bb);
			cvSaveImage(bb,dst);
		}
	}
	

	/*Map�ؼ���ʾ�ο�ͼ��*/
	BSTR bstr_str;

	bstr_str = SysAllocString(m_strFileName1.utf16());

	HRESULT hr;

	IRasterLayerPtr pRasterLy(CLSID_RasterLayer);

	hr=pRasterLy->CreateFromFilePath(bstr_str);

	if(FAILED(hr))

	{
		QMessageBox::information(this, tr("Information"),tr("��ͼ��ʧ�ܣ���ѡ����ȷ·����"));
		return false;
	}

	ILayerPtr pLyr(pRasterLy);

	m_pMapControl1_2->AddLayer(pLyr,0);

	IActiveViewPtr pActiveView(m_pMapControl1_2);

	m_pMapControl1_2->Refresh(esriViewAll);  


	FileOpenFlag3 = 1;
	//��ͼ���С�����ж�
	if (pyramid3.iWidth < 1000 || pyramid3.iHeight < 1000)
		FileOpenFlag3 = 2;
	else if(pyramid3.iWidth < 4300 || (pyramid3.iHeight < 4300))
		FileOpenFlag3 = 3;
	else
		FileOpenFlag3 = 4;
	return true;

	if (FileOpenFlag3 > 0 && FileOpenFlag4 > 0)
	{
		//�������ͼ�����ˡ�
		if(FileOpenFlag3 == 2 && FileOpenFlag4 == 2)
		{
			//����ʾʹ��Сͼƥ���㷨
			QMessageBox::information(this, tr("Information"),tr("����ͼ��ߴ��С��1000������ʹ�ÿ���ƥ���㷨��"));

		}
		else if ((FileOpenFlag3 == 3 && FileOpenFlag4 == 2)||(FileOpenFlag1 == 4 && FileOpenFlag1 == 2))
		{
			//����ʾʹ�ÿ��ٶ�λ�㷨
			QMessageBox::information(this, tr("Information"),tr("����ͼ��һ��һС������ʹ�ÿ��ٶ�λ�㷨��"));
		}
		else if (FileOpenFlag3 == 4 && FileOpenFlag4 == 4)
		{
			//����ʾʹ�ó���ͼ����׼�㷨
			QMessageBox::information(this, tr("Information"),tr(" ����ͼΪ����ͼ�񣬽���ʹ�ó���ͼ����׼�㷨�� "));
		}

	}
	else if (FileOpenFlag4 == 0)
	{
		//��ʾδ�򿪴���׼ͼ��
		QMessageBox::information(this, tr("Information"),tr(" �����´򿪴���׼ͼ�� "));
	}




	return true;
}

bool sy_2::OpenFile4()
{
	GDALAllRegister();
	
	PyramidBase pyramid4; 
	senImg.clear();
	QString file_full1,file_name1,m_strFileName1;
	QString file_path1 = NULL;
	String senImg_full,senImg_path;	//reimg_full:ͼ�������·����reimg_path��ͼ�������ļ���·��

	QFileInfo fi;		  
	file_full1 = QFileDialog::getOpenFileName(this,"open the second picture",file_path1);  
	fi = QFileInfo(file_full1);  
	m_strFileName1=fi.absoluteFilePath();

	senImg_full = m_strFileName1.toStdString();////QString ת��Ϊstring///ͼ������·��
	int po = senImg_full.find_last_of('/');
	senImg_path = senImg_full.substr( 0,po+1); ///�ļ���·��

	//���ο�ͼ�������ļ���·���Լ�ͼ��·�������·������
	senImg.push_back(senImg_path);//refImg[0]:�ο�ͼ�������ļ���·��
	senImg.push_back(senImg_full);//refImg[1]:�ο�ͼ������·��

	//���ο�ͼ�񱣴��������������·����
	pyramid4.InFilePath = senImg_full.c_str();	//�ο�ͼ������·��
	pyramid4.FilePath = senImg_path.c_str();		//�ο�ͼ�������ļ���·��

	pyramid4.InFile = (GDALDataset*)GDALOpen(pyramid4.InFilePath, GA_ReadOnly);		//��·���е��ļ����뵽���ݼ���
	if (pyramid4.InFile == NULL)
	{
		QMessageBox::information(this, tr("Information"),tr("��ͼ��ʧ�ܣ���ѡ����ȷ·����"));
		return false;
	}

	pyramid4.sCheckPyramid();

	char rsize[1024];
	sprintf(rsize,"%d*%d",pyramid4.iWidth,pyramid4.iHeight);
	QString rsize2 = QString::fromUtf8(rsize);
	ui.senImageSize_3->setText(rsize2);
	ui.senImageSize_4->setText(rsize2);

	senLayer.clear();
	char dir[1024];
	sprintf(dir,"%s%s",pyramid4.FilePath,"senPyramidLayers");//filepath:�������������ļ���
	//�жϸ��ļ����Ƿ���ڣ������ڣ��������½������������ڣ��򴴽����ļ���
	if(_access(dir, 0) != -1)  
	{  
		cout << "�ļ����Ѵ���" <<endl;  
		//system("pause");
	}  
	else  
	{  
		cout << "�ļ��в����ڣ������ļ���" << endl;  
		_mkdir(dir); 
		//system("pause");
	} 
	pyramid4.LayerPath = dir;
	senLayer.push_back(dir);		//refLayer[0]:�ο�ͼ��������ļ���·��
	snimg.clear();
	IplImage *scr=0;
	IplImage *dst=0;
	char rsname[100];
	char * bb;

	scr = cvLoadImage(senImg[1].c_str(),-1);
	snimg.push_back(scr);
	CvSize dst_cvsize;
	double scale;
	int sOverViewCount = pyramid4.iOverViewCount;//��ȡ�ܹ��ж��ٲ�������ʹ������㽵����ͼ��
	for (int i = 1; i <= sOverViewCount;i++)
	{

		if (scr)
		{
			scale = 1.0/pow(2.0,i);
			dst_cvsize.width=(int)(scr->width*scale);
			dst_cvsize.height=(int)(scr->height*scale);
			dst=cvCreateImage(dst_cvsize,scr->depth,scr->nChannels);
			cvResize(scr,dst,CV_INTER_AREA);
			sprintf(rsname,"%s%s%d%s",dir,"/1_",i,".jpg");
			bb=rsname;
			senLayer.push_back(bb);
			cvSaveImage(bb,dst);
		}
	}
	


	/*Map�ؼ���ʾ�ο�ͼ��*/
	BSTR bstr_str;

	bstr_str = SysAllocString(m_strFileName1.utf16());

	HRESULT hr;

	IRasterLayerPtr pRasterLy(CLSID_RasterLayer);

	hr=pRasterLy->CreateFromFilePath(bstr_str);

	if(FAILED(hr))

	{
		QMessageBox::information(this, tr("Information"),tr("��ͼ��ʧ�ܣ���ѡ����ȷ·����"));
		return false;
	}

	ILayerPtr pLyr(pRasterLy);

	m_pMapControl2_2->AddLayer(pLyr,0);

	IActiveViewPtr pActiveView(m_pMapControl2_2);

	m_pMapControl2_2->Refresh(esriViewAll);  


	FileOpenFlag4 = 1;
	//��ͼ���С�����ж�
	if (pyramid4.iWidth < 1000 || pyramid4.iHeight < 1000)
		FileOpenFlag4 = 2;
	else if(pyramid4.iWidth < 4300 || (pyramid4.iHeight < 4300))
		FileOpenFlag4 = 3;
	else
		FileOpenFlag4 = 4;
	return true;

	if (FileOpenFlag3 > 0 && FileOpenFlag4 > 0)
	{
		//�������ͼ�����ˡ�
		if(FileOpenFlag3 == 2 && FileOpenFlag4 == 2)
		{
			//����ʾʹ��Сͼƥ���㷨
			QMessageBox::information(this, tr("Information"),tr("����ͼ��ߴ��С��1000������ʹ�ÿ���ƥ���㷨��"));

		}
		else if ((FileOpenFlag3 == 3 && FileOpenFlag4 == 2)||(FileOpenFlag3 == 4 && FileOpenFlag4 == 2))
		{
			//����ʾʹ�ÿ��ٶ�λ�㷨
			QMessageBox::information(this, tr("Information"),tr("����ͼ��һ��һС������ʹ�ÿ��ٶ�λ�㷨��"));
		}
		else if (FileOpenFlag3 == 4 && FileOpenFlag4 == 4)
		{
			//����ʾʹ�ó���ͼ����׼�㷨
			QMessageBox::information(this, tr("Information"),tr(" ����ͼΪ����ͼ�񣬽���ʹ�ó���ͼ����׼�㷨�� "));
		}

	}
	else if (FileOpenFlag3 == 0)
	{
		//��ʾδ�򿪴���׼ͼ��
		QMessageBox::information(this, tr("Information"),tr(" �����´򿪴���׼ͼ�� "));
	}


	return true;
}


//��ͼ��׼
void sy_2::BigMapRegistration()
{

	
	double t2=(double)cvGetTickCount();//��ʼ��ʱ
	double t3=(double)cvGetTickCount();//��ʼ��ʱ
	///���ȶ�Сͼ������׼���ʵ��ö�Сͼƥ����㷨

	//�ȶ���Խ�����Сͼ��׼�Ľ��ͼ���·�����ݶ������ͼ������ڲο�ͼ���ļ�����

	string dir = refImg[0] + "BigMapResultFiles"; ///�ڲο�ͼ���ļ����´����ļ��У�����Сͼ�ʹ�ͼ����׼���
	if(_access(dir.c_str(), 0) != -1)  
	{  
		cout << "�ļ����Ѵ���" <<endl;  
		//system("pause");
	}  
	else  
	{  
		cout << "�ļ��в����ڣ������ļ���" << endl;  
		_mkdir(dir.c_str()); 
		//system("pause");
	} 
	string dir1 = dir + "\\";

	string sImageResultPath = dir1 + "PyramidImageRegResult.jpg";
	string BigMapResult = dir1 + "BigMapRegistrationResult.tif";

	resultImg.clear(); 

	resultImg.push_back(dir1);
	resultImg.push_back(sImageResultPath);
	resultImg.push_back(BigMapResult);


	//t2=(double)cvGetTickCount();//��ʼ��ʱ
	//double t1=(double)cvGetTickCount();//��ʼ��ʱ

	//������Ҫ�Ե�һ�ν���ƥ�������ͼ����ѡ�����ں��滹Ҫ�õ��ú������Һ����ƥ�䲻�漰����ͼ��ѡ��ֻ�Ǹ�·���йأ���������ӶԵ�һ��ƥ���ѡ��
	///����ͼƥ�������ѡ����ԣ���һ����Ĭ�ϵ�ѡ�������Լ��趨���ʵĴ�С����ƥ�䣩���ڶ����Ǹ�����ѡ��������Ҫ�ж�
	///
	int i = 1;
	string reImage,senImage;//���������ֱ�������ʱ�洢����ƥ���ͼ��·���������ݸ�ͼ����׼�ĺ���
	double t_read=(double)cvGetTickCount();//��ʼ��ʱ
	for ( i; i < refLayer.size();i ++ )
	{
		
		IplImage * image = cvLoadImage(refLayer[i].c_str());
		//���ζ�ԭͼ���������������Ĵ�С�����жϣ�ѡ����ʵĲ����ƥ��
		if (image->width <= 1000 || image->height <= 1000)
		{
			reImage = refLayer[i];
			senImage = senLayer[i];
			break;
		}

	}
	//t2=(double)cvGetTickCount();//��ʼ��ʱ
	t_read = (double)(cvGetTickCount()-t_read)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
	SURF_ON_TWO(reImage,senImage,sImageResultPath);//��ȡ��������������С��1000��ͼ�����ƥ��
	cv::initModule_nonfree();
	CvMat* H;
	CvMat h;
	CvMat* H1=&h;
	double a[9];
	double px,py;
	double sfx,sfy,rotate;
	vector<int>ptpairs1;
	if (i == 1)
	{
		 OpenResultFile1(sImageResultPath);			
	}
	else
	{
		//��i������1����˵��Ҫ�Խ�������ĳһ��ͼ�������׼�������Ҫ���вü���ƥ��Ȳ���

		Cut_Count_Overlap(i);//�Խ��ͼ����ƥ����������ͳ�ƣ���������Ȥ������в��в�����

		//t3=(double)(cvGetTickCount()-t3)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����

		//	double t4=(double)cvGetTickCount();//��ʼ��ʱ
		//���ŶԸ���Ȥ�����ͼ�����surfƥ��
	//	int pathcount = simage_repath.size();
		string OverlapImage1 = refOverlap[0];
		string OverlapImage2 = senOverlap[0];
		string OverlapResult = refImg[0] + "OverLapResultImage.jpg";
		SURF_ON_TWO(OverlapImage1,OverlapImage2,OverlapResult); //�ú���������Ϻ���H_2[0]�б�����ǵ�ǰͼ���任���H����
		//	t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		//Cut_Count_Overlap();//�Խ��ͼ����ƥ����������ͳ�ƣ���������Ȥ������в��в�����
		//����ӱ任���H��������ȡ�任���������ԶԴ�ͼ���б任
		H = H_2[0];

		double smallhx,smallhy;///�ò�����ʾ�����ü�������Сͼ��ƽ�Ʋ���
		double bighx,bighy;//�ò�����ʾ����׼ͼ������ڲο�ͼ���ƽ�Ʋ���
		//px = cvmGet( H, 0, 2 );
		//py = cvmGet( H, 1, 2 );
		smallhx = cvmGet( H, 0, 2 ) + rePoint[0].x; //����������Ǵ���׼ͼ���и���Ȥ��������ڲο�ͼ���ƽ�ƾ���
		smallhy = cvmGet( H, 1, 2 ) + rePoint[0].y;
		//���ڻ���Ҫ֪�� �ο�ͼ���е�Сͼ����ڴ�ͼ��λ�û�������
		//���ο�ͼ���вü�����Сͼ���Ͻǵ�����
		CvMat *H1; //�����ͼ֮��ı任����������ƽ�������и���

		//����������ݲü����Ĵ���׼ͼ���е�Сͼ�ڲο�ͼ���е�λ�ã����������׼ͼ���ԭͼ�����Ͻ������ڲο�ͼ������ϵ�е�λ�ã�����˵�Ǵ���׼ͼ�����ϽǶ����ƽ������
		bighx = smallhx - senPoint[0].x;
		bighy = smallhy - senPoint[0].y;
		int h1 = abs(bighx) + 0.5;
		int h2 = abs(bighy) + 0.5;
		//�Դ�ͼ��ı任�����е�ƽ�������и���
		H1 = H;
		cvmSet(H1,0,2,bighx);
		cvmSet(H1,1,2,bighy);
		t2=(double)(cvGetTickCount()-t2)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		
		double h11,h12,h21,h22;
		
		h11 = cvmGet(H1,0,0);
		h12 = cvmGet(H1,0,1);
		h21 = cvmGet(H1,1,0);
		h22 = cvmGet(H1,1,1);
		sfx = cvSqrt(h11*h11+h21*h21);
		sfy = cvSqrt(h22*h22+h12*h12);
		rotate = (180*atan(h21/h11))/3.1415;

		QString sbighx = QString::number(bighx,'g',6);
		ui.shift_x->setText(sbighx);
		QString sbighy = QString::number(bighy,'g',6);
		ui.shift_y->setText(sbighy);
		QString iscale1 = QString::number(sfx,'g',6);
		QString iscale2 = QString::number(sfy,'g',6);
		QString iscale = iscale1+"/"+iscale2;
		ui.scale->setText(iscale);
		QString srotate = QString::number(rotate,'g',6);
		ui.rotate->setText(srotate);
		
		
		//�õ��任����֮�������Ҫ����һ���µĸ����ͼ�����Դ洢������ͼ��׼�Ľ��

		//ResultImagePath.push_back(BigMapResult);
		GDALAllRegister();
		//��������ͼ�������ݼ���
		GDALDataset* pSrcDS1 = ( GDALDataset* )GDALOpen(refImg[1].c_str(),GA_ReadOnly); 
		GDALDataset* pSrcDS2 = ( GDALDataset* )GDALOpen(senImg[1].c_str(),GA_ReadOnly);

		GDALDataType eDT1 = pSrcDS1->GetRasterBand(1)->GetRasterDataType();
		GDALDataType eDT2 = pSrcDS2->GetRasterBand(1)->GetRasterDataType();


		int iBandCount1 = pSrcDS1->GetRasterCount();
		int iBandCount2 = pSrcDS2->GetRasterCount();

		double adfGeoTransform[6] = {0};
		//double adfGeoTransform2[6] = {0};

		pSrcDS1->GetGeoTransform(adfGeoTransform);
		pSrcDS2->GetGeoTransform(adfGeoTransform);


		if (H1)
		{

			GDALDriver *poDriver = (GDALDriver *) GDALGetDriverByName("GTIFF");

			int xsize1 = pSrcDS1->GetRasterXSize();
			int ysize1 = pSrcDS1->GetRasterYSize();
			int xsize2 = pSrcDS2->GetRasterXSize();
			int ysize2 = pSrcDS2->GetRasterYSize();

			int iDstWidth = h1 + xsize2;
			int iDstHeight = h2 + ysize2;

			//������׼���ͼ��
			GDALDataset *pDstDS = poDriver->Create(BigMapResult.c_str(), iDstWidth, iDstHeight, iBandCount1, eDT1, NULL);
			pDstDS->SetGeoTransform(adfGeoTransform);
			pDstDS->SetProjection(pSrcDS1->GetProjectionRef());

			int *pBandMap = new int[iBandCount1];
			for (int i=0; i<iBandCount1; i++)
				pBandMap[i] = i+1;

			// ����������Ҫ���������������жϣ����벻ͬ���͵Ļ��������д������������8bitͼ�������ʾ
			//��������ǽ�����ͼ�񶼱任����׼���ͼ���У�ֻ�����õ��Ƿֿ��˼��
			if (eDT1 == GDT_Byte)	// �����8bitͼ��
			{
				char *pDataBuff = new char[xsize1/2*ysize1/2*iBandCount1];
				//���ο�ͼ��任�����ͼ����
				pSrcDS1->RasterIO(GF_Read, 0,0,xsize1/2, ysize1/2, pDataBuff, xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write, 0, 0,  xsize1/2,  ysize1/2, pDataBuff,  xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS1->RasterIO(GF_Read, 0,ysize1/2,xsize1/2, ysize1/2, pDataBuff, xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write, 0, ysize1/2,  xsize1/2,  ysize1/2, pDataBuff,  xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS1->RasterIO(GF_Read, xsize1/2,0,xsize1/2, ysize1/2, pDataBuff, xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write, xsize1/2, 0,  xsize1/2,  ysize1/2, pDataBuff,  xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS1->RasterIO(GF_Read, xsize1/2,ysize1/2,xsize1/2, ysize1/2, pDataBuff, xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write, xsize1/2, ysize1/2,  xsize1/2,  ysize1/2, pDataBuff,  xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				delete(pDataBuff);
				char *pDataBuff2 = new char[xsize2/2*ysize2/2*iBandCount2];

				pSrcDS2->RasterIO(GF_Read, 0,0,xsize2/2, ysize2/2, pDataBuff2, xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write,h1,h2, xsize2/2,  ysize2/2, pDataBuff2,  xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS2->RasterIO(GF_Read, 0,ysize2/2,xsize2/2, ysize2/2, pDataBuff2, xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write,h1,h2+ysize2/2, xsize2/2,  ysize2/2, pDataBuff2,  xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS2->RasterIO(GF_Read,xsize2/2,0,xsize2/2, ysize2/2, pDataBuff2, xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write,h1+xsize2/2,h2, xsize2/2,  ysize2/2, pDataBuff2,  xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS2->RasterIO(GF_Read,xsize2/2,ysize2/2,xsize2/2, ysize2/2, pDataBuff2, xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write,h1+xsize2/2 ,h2+ysize2/2, xsize2/2,  ysize2/2, pDataBuff2,  xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				delete(pDataBuff2);

				t3=(double)(cvGetTickCount()-t3)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����

			}
			else
			{
				QMessageBox::information(NULL, "Title", "Failed!! !", QMessageBox::Yes, QMessageBox::Yes);

			}

			delete(pBandMap);

			GDALClose((GDALDatasetH)pSrcDS1);
			GDALClose((GDALDatasetH)pSrcDS2);
			GDALClose((GDALDatasetH)pDstDS);

		}

		OpenResultFile1(BigMapResult);

	}


	QString s1 = QString::number(t2,'g',6);
	ui.registratetime->setText(s1);
	QString mse2 = QString::number(mese2,'g',6);
	ui.mse->setText(mse2);
	QString s2 = QString::number(num2,'g',6);
	ui.ptpairsnum->setText(s2);
	//QString size1 = QString::number(t3,'g',6);
	//ui.refImageSize->setText(size1);
	//QString size2 = QString::number(t3,'g',6);
	//ui.senImageSize->setText(size2);

	
}



void SURF_ON_TWO(string SrcReImagePath, string SrcSenImagePath, string DstImagePath )
{
	cv::initModule_nonfree();
	img.clear();
	img.push_back(cvLoadImage(SrcReImagePath.c_str())); //����Դ����Ľ��������е�һ��(�������Ǵӵ�0�㿪ʼ������)���е���׼����֮���Բ����öԵ�һ����г���׼��ԭ���������ֲ����ﵽ��Ŀ�Ķ��ǻ�ȡ�ص��������Ȥ�Ĳ��֣�����ʱ��ȴ��һ�������ң��������ܴ��ͼ���ʱ�򣬼�ʹ�ǵ�0��ͼ��Ҳ�޷���opencv�������ۺ�������ʹ�õ�1�����ȫ����Ч�ġ�
	img.push_back(cvLoadImage(SrcSenImagePath.c_str())); 
	//ptpairs_befores.clear();
	CvMat* H;
	CvMat h;
	CvMat* H1=&h;
	double a[9];

	double sfx,sfy;
	double rotate;
	double R11,R12,R21,R22;
	char PX[20]={""},PY[20]={""};
	char SFX[20]={""},SFY[20]={""};
	char ROTATE[20]={""};
	char TIME[20]={""};
	CString d=",";
	int dian;
	char dot[20]={""};
	char time[20]={""};
	vector<int>ptpairs;

	if ( !img[0] || !img[1] )
	{
		QMessageBox::information( 0, "Tips", "Please open another image!", 0, 0 );
		return;
	}
	CvMemStorage* storage = cvCreateMemStorage(0);//�ڴ�洢��
	static CvScalar colors[] =
	{
		{{ 0, 0, 255 }},
		{{ 0, 128, 255 }},
		{{ 0, 255, 255 }},
		{{ 0, 255, 0 }},
		{{ 255, 128, 0 }},
		{{ 255, 255, 0 }},
		{{ 255, 0, 0 }},
		{{ 255, 0, 255 }},
		{{ 255, 255, 255 }}
	};//���������ڵ�ɫ��Ķ���
	IplImage* img1 = convert_to_gray8( img[0] );
	IplImage* img2 = convert_to_gray8( img[1] );

	CvSURFParams params = cvSURFParams( 1000, 1 );
	cv::initModule_nonfree();
	cvExtractSURF( img1, 0, &objectKeypoints, &objeceDescriptors, storage, params );
	cvExtractSURF( img2, 0, &imageKeypoints, &imageDescriptors, storage, params );
	findPairs3( imageKeypoints, imageDescriptors, objectKeypoints, objeceDescriptors,ptpairs);

	if ( ptpairs.size() / 2  < 4 )
	{
		QMessageBox::information(0, "Tips", "Image Registration By SURF Failed !", 0, 0 );
		mese2=0;
		num2=0;
		return;
	}
	else
	{
		num2 = ptpairs.size() / 2;
		H = GetH3( ptpairs, imageKeypoints, objectKeypoints);////�ڸú������ù����н�����RANSAC�㷨

		if ( H )
		{
			mese2 = GetRMSE3( ptpairs, imageKeypoints, objectKeypoints, H );
			ptpairs_befores = ptpairs;////�Ѿ���RANSAC�㷨֮����ڵ����б���Ϊ������ѡ��������
			H_2.clear();
			H_2.push_back(H);
// 			double px, py;
// 			int h1, h2;
// 			px = cvmGet( H, 0, 2 );
// 			py = cvmGet( H, 1, 2 );
// 			h1 = abs(px) + 0.5;
// 			h2 = abs(py) + 0.5;
// 			IplImage* xformed;
// 			xformed = cvCreateImage( cvSize( h1 + img[1]->width, h2 + img[1]->height ), IPL_DEPTH_8U, 3 );
// 			cvWarpPerspective( img[1], xformed, H, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
// 			//͸��ͶӰ
// 			cvSetImageROI(xformed, cvRect( 0, 0, img[0]->width, img[0]->height ) );
// 			cvAddWeighted( img[0], 1, xformed, 0, 0, xformed );
// 			//Ȩ�غ�������������ֱ��ǣ���һ�����飬Ȩֵ���ڶ������飬Ȩֵ�����ʱ����ӳ�����������
// 			cvResetImageROI( xformed );
// 
// 			cvSaveImage( DstImagePath.c_str(), xformed );
// 
// 			//cvShowImage(xformed)
// 			cvReleaseImage(&xformed);

		}

	}

}

//void sy_2::Openbigresult()
//{
//	OpenResultFile1(resultImg[2]);
//}
void sy_2::OpenResultFile1(string DstImagePath)
{
	PyramidBase pyramid1; 
	pyramid1.InFilePath = DstImagePath.c_str(); 
	pyramid1.InFile = (GDALDataset*)GDALOpen(pyramid1.InFilePath, GA_ReadOnly);		//��·���е��ļ����뵽���ݼ���
	//pyramid1.FilePath = resultImg[0].c_str();
	pyramid1.CheckPyramid();

	BSTR bstr_str = _bstr_t(DstImagePath.c_str());


	//bstr_str = SysAllocString(m_strFileName2.utf16());

	HRESULT hr;

	IRasterLayerPtr pRasterLy(CLSID_RasterLayer);

	hr=pRasterLy->CreateFromFilePath(bstr_str);

	if(FAILED(hr))

		return;

	ILayerPtr pLyr(pRasterLy);

	m_pMapControl4->AddLayer(pLyr,0);

	IActiveViewPtr pActiveView(m_pMapControl4);

	m_pMapControl4->Refresh(esriViewAll);  

	//	QMessageBox::information(this, tr("Information"),tr("����"));
}

void sy_2::OpenResultFile2(string DstImagePath)
{
	GDALAllRegister();
	PyramidBase pyramid1; 
	pyramid1.InFilePath = DstImagePath.c_str(); 
	//pyramid1.FilePath = resultImg[0].c_str();
	pyramid1.InFile = (GDALDataset*)GDALOpen(pyramid1.InFilePath, GA_ReadOnly);
	pyramid1.sCheckPyramid();

	BSTR bstr_str = _bstr_t(DstImagePath.c_str());


	//bstr_str = SysAllocString(m_strFileName2.utf16());

	HRESULT hr;

	IRasterLayerPtr pRasterLy(CLSID_RasterLayer);

	hr=pRasterLy->CreateFromFilePath(bstr_str);

	if(FAILED(hr))

		return;

	ILayerPtr pLyr(pRasterLy);

	m_pMapControl4_2->AddLayer(pLyr,0);

	IActiveViewPtr pActiveView(m_pMapControl4_2);

	m_pMapControl4_2->Refresh(esriViewAll);  

	//	QMessageBox::information(this, tr("Information"),tr("����"));
}



void ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY,const char* pszFormat)
{
	GDALAllRegister();

	GDALDataset *pSrcDS = (GDALDataset*)GDALOpen(pszSrcFile, GA_ReadOnly);
	GDALDataType eDT = pSrcDS->GetRasterBand(1)->GetRasterDataType();

	int iBandCount = pSrcDS->GetRasterCount();

	// ���ݲ��з�Χȷ�����к��ͼ����
	int iDstWidth  = iSizeX;
	int iDstHeight = iSizeY;

	double adfGeoTransform[6] = {0};
	pSrcDS->GetGeoTransform(adfGeoTransform);

	// ��������ļ������ÿռ�ο���������Ϣ
	GDALDriver *poDriver = (GDALDriver *) GDALGetDriverByName(pszFormat);
	GDALDataset *pDstDS = poDriver->Create(pszDstFile, iDstWidth, iDstHeight, iBandCount, eDT, NULL);
	pDstDS->SetGeoTransform(adfGeoTransform);
	pDstDS->SetProjection(pSrcDS->GetProjectionRef());

	int *pBandMap = new int[iBandCount];
	for (int i=0; i<iBandCount; i++)
		pBandMap[i] = i+1;

	if (eDT == GDT_Byte)	// �����8bitͼ��
	{
		// ����������������Ҫ�Ļ��棬���ͼ��̫��Ӧ���÷ֿ鴦��
		char *pDataBuff = new char[iDstWidth*iDstHeight*iBandCount];	

		pSrcDS->RasterIO(GF_Read, iStartX, iStartY, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		pDstDS->RasterIO(GF_Write, 0, 0, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);

		delete(pDataBuff);
	}
	else
	{
		// �������͵�ͼ����8bit���ƣ���������Ļ������Ͳ�ͬ����
	}

	delete(pBandMap);

	GDALClose((GDALDatasetH)pSrcDS);
	GDALClose((GDALDatasetH)pDstDS);
	return  ;

}


void Find_OverlapArea ( int ilayer )
{
	//���������ͼ��ԭͼ����С����
	int iscale = static_cast<int>(pow(2.0,ilayer+1));

	rePoint.clear();	//�ο�ͼ���вü�������Сͼ���Ͻ��ڲο�ͼ���е�����
	senPoint.clear();	//����׼ͼ���вü���Сͼ���Ͻ��ڴ���׼ͼ���е�����
	refOverlap.clear();
	senOverlap.clear();
	//�����ü�ͼ���·��
	string Overlap1 = refImg[0] + "refImage_Overlap.jpg";
	string Overlap2 = senImg[0] + "senImage_Overlap.jpg";
	refOverlap.push_back(Overlap1);
	senOverlap.push_back(Overlap2);
//	refImg.push_back(Overlap1);//refImg[2]�б���ο�ͼ���ص�����ü����ͼ��·��
//	senImg.push_back(Overlap2);//senImg[2]�б������׼ͼ�����ص�����ü���ͼ��·��

	int image1_xsize,image1_ysize; ///����ο�ͼ���С��Ϣ
	int image2_xsize,image2_ysize;//�������׼ͼ���С��Ϣ

	CvMat* H;///������ʱ����H����
	H = H_2[0]; //����һ����׼����е�H���󴫵ݹ�����//�˴�Ҳ���Ժ������Ϊһ���������ݵĲ���
	double px, py; //����׼ͼ������ڲο�ͼ��ƽ�Ƶľ��룬Ҳ����Ϊ�ص��������Ͻǻ������½ǵ�����
	double h1, h2, h3, h4;
	double dx,dy;///�����ص�����Ĵ�С
	double hx,hy;///����ֿ�ͳ��ʱ��x��y����ÿһС��Ĵ�С��
	px = cvmGet( H, 0, 2 );
	py = cvmGet( H, 1, 2 );
	h1 = abs(px) + 0.5;////Ϊ�������������Լ���0.5����һ�㻹û��������
	h2 = abs(py) + 0.5;///���ص��������Ͻǻ������½ǵ����꼴Ϊ��(h1,h2)

	///������Ҫ��ȡͼ��Ĵ�С����
	//�Ȼ�ȡ�ο�ͼ����ߵ���Ϣ
	image1_xsize = img[0]->width;
	image1_ysize = img[0]->height;
//	cvShowImage("img[0]",img[0]);
//	cvWaitKey(0);

	//�ٻ�ȡ����׼ͼ�����Ϣ
	image2_xsize = img[1]->width;
	image2_ysize = img[1]->height;
//	cvShowImage("img[1]",img[1]);
//	cvWaitKey(0);

	//�����ص�����Ĵ�С
	dx = image1_xsize - h1;
	dy = image1_ysize - h2;
	int sdx = dx/4;
	int sdy = dy/4;
	Mat overlap = Mat::zeros((int)dy,(int)dx,CV_8UC1);
	Mat result;
	

 	CvSeq *CurrentKeypoints = objectKeypoints;
//	vector<int>ptpairs = ptpairs_befores;
	int num = ptpairs_befores.size()/2;
	CvSURFPoint* r1,*r2;
	cv::Point fla;
	//int image1_xsize,image1_ysize; ///����ο�ͼ���С��Ϣ
	//int image2_xsize,image2_ysize;//�������׼ͼ���С��Ϣ
	double tmpCountMinVal = 0, tmpCountMaxVal = 0;
	cv::Point minPointt,maxPoint,r11,r22;

	for (int i = 0; i < ptpairs_befores.size(); i++)
	{
		//����׼ͼ��img[1]
		r1 = (CvSURFPoint*)cvGetSeqElem(imageKeypoints, ptpairs_befores[i] );
		//cvmSet(overlap,r1->pt.x,r1->pt.y,1);
	//	fla = r1->pt;
		r11.x = r1->pt.x;
		r11.y = r1->pt.y;
		int flax,flay;
		flax = r1->pt.x;
		flay = r1->pt.y;
		overlap.at<char>(flay,flax)=1;
	//	cvCircle(img[1],r11,3,cvScalar(0,255,255),-1,8);
		i++;
		//�ο�ͼ��img[0]
	//	r2 = (CvSURFPoint*)cvGetSeqElem(objectKeypoints, ptpairs_befores[i] );
		//cvmSet(overlap,r1->pt.x,r1->pt.y,1);
		//	fla = r1->pt;
	//	r22.x = r2->pt.x;
	//	r22.y = r2->pt.y;
//		cvCircle(img[0],r22,3,cvScalar(0,255,255),-1,8);
		
	}
//	cvShowImage("img[0]",img[0]);
//	cvShowImage("img[1]",img[1]);
//	cvWaitKey(0);
	//t3=(double)(cvGetTickCount()-t3)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����

//	double t4=(double)cvGetTickCount();//��ʼ��ʱ
 	boxFilter(overlap,result,-1,Size(sdx,sdy),cv::Point(-1,-1),false);
	minMaxLoc(result,&tmpCountMinVal,&tmpCountMaxVal,&minPointt,&maxPoint);
//	t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����

	//	double t4=(double)cvGetTickCount();//��ʼ��ʱ
	CvPoint2D64f point1,point2;
	point1.x = iscale*(maxPoint.x-sdx/2);//�������׼����ͼ��1���Ͻ��ڲο�ͼ���ϵ�λ��
	point1.y = iscale*(maxPoint.y-sdy/2);
	point2.x = iscale*(maxPoint.x-sdx/2+h1);//����ο�����ͼ��2���Ͻ��ڴ���׼ͼ���ϵ�λ��
	point2.y = iscale*(maxPoint.y-sdy/2+h2);
	rePoint.push_back(point2);//���п���ԭͼ�����Ͻ����꣬Ҳ��λ��
	senPoint.push_back(point1);

	////�����ص�����ľ��ο�
//	cvRectangle(img[0],cvPoint(maxPoint.x-sdx/2+h1,maxPoint.y-sdy/2+h2),cvPoint(maxPoint.x+sdx/2+h1,maxPoint.y+sdy/2+h2),cvScalar(0,0,255),3,4,0 );
	
//	cvRectangle(img[1],cvPoint(maxPoint.x-sdx/2,maxPoint.y-sdy/2),cvPoint(maxPoint.x+sdx/2,maxPoint.y+sdy/2),cvScalar(0,255,255),2,4,0 );
	

	//double t4=(double)cvGetTickCount();//��ʼ��ʱ
	ImageCut(refImg[1].c_str(),Overlap1.c_str(),point2.x,point2.y,iscale*sdx,iscale*sdy,"GTiff");
	ImageCut(senImg[1].c_str(),Overlap2.c_str(),point1.x,point1.y,iscale*sdx,iscale*sdy,"GTiff");
	//double t4=(double)cvGetTickCount();//��ʼ��ʱ
	//t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
	//ImageCut(senImg[1].c_str(),Overlap2.c_str(),point2.x-iscale*h1,point2.y,iscale*sdx,iscale*sdy,"GTiff");
//	cvNamedWindow("OverLapArea",CV_WINDOW_AUTOSIZE);
//	cvShowImage("img[0]WithOverLapArea",img[0]);
//	cvShowImage("img[1]CutOverLapArea",img[1]);
//	cvWaitKey(0);

}



void sy_2::OnClearMapLayer()
{
	HRESULT DeleteLayer1,DeleteLayer2,DeleteLayer3,DeleteLayer4,DeleteTOCC1,DeleteTOCC2,DeleteTOCC3;
	DeleteLayer1 = m_pMapControl1->ClearLayers();////����������������������ݡ�
	DeleteLayer2 = m_pMapControl2->ClearLayers();//DeletLayer(0)ֻ������������ͼ�񣬵��ǲ����������ͼ�񣬵���ClearLayers()�����������ͼ��
	//	DeleteLayer3 = m_pMapControl3->ClearLayers();
	DeleteLayer4 = m_pMapControl4->ClearLayers();////����������������������ݡ�

	DeleteLayer1 = m_pMapControl1_2->ClearLayers();////����������������������ݡ�
	DeleteLayer2 = m_pMapControl2_2->ClearLayers();//DeletLayer(0)ֻ������������ͼ�񣬵��ǲ����������ͼ�񣬵���ClearLayers()�����������ͼ��
	//	DeleteLayer3 = m_pMapControl3->ClearLayers();
	DeleteLayer4 = m_pMapControl4_2->ClearLayers();////����������������������ݡ�

	DeleteTOCC1 = m_pTOCControl1->Update();
	DeleteTOCC2 = m_pTOCControl2->Update();
	DeleteTOCC3 = m_pTOCControl4->Update();
	DeleteTOCC1 = m_pTOCControl1_2->Update();
	DeleteTOCC2 = m_pTOCControl2_2->Update();
	DeleteTOCC3 = m_pTOCControl4_2->Update();

	if (img.size())
	{
		for ( int i = 0; i < img.size(); i ++ )
		{
			cvReleaseImage(&img[i]);
		}
		img.clear();
	}
	ui.mse->clear();
	ui.registratetime->clear();
	ui.ptpairsnum->clear();
	ui.refImageSize->clear();
	ui.senImageSize->clear();
	ui.scale->clear();
	ui.shift_x->clear();
	ui.shift_y->clear();
	ui.mse_2->clear();
	ui.registratetime_2->clear();
	ui.ptpairsnum_2->clear();
	ui.refImageSize_2->clear();
	ui.senImageSize_2->clear();
	ui.scale_2->clear();
	ui.shift_x_2->clear();
	ui.shift_y_2->clear();
	ui.rotate->clear();
	ui.rotate_2->clear();
	
	ui.mse_3->clear();
	ui.registratetime_3->clear();
	ui.ptpairsnum_3->clear();
	ui.refImageSize_3->clear();
	ui.senImageSize_3->clear();
	ui.scale_3->clear();
	ui.shift_x_3->clear();
	ui.shift_y_3->clear();
	ui.rotate_3->clear();

	ui.mse_4->clear();
	ui.registratetime_4->clear();
	ui.ptpairsnum_4->clear();
	ui.refImageSize_4->clear();
	ui.senImageSize_4->clear();
	ui.scale_4->clear();
	ui.shift_x_4->clear();
	ui.shift_y_4->clear();
	ui.rotate_4->clear();

	rfimg.clear();
	snimg.clear();
	refImg.clear();
	senImg.clear();///�ֱ𱣴����ڳ���ƥ���Сͼ��·��
	resultImg.clear();
	rePoint.clear();
	senPoint.clear();
	refLayer.clear();
	senLayer.clear();
}


void sy_2::AdBigMapRegistration()
{

	//*�������ͼ�������ļ���*//
	string dir = refImg[0] + "BigMapResultFiles"; ///�ڲο�ͼ���ļ����´����ļ��У�����Сͼ�ʹ�ͼ����׼���
	if(_access(dir.c_str(), 0) != -1)  
	{  
		cout << "�ļ����Ѵ���" <<endl;  
		//system("pause");
	}  
	else  
	{  
		cout << "�ļ��в����ڣ������ļ���" << endl;  
		_mkdir(dir.c_str()); 
		//system("pause");
	} 
	string dir1 = dir + "/";

	string sImageResultPath = dir1 + "PyramidImageRegResult.jpg";	//�ü�ͼ��ƥ����ͼ
	string BigMapResult = dir1 + "BigMapRegistrationResult.tif";	//��ͼƥ����ͼ
	resultImg.clear(); 
	resultImg.push_back(dir1);				//resultImg[0]:���ͼ�������ļ���
	resultImg.push_back(sImageResultPath);	//resultImg[1]��������ͼ����׼���·��
	resultImg.push_back(BigMapResult);		//resultImg[2]����ͼ��׼���

	int i = refLayer.size()-2;
	string refImage,senImage;	//���������ֱ�������ʱ�洢����ƥ���ͼ��·���������ݸ�ͼ����׼�ĺ���

	double t=(double)cvGetTickCount();//��ʼ��ʱ
	//	double t3=(double)cvGetTickCount();//��ʼ��ʱ

	//���ǰ��Խ���������������ѡ�񣬻��߸ı䣬�����������Ҫ�ܹ���Ӧ���ָı�
	//����Ӧ�ö�i�����жϣ���i=-1ʱ������û�н�������������вü���ֱ�ӽ���ƥ�䣬
	CvMat* H;//�����Ȳ�����ͼ���Сʱ�����⣬�ȰѴ�ͼ��׼���Ľ�
	if (i <= 0)
	{
		refImage = refImg[1];
		senImage = senImg[1];
		//������е��Ƕ�Сͼ��ƥ��
		SURF_ON_TWO(refImage,senImage,sImageResultPath);
		H = H_2[0];
	}
	else
	{

		refImage = refLayer[i+1];				//ȡ�������ڶ���ͼ����Ϊ����������ͼ���СΪ512*512.
		senImage = senLayer[i+1];		
		//
		//		double t4=(double)cvGetTickCount();//��ʼ��ʱ
		//t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		SURF_ON_TWO(refImage,senImage,sImageResultPath);//��ȡ��������������С��1000��ͼ�����ƥ��
		//double t4=(double)cvGetTickCount();//��ʼ��ʱ
		//		t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		//��i������1����˵��Ҫ�Խ�������ĳһ��ͼ�������׼�������Ҫ���вü���ƥ��Ȳ���

		Find_OverlapArea(i+1);//�Խ��ͼ����ƥ����������ͳ�ƣ���������Ȥ������в��в�����

		//t3=(double)(cvGetTickCount()-t3)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����

		//	double t4=(double)cvGetTickCount();//��ʼ��ʱ
		//���ŶԸ���Ȥ�����ͼ�����surfƥ��
		//int pathcount = simage_repath.size();
		string OverlapImage1 = refOverlap[0];
		string OverlapImage2 = senOverlap[0];
		string OverlapResult = refImg[0] + "OverLapResultImage.jpg";
		//		double t5=(double)cvGetTickCount();//��ʼ��ʱ
		//t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		SURF_ON_TWO(OverlapImage1,OverlapImage2,OverlapResult); //�ú���������Ϻ���H_2[0]�б�����ǵ�ǰͼ���任���H����
		//		t5=(double)(cvGetTickCount()-t5)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		//Cut_Count_Overlap();//�Խ��ͼ����ƥ����������ͳ�ƣ���������Ȥ������в��в�����
		//����ӱ任���H��������ȡ�任���������ԶԴ�ͼ���б任
		H = H_2[0];

		double smallhx,smallhy;///�ò�����ʾ�����ü�������Сͼ��ƽ�Ʋ���
		double bighx,bighy;//�ò�����ʾ����׼ͼ������ڲο�ͼ���ƽ�Ʋ���
		//px = cvmGet( H, 0, 2 );
		//py = cvmGet( H, 1, 2 );
		smallhx = cvmGet( H, 0, 2 ) + rePoint[0].x; //����������Ǵ���׼ͼ���и���Ȥ��������ڲο�ͼ���ƽ�ƾ���
		smallhy = cvmGet( H, 1, 2 ) + rePoint[0].y;
		//���ڻ���Ҫ֪�� �ο�ͼ���е�Сͼ����ڴ�ͼ��λ�û�������
		//���ο�ͼ���вü�����Сͼ���Ͻǵ�����
		CvMat *H1; //�����ͼ֮��ı任����������ƽ�������и���

		//����������ݲü����Ĵ���׼ͼ���е�Сͼ�ڲο�ͼ���е�λ�ã����������׼ͼ���ԭͼ�����Ͻ������ڲο�ͼ������ϵ�е�λ�ã�����˵�Ǵ���׼ͼ�����ϽǶ����ƽ������
		bighx = smallhx - senPoint[0].x;
		bighy = smallhy - senPoint[0].y;
		//����ͼƽ����ת������ʾ�ڽ�����
	
		int h1 = abs(bighx) + 0.5;
		int h2 = abs(bighy) + 0.5;
		//�Դ�ͼ��ı任�����е�ƽ�������и���
		H1 = H;
		cvmSet(H1,0,2,bighx);
		cvmSet(H1,1,2,bighy);
		t=(double)(cvGetTickCount()-t)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		//�õ��任����֮�������Ҫ����һ���µĸ����ͼ�����Դ洢������ͼ��׼�Ľ��

		double h11,h12,h21,h22,sfx,sfy,rotate;

		h11 = cvmGet(H1,0,0);
		h12 = cvmGet(H1,0,1);
		h21 = cvmGet(H1,1,0);
		h22 = cvmGet(H1,1,1);
		sfx = cvSqrt(h11*h11+h21*h21);
		sfy = cvSqrt(h22*h22+h12*h12);
		rotate = (180*atan(h21/h11))/3.1415;

		QString sbighx = QString::number(bighx,'g',6);
		ui.shift_x_2->setText(sbighx);
		QString sbighy = QString::number(bighy,'g',6);
		ui.shift_y_2->setText(sbighy);
		QString iscale1 = QString::number(sfx,'g',6);
		QString iscale2 = QString::number(sfy,'g',6);
		QString iscale = iscale1+"/"+iscale2;
		ui.scale_2->setText(iscale);
		QString srotate = QString::number(rotate,'g',6);
		ui.rotate_2->setText(srotate);


		//ResultImagePath.push_back(BigMapResult);
		GDALAllRegister();
		//��������ͼ�������ݼ���
		GDALDataset* pSrcDS1 = ( GDALDataset* )GDALOpen(refImg[1].c_str(),GA_ReadOnly); 
		GDALDataset* pSrcDS2 = ( GDALDataset* )GDALOpen(senImg[1].c_str(),GA_ReadOnly);

		GDALDataType eDT1 = pSrcDS1->GetRasterBand(1)->GetRasterDataType();
		GDALDataType eDT2 = pSrcDS2->GetRasterBand(1)->GetRasterDataType();


		int iBandCount1 = pSrcDS1->GetRasterCount();
		int iBandCount2 = pSrcDS2->GetRasterCount();

		double adfGeoTransform[6] = {0};
		//double adfGeoTransform2[6] = {0};

		pSrcDS1->GetGeoTransform(adfGeoTransform);
		pSrcDS2->GetGeoTransform(adfGeoTransform);


		if (H1)
		{

			GDALDriver *poDriver = (GDALDriver *) GDALGetDriverByName("GTIFF");

			int xsize1 = pSrcDS1->GetRasterXSize();
			int ysize1 = pSrcDS1->GetRasterYSize();
			int xsize2 = pSrcDS2->GetRasterXSize();
			int ysize2 = pSrcDS2->GetRasterYSize();

			int iDstWidth = h1 + xsize2;
			int iDstHeight = h2 + ysize2;

			//������׼���ͼ��
			GDALDataset *pDstDS = poDriver->Create(BigMapResult.c_str(), iDstWidth, iDstHeight, iBandCount1, eDT1, NULL);
			pDstDS->SetGeoTransform(adfGeoTransform);
			pDstDS->SetProjection(pSrcDS1->GetProjectionRef());

			int *pBandMap = new int[iBandCount1];
			for (int i=0; i<iBandCount1; i++)
				pBandMap[i] = i+1;

			// ����������Ҫ���������������жϣ����벻ͬ���͵Ļ��������д������������8bitͼ�������ʾ
			//��������ǽ�����ͼ�񶼱任����׼���ͼ���У�ֻ�����õ��Ƿֿ��˼��
			if (eDT1 == GDT_Byte)	// �����8bitͼ��
			{
				char *pDataBuff = new char[xsize1/2*ysize1/2*iBandCount1];
				//���ο�ͼ��任�����ͼ����
				pSrcDS1->RasterIO(GF_Read, 0,0,xsize1/2, ysize1/2, pDataBuff, xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write, 0, 0,  xsize1/2,  ysize1/2, pDataBuff,  xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS1->RasterIO(GF_Read, 0,ysize1/2,xsize1/2, ysize1/2, pDataBuff, xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write, 0, ysize1/2,  xsize1/2,  ysize1/2, pDataBuff,  xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS1->RasterIO(GF_Read, xsize1/2,0,xsize1/2, ysize1/2, pDataBuff, xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write, xsize1/2, 0,  xsize1/2,  ysize1/2, pDataBuff,  xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS1->RasterIO(GF_Read, xsize1/2,ysize1/2,xsize1/2, ysize1/2, pDataBuff, xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write, xsize1/2, ysize1/2,  xsize1/2,  ysize1/2, pDataBuff,  xsize1/2,  ysize1/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				delete(pDataBuff);
				char *pDataBuff2 = new char[xsize2/2*ysize2/2*iBandCount2];

				pSrcDS2->RasterIO(GF_Read, 0,0,xsize2/2, ysize2/2, pDataBuff2, xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write,h1,h2, xsize2/2,  ysize2/2, pDataBuff2,  xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS2->RasterIO(GF_Read, 0,ysize2/2,xsize2/2, ysize2/2, pDataBuff2, xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write,h1,h2+ysize2/2, xsize2/2,  ysize2/2, pDataBuff2,  xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS2->RasterIO(GF_Read,xsize2/2,0,xsize2/2, ysize2/2, pDataBuff2, xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write,h1+xsize2/2,h2, xsize2/2,  ysize2/2, pDataBuff2,  xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				pSrcDS2->RasterIO(GF_Read,xsize2/2,ysize2/2,xsize2/2, ysize2/2, pDataBuff2, xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);
				pDstDS->RasterIO(GF_Write,h1+xsize2/2 ,h2+ysize2/2, xsize2/2,  ysize2/2, pDataBuff2,  xsize2/2,  ysize2/2, eDT1, iBandCount1, pBandMap, 0, 0, 0);

				delete(pDataBuff2);

				//				t3=(double)(cvGetTickCount()-t3)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����

			}
			else
			{
				QMessageBox::information(NULL, "Title", "Failed!! !", QMessageBox::Yes, QMessageBox::Yes);

			}

			delete(pBandMap);

			GDALClose((GDALDatasetH)pSrcDS1);
			GDALClose((GDALDatasetH)pSrcDS2);
			GDALClose((GDALDatasetH)pDstDS);

		}


	}

	QString s1 = QString::number(t,'g',6);
	ui.registratetime_2->setText(s1);
	QString mse2 = QString::number(mese2,'g',6);
	ui.mse_2->setText(mse2);
	QString s2 = QString::number(num2,'g',6);
	ui.ptpairsnum_2->setText(s2);
	//QString size1 = QString::number(t3,'g',6);
	//ui.refImageSize->setText(size1);
	//QString size2 = QString::number(t3,'g',6);
	//ui.senImageSize->setText(size2);

	OpenResultFile1(resultImg[2]);

}



void Cut_Count_Overlap(int ilayer)		//����Ĳ���ilayer��ʾ�Ƕ�simage_repath[ilayer]·���е�ͼ����д���
{

	//ͨ�����ݵĲ���ilayer���������ͼ��Դͼ�ķŴ���
	 
	int iscale = static_cast<int>(pow(2.0,ilayer+1)); 
	

	rePoint.clear(); //����ο�ͼ���вü�������Сͼ�����Ͻ�����
	senPoint.clear();//�������׼ͼ���вü�������Сͼ�����Ͻ�����

	//�������г���ͼ���·��
	string Overlap1 = refImg[0] + "reImage_Overlap.jpg";
	string Overlap2 = senImg[0] + "senImage_Overlap.jpg";
	refOverlap.clear();
	senOverlap.clear();
	refOverlap.push_back(Overlap1);
	senOverlap.push_back(Overlap2);
//	refImg.push_back(Overlap1);//refImg[2]�б���ü��ο�ͼ��
//	senImg.push_back(Overlap2);//senImg[2]�б���ü�����׼ͼ��

	vector<int> ptpairs = ptpairs_befores;
	int num  = ptpairs.size()/2;

	int image1_xsize,image1_ysize; ///����ο�ͼ���С��Ϣ
	int image2_xsize,image2_ysize;//�������׼ͼ���С��Ϣ

	int areaCount1[10] = {0,0,0,0,0,0,0,0,0,0};///ÿһ�����ִ���һ������
	int areaCount2[10] = {0,0,0,0,0,0,0,0,0,0};///
	CvMat* H;///������ʱ����H����
	H = H_2[0]; //����һ����׼����е�H���󴫵ݹ�����//�˴�Ҳ���Ժ������Ϊһ���������ݵĲ���
	double px, py; //����׼ͼ������ڲο�ͼ��ƽ�Ƶľ��룬Ҳ����Ϊ�ص��������Ͻǻ������½ǵ�����
	double h1, h2, h3, h4;
	double dx,dy;///�����ص�����Ĵ�С
	double hx,hy;///����ֿ�ͳ��ʱ��x��y����ÿһС��Ĵ�С��
	px = cvmGet( H, 0, 2 );
	py = cvmGet( H, 1, 2 );
	h1 = abs(px) + 0.5;////Ϊ�������������Լ���0.5����һ�㻹û��������
	h2 = abs(py) + 0.5;///���ص��������Ͻǻ������½ǵ����꼴Ϊ��(h1,h2)
	
	///������Ҫ��ȡͼ��Ĵ�С����
	//�Ȼ�ȡ�ο�ͼ����ߵ���Ϣ
	image1_xsize = img[0]->width;
	image1_ysize = img[0]->height;

	//�ٻ�ȡ����׼ͼ�����Ϣ
	image2_xsize = img[1]->width;
	image2_ysize = img[1]->height;
	
	
	//�����ص�����Ĵ�С
	dx = image1_xsize - h1;
	dy = image1_ysize - h2;

	//�������hx,hy
	double fenkuai3 = 3;
	hx = dx/fenkuai3;
	hy = dy/fenkuai3;

	//IplImage* image0 = cvLoadImage(simage_repath[2].c_str());
	//Mat image0 = cvLoadImage(simage_repath[4].c_str());
	//Mat image1 = cvLoadImage(simage_senpath[4].c_str());

	////�����ص�����ľ��ο�
	//cvRectangle(img[0],cvPoint(h1,h2),cvPoint(image1_xsize,image1_ysize),cvScalar(0,0,255),3,4,0 );
	//cvNamedWindow("OverLapArea",CV_WINDOW_AUTOSIZE);
	//cvShowImage("img[0]WithOverLapArea",img[0]);
	//cvWaitKey(0);
	//cvReleaseImage(&img[0]);

	CvSeq *CurrentKeypoints = objectKeypoints;
	//CvSeq *PreKeypoints = imageKeypoints;
	//CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem(CurrentKeypoints, ptpairs[i] );
	//����Բο�ͼ�����Ѿ�ƥ������������ͳ��
	//char atom_window0[] = "OverLapArea0";  
	//char atom_window1[] = "OverLapArea1";  
	//char atom_window2[] = "OverLapArea2";  
	//char atom_window3[] = "OverLapArea3";  
	//char atom_window4[] = "OverLapArea4";  
	//char atom_window5[] = "OverLapArea5";  
	//char atom_window6[] = "OverLapArea6";  
	//char atom_window7[] = "OverLapArea7";  
	//char atom_window8[] = "OverLapArea8";  

	//char atom1_window0[] = "OverLap1Area0";  
	//char atom1_window1[] = "OverLap1Area1";  
	//char atom1_window2[] = "OverLap1Area2";  
	//char atom1_window3[] = "OverLap1Area3";  
	//char atom1_window4[] = "OverLap1Area4";  
	//char atom1_window5[] = "OverLap1Area5";  
	//char atom1_window6[] = "OverLap1Area6";  
	//char atom1_window7[] = "OverLap1Area7";  
	//char atom1_window8[] = "OverLap1Area8";  

	//ͳ������������Ķ��� 

	for (int i = 0; i < ptpairs.size(); i++)
	{
		CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem(objectKeypoints, ptpairs[i+1] );


		if (r1->pt.x >= h1 && r1->pt.x <= (h1+hx) )
		{
			if (r1->pt.y >= h2 && r1->pt.y <= (h2+hy))
			{
				areaCount1[0]++;

//				rectangle(img[0],cvPoint(h1,h2),cvPoint(h1+hx,h2+hy),cvScalar(255,0,255),2,4,0 );
//				circle(img[0],r1->pt,3,cvScalar(0,255,255),-1,8);

				////cvNamedWindow("OverLapArea0",CV_WINDOW_AUTOSIZE);
				//imshow(atom_window0,img[0]);

			} 
			else if(r1->pt.y > (h2+hy) && r1->pt.y <= (image1_ysize-hy) )
			{
				areaCount1[3]++;

//				rectangle(img[0],cvPoint(h1,h2+hy),cvPoint(h1+hx,image1_ysize-hy),cvScalar(255,0,255),2,4,0 );
//				circle(img[0],r1->pt,3,cvScalar(0,255,255),-1,8);

				////cvNamedWindow("OverLapArea3",CV_WINDOW_AUTOSIZE);
				//imshow(atom_window3,img[0]);

			}
			else if (r1->pt.y > image1_ysize-hy && r1->pt.y <= image1_ysize  )
			{
				areaCount1[6]++;
//				rectangle(img[0],cvPoint(h1,image1_ysize-hy),cvPoint(h1+hx,image1_ysize),cvScalar(255,0,255),2,4,0 );
//				circle(img[0],r1->pt,3,cvScalar(0,255,255),-1,8);

				////cvNamedWindow("OverLapArea6",CV_WINDOW_AUTOSIZE);
				//imshow(atom_window6,img[0]);
			}

		}
		else if ( r1->pt.x > h1+hx && r1->pt.x <= image1_xsize-hx )
		{
			if (r1->pt.y >= h2 && r1->pt.y <= (h2+hy))
			{
				areaCount1[1]++;
//				rectangle(img[0],cvPoint(h1+hx,h2),cvPoint(image1_xsize-hx,image1_ysize),cvScalar(255,0,255),2,4,0 );
//				circle(img[0],r1->pt,3,cvScalar(0,255,255),-1,8);
				////cvNamedWindow("OverLapArea1",CV_WINDOW_AUTOSIZE);
				//imshow(atom_window1,img[0]);
			} 
			else if(r1->pt.y > (h2+hy) && r1->pt.y <= (image1_ysize-hy) )
			{
				areaCount1[4]++;
				//rectangle(img[0],cvPoint(h1+hx,h2+hy),cvPoint(image1_xsize-hx,image1_ysize-hy),cvScalar(255,0,255),2,4,0 );
				//circle(img[0],r1->pt,3,cvScalar(0,255,255),-1,8);
				////cvNamedWindow("OverLapArea4",CV_WINDOW_AUTOSIZE);
				//imshow(atom_window4,img[0]);
			}
			else if (r1->pt.y > image1_ysize-hy && r1->pt.y <= image1_ysize  )
			{
				areaCount1[7]++;
//				rectangle(img[0],cvPoint(h1+hx,image1_ysize-hy),cvPoint(image1_xsize-hx,image1_ysize),cvScalar(255,0,255),2,4,0 );
//				circle(img[0],r1->pt,3,cvScalar(0,255,255),-1,8);
				////cvNamedWindow("OverLapArea7",CV_WINDOW_AUTOSIZE);
				//imshow(atom_window7,img[0]);
			}

		}
		else if ( r1->pt.x > image1_xsize-hx && r1->pt.x <= image1_xsize )
		{
			if (r1->pt.y >= h2 && r1->pt.y <= (h2+hy))
			{
				areaCount1[2]++;
//				rectangle(img[0],cvPoint(image1_xsize-hx,h2),cvPoint(image1_xsize,image1_ysize),cvScalar(255,0,255),2,4,0 );
//				circle(img[0],r1->pt,3,cvScalar(0,255,255),-1,8);
				////cvNamedWindow("OverLapArea2",CV_WINDOW_AUTOSIZE);
				//imshow(atom_window2,img[0]);
			} 
			else if(r1->pt.y > (h2+hy) && r1->pt.y <= (image1_ysize-hy) )
			{
				areaCount1[5]++;
//				rectangle(img[0],cvPoint(image1_xsize-hx,h2+hy),cvPoint(image1_xsize,image1_ysize-hy),cvScalar(255,0,255),2,4,0 );
//				circle(img[0],r1->pt,3,cvScalar(0,255,255),-1,8);
				////cvNamedWindow("OverLapArea5",CV_WINDOW_AUTOSIZE);
				//imshow(atom_window5,img[0]);
			}
			else if (r1->pt.y > image1_ysize-hy && r1->pt.y <= image1_ysize  )
			{
				areaCount1[8]++;
//				rectangle(img[0],cvPoint(image1_xsize-hx,image1_ysize-hy),cvPoint(image1_xsize,image1_ysize),cvScalar(255,0,255),2,4,0 );
//				circle(img[0],r1->pt,3,cvScalar(0,255,255),-1,8);
				////cvNamedWindow("OverLapArea8",CV_WINDOW_AUTOSIZE);
				//imshow(atom_window8,img[0]);
			}
		}
		i++;//��Ϊptpairs�д洢����һ��һ�Ե�������������ţ�����ֵ��Ҫ��ȡ�����вο�ͼ�������������ţ���Ӧ�ø�һ��һȡ
	}


	int maxvaule1 = areaCount1[0]; //��ʾ�����е����ֵ
	int maxnum1 = 0;	//������������ֵ����ţ�Ҳ����Ӧ����
	//ѭ��������ͳ��������������,��������
	for (int i = 1; i < 9; i++)
	{
		
		if (areaCount1[i] > maxvaule1 )
		{
			maxvaule1 = areaCount1[i];
			maxnum1 = i;
		}

	}
// 	char ch;
// 	itoa(maxnum,ch,10);
	//�õ������������������ţ��Ͷ�Ӧ�ػ�����
	//rectangle(img[0],cvPoint(h1,h2),cvPoint(h1+hx,h2+hy),cvScalar(0,0,255),3,4,0 );
	
	switch (maxnum1)
	{
	case 0:
		//�ο�ͼ��Ĳü�
		CvPoint2D64f point0;
		point0.x= iscale*h1;///���У�Ӧ�ð���Щ������Ϊ������
		point0.y = iscale*h2;
		rePoint.push_back(point0);
	//	rectangle(img[0],cvPoint(h1,h2),cvPoint(h1+hx,h2+hy),cvScalar(0,255,255),2,4,0 );
	//	imshow(atom_window0,img[0]);
	
		//����׼ͼ��Ĳü�
		CvPoint2D64f point00;
		point00.x = 0;///���У�Ӧ�ð���Щ������Ϊ������
		point00.y = 0;
		senPoint.push_back(point00);

	//	rectangle(img[1],cvPoint(0,0),cvPoint(hx,hy),cvScalar(255,255,0),2,4,0 );
	//	imshow(atom1_window0,img[1]);
		ImageCut(senImg[1].c_str(),senOverlap[0].c_str(),0,0,iscale*hx,iscale*hy,"GTiff");
		ImageCut(refImg[1].c_str(),refOverlap[0].c_str(),point0.x,point0.y,iscale*hx,iscale*hy,"GTiff");
		//waitKey(0);
		break;
	case 1:
		CvPoint2D64f point1;
		point1.x = iscale*(h1+hx);///���У�Ӧ�ð���Щ������Ϊ������
		point1.y = iscale*h2;
		rePoint.push_back(point1);

	//	rectangle(img[0],cvPoint(h1+hx,h2),cvPoint(image1_xsize-hx,h2+hy),cvScalar(0,255,255),2,4,0 );
	//	imshow(atom_window1,img[0]);
		ImageCut(refImg[1].c_str(),refOverlap[0].c_str(),point1.x,point1.y,iscale*(image1_xsize-2*hx),iscale*hy,"GTiff");
		CvPoint2D64f point11;
		point11.x = iscale*hx;///���У�Ӧ�ð���Щ������Ϊ������
		point11.y = 0;
		senPoint.push_back(point11);
	//	rectangle(img[1],cvPoint(hx,0),cvPoint(dx-hx,hy),cvScalar(255,255,0),2,4,0 );
	//	imshow(atom1_window1,img[1]);
		ImageCut(senImg[1].c_str(),senOverlap[0].c_str(),point11.x,point11.y,iscale*(dx-hx*2),iscale*hy,"GTiff");
		//waitKey(0);
		break;
	case 2:
		CvPoint2D64f point2;
		point2.x =iscale*(image1_xsize-hx);///���У�Ӧ�ð���Щ������Ϊ������
		point2.y = iscale*h2;
		rePoint.push_back(point2);
	//	rectangle(img[0],cvPoint(image1_xsize-hx,h2),cvPoint(image1_xsize,h2+hy),cvScalar(0,255,255),2,4,0 );
	//	imshow(atom_window2,img[0]);
		ImageCut(refImg[1].c_str(),refOverlap[0].c_str(),point2.x,point2.y,iscale*hx,iscale*hy,"GTiff");
		CvPoint2D64f point22;
		point22.x = iscale*(dx-hx);///���У�Ӧ�ð���Щ������Ϊ������
		point22.y = 0;
		senPoint.push_back(point22);

	//	rectangle(img[1],cvPoint(dx-hx,0),cvPoint(dx,hy),cvScalar(255,255,0),2,4,0 );
	//	imshow(atom1_window2,img[1]);
		ImageCut(senImg[1].c_str(),senOverlap[0].c_str(),point22.x,point22.y,iscale*hx,iscale*hy,"GTiff");
		//waitKey(0);
		break;
	case 3:
		CvPoint2D64f point3;
		point3.x = iscale*h1;///���У�Ӧ�ð���Щ������Ϊ������
		point3.y = iscale*(h2+hy);
		rePoint.push_back(point3);

	//	rectangle(img[0],cvPoint(h1,h2+hy),cvPoint(h1+hx,image1_ysize-hy),cvScalar(0,255,255),2,4,0 );
	//	imshow(atom_window3,img[0]);
		ImageCut(refImg[1].c_str(),refOverlap[0].c_str(),point3.x,point3.y,iscale*hx,iscale*hy,"GTiff");
		CvPoint2D64f point33;
		point33.x = 0;///���У�Ӧ�ð���Щ������Ϊ������
		point33.y = iscale*hy;
		senPoint.push_back(point33);
	//	rectangle(img[1],cvPoint(0,hy),cvPoint(hx,dy-hy),cvScalar(255,255,0),2,4,0 );
	//	imshow(atom1_window3,img[1]);
		ImageCut(senImg[1].c_str(),senOverlap[0].c_str(),point33.x,point33.y,iscale*hx,iscale*(dy-2*hy),"GTiff");
		//waitKey(0);
		break;
	case 4:	
		CvPoint2D64f point4;
		point4.x = iscale*(h1+hx);///���У�Ӧ�ð���Щ������Ϊ������
		point4.y = iscale*(h2+hy);
		rePoint.push_back(point4);
	//	rectangle(img[0],cvPoint(h1+hx,h2+hy),cvPoint(image1_xsize-hx,image1_ysize-hy),cvScalar(0,255,255),2,4,0 );
	//	imshow(atom_window4,img[0]);
		ImageCut(refImg[1].c_str(),refOverlap[0].c_str(),point4.x,point4.y,iscale*(image1_xsize-2*hx),iscale*(image1_ysize-2*hy),"GTiff");

		CvPoint2D64f point44;
		point44.x = iscale*hx;///���У�Ӧ�ð���Щ������Ϊ������
		point44.y = iscale*hy;
		senPoint.push_back(point44);

//		rectangle(img[1],cvPoint(hx,hy),cvPoint(dx-hx,dy-hy),cvScalar(255,255,0),2,4,0 );
//		imshow(atom1_window4,img[1]);
		ImageCut(senImg[1].c_str(),senOverlap[0].c_str(),point44.x,point44.y,iscale*(dx-2*hx),iscale*(dy-2*hy),"GTiff");
		//waitKey(0);
		break;
	case 5:
		CvPoint2D64f point5;
		point5.x = iscale*(image1_xsize-hx);///���У�Ӧ�ð���Щ������Ϊ������
		point5.y = iscale*(h2+hy);
		rePoint.push_back(point5);

//		rectangle(img[0],cvPoint(image1_xsize-hx,h2+hy),cvPoint(image1_xsize,image1_ysize-hy),cvScalar(0,255,255),2,4,0 );
//		imshow(atom_window5,img[0]);
		ImageCut(refImg[1].c_str(),refOverlap[0].c_str(),point5.x,point5.y,iscale*hx,iscale*(image1_ysize-2*hy),"GTiff");

		CvPoint2D64f point55;
		point55.x = iscale*(dx-hx);///���У�Ӧ�ð���Щ������Ϊ������
		point55.y = iscale*hy;
		senPoint.push_back(point55);
//		rectangle(img[1],cvPoint(dx-hx,hy),cvPoint(dx,dy-hy),cvScalar(255,255,0),2,4,0 );
//		imshow(atom1_window5,img[1]);
		ImageCut(senImg[1].c_str(),senOverlap[0].c_str(),point55.x,point55.y,iscale*hx,iscale*(dy-2*hy),"GTiff");


		//waitKey(0);
		break;
	case 6:
		CvPoint2D64f point6;
		point6.x = iscale*h1;///���У�Ӧ�ð���Щ������Ϊ������
		point6.y = iscale*(image1_ysize-hy);
		rePoint.push_back(point6);

//		rectangle(img[0],cvPoint(h1,image1_ysize-hy),cvPoint(h1+hx,image1_ysize),cvScalar(0,255,255),2,4,0 );
//		imshow(atom_window6,img[0]);
		ImageCut(refImg[1].c_str(),refOverlap[0].c_str(),point6.x,point6.y,iscale*hx,iscale*hy,"GTiff");

		CvPoint2D64f point66;
		point66.x = 0;///���У�Ӧ�ð���Щ������Ϊ������
		point66.y = iscale*(dy-hy);
		senPoint.push_back(point66);
//		rectangle(img[1],cvPoint(0,dy-hy),cvPoint(hx,dy),cvScalar(255,255,0),2,4,0 );
//		imshow(atom1_window6,img[1]);
		ImageCut(senImg[1].c_str(),senOverlap[0].c_str(),point66.x,point66.y,iscale*hx,iscale*hy,"GTiff");

	//	waitKey(0);
		break;
	case 7:
		CvPoint2D64f point7;
		point7.x = iscale*(h1+hx);///���У�Ӧ�ð���Щ������Ϊ������
		point7.y = iscale*(image1_ysize-hy);
		rePoint.push_back(point7);

//		rectangle(img[0],cvPoint(h1+hx,image1_ysize-hy),cvPoint(image1_xsize-hx,image1_ysize),cvScalar(0,255,255),2,4,0 );
//		imshow(atom_window7,img[0]);
		ImageCut(refImg[1].c_str(),refOverlap[0].c_str(),point7.x,point7.y,iscale*(image1_xsize-2*hx),iscale*hy,"GTiff");

		CvPoint2D64f point77;
		point77.x = iscale*hx;///���У�Ӧ�ð���Щ������Ϊ������
		point77.y = iscale*(dy-hy);
		senPoint.push_back(point77);
//		rectangle(img[1],cvPoint(hx,dy-hy),cvPoint(dx-hx,dy),cvScalar(255,255,0),2,4,0 );
//		imshow(atom1_window7,img[1]);
		ImageCut(senImg[1].c_str(),senOverlap[0].c_str(),point77.x,point77.y,iscale*(dx-2*hx),iscale*hy,"GTiff");


		//waitKey(0);
		break;
	case 8:
		CvPoint2D64f point8;
		point8.x = iscale*(image1_xsize-hx);///���У�Ӧ�ð���Щ������Ϊ������
		point8.y = iscale*(image1_ysize-hy);
		rePoint.push_back(point8);

	//	rectangle(img[0],cvPoint(image1_xsize-hx,image1_ysize-hy),cvPoint(image1_xsize,image1_ysize),cvScalar(0,255,255),2,4,0 );
	//	imshow(atom_window8,img[0]);
		ImageCut(refImg[1].c_str(),refOverlap[0].c_str(),point8.x,point8.y,iscale*hx,iscale*hy,"GTiff");

		CvPoint2D64f point88;
		point88.x = iscale*(dx-hx);///���У�Ӧ�ð���Щ������Ϊ������
		point88.y = iscale*(dy-hy);
		senPoint.push_back(point88);
	//	rectangle(img[1],cvPoint(dx-hx,dy-hy),cvPoint(dx,dy),cvScalar(255,255,0),2,4,0 );
	//	imshow(atom1_window8,img[1]);
		ImageCut(senImg[1].c_str(),senOverlap[0].c_str(),point88.x,point88.y,iscale*hx,iscale*hy,"GTiff");


		//waitKey(0);
		break;
	}
	

}


//SURF+�������㷨
void sy_2::DSurfSpeed()
{

	//*�������ͼ�������ļ���*//
	string dir = refImg[0] + "DSURFResultFiles"; ///�ڲο�ͼ���ļ����´����ļ��У�����Сͼ�ʹ�ͼ����׼���
	if(_access(dir.c_str(), 0) != -1)  
	{  
		cout << "�ļ����Ѵ���" <<endl;  
		//system("pause");
	}  
	else  
	{  
		cout << "�ļ��в����ڣ������ļ���" << endl;  
		_mkdir(dir.c_str()); 
		//system("pause");
	} 
	string dir1 = dir + "/";

	string sImageResultPath = dir1 + "PyramidImageRegResult.jpg";	//������ͼ��ƥ����ͼ
	string BigMapResult = dir1 + "BigMapRegistrationResult.jpg";	//ԭͼƥ����ͼ
	resultImg.clear(); 
	resultImg.push_back(dir1);				//resultImg[0]:���ͼ�������ļ���
	resultImg.push_back(sImageResultPath);	//resultImg[1]��������ͼ����׼���·��
	resultImg.push_back(BigMapResult);		//resultImg[2]��ԭͼ��׼���
	
	//����Ĭ�������ѡ��1/2��������ͼ�����ƥ��
	int i = (1/downScale_qt)/4+1;
	iLayer1 = i;
	iLayer2 = iLayer1;
	string refImage,senImage;	//���������ֱ�������ʱ�洢����ƥ���ͼ��·���������ݸ�ͼ����׼�ĺ���
	
	double downrate = downScale_qt;//����qt�����ȡ�Ĳ���/����Ĭ�ϲ���������������������������ԭʼͼ����н�����



	double t=(double)cvGetTickCount();//��ʼ��ʱ


	//	double t3=(double)cvGetTickCount();//��ʼ��ʱ

	//���ǰ��Խ���������������ѡ�񣬻��߸ı䣬�����������Ҫ�ܹ���Ӧ���ָı�
	//����Ӧ�ö�i�����жϣ���i=-1ʱ������û�н�������������вü���ֱ�ӽ���ƥ�䣬
	CvMat* H;//�����Ȳ�����ͼ���Сʱ�����⣬�ȰѴ�ͼ��׼���Ľ�
	if (i <= 0)
	{
		refImage = refImg[1];
		senImage = senImg[1];
		//������е��Ƕ�Сͼ��ƥ��
		SURF_ON_TWO(refImage,senImage,sImageResultPath);
		H = H_2[0];
	}
	else
	{


		refImage = refLayer[iLayer1];				//ȡ�������ڶ���ͼ����Ϊ����������ͼ���СΪ512*512.
		senImage = senLayer[iLayer2];		
		//
		//		double t4=(double)cvGetTickCount();//��ʼ��ʱ
		//t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		SURF_ON_TWO(refImage,senImage,sImageResultPath);//��ȡ��������������С��1000��ͼ�����ƥ��
		H = H_2[0];

		double smallhx,smallhy,ha,hb;///�ò�����ʾ�����ü�������Сͼ��ƽ�Ʋ���
		double bighx,bighy;//�ò�����ʾ����׼ͼ������ڲο�ͼ���ƽ�Ʋ���
		//px = cvmGet( H, 0, 2 );
		//py = cvmGet( H, 1, 2 );
		smallhx = cvmGet( H, 0, 2 ); //����������Ǵ���׼ͼ���и���Ȥ��������ڲο�ͼ���ƽ�ƾ���
		smallhy = cvmGet( H, 1, 2 );
		ha = cvmGet(H,2,0);
		hb = cvmGet(H,2,1);
		//���ڻ���Ҫ֪�� �ο�ͼ���е�Сͼ����ڴ�ͼ��λ�û�������
		//���ο�ͼ���вü�����Сͼ���Ͻǵ�����
		CvMat *H1; //�����ͼ֮��ı任����������ƽ�������и���

		//����������ݲü����Ĵ���׼ͼ���е�Сͼ�ڲο�ͼ���е�λ�ã����������׼ͼ���ԭͼ�����Ͻ������ڲο�ͼ������ϵ�е�λ�ã�����˵�Ǵ���׼ͼ�����ϽǶ����ƽ������
		bighx = smallhx / downrate;
		bighy = smallhy / downrate;
		//����ͼƽ����ת������ʾ�ڽ�����

		//int h1 = abs(bighx) + 0.5;
		//int h2 = abs(bighy) + 0.5;
		//�Դ�ͼ��ı任�����е�ƽ�������и���
		H1 = H;
		cvmSet(H1,0,2,bighx);
		cvmSet(H1,1,2,bighy);
		cvmSet(H1,2,0,ha*downrate);
		cvmSet(H1,2,1,hb*downrate);
		t=(double)(cvGetTickCount()-t)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		//�õ��任����֮�������Ҫ����һ���µĸ����ͼ�����Դ洢������ͼ��׼�Ľ��

		double h11,h12,h21,h22,sfx,sfy,rotate;

		h11 = cvmGet(H1,0,0);
		h12 = cvmGet(H1,0,1);
		h21 = cvmGet(H1,1,0);
		h22 = cvmGet(H1,1,1);
		sfx = cvSqrt(h11*h11+h21*h21);
		sfy = cvSqrt(h22*h22+h12*h12);
		rotate = (180*atan(h21/h11))/3.1415;
		CvPoint p1,p2,p3,p4; 
		p2.x=bighx;  p2.y=bighy;

		p1.x=(snimg[0]->width)*h11+bighx;  p1.y=h21*(snimg[0]->width)+bighy; 
		p3.x=h12*(snimg[0]->height)+bighx; p3.y=h22*(snimg[0]->height)+bighy; 
		p4.x=(snimg[0]->width)*h11+h12*(snimg[0]->height)+bighx; p4.y=h21*(snimg[0]->width)+h22*(snimg[0]->height)+bighy; 

		cvLine(rfimg[0],p1,p2,CV_RGB(255,0,0),2,CV_AA);
		cvLine(rfimg[0],p2,p3,CV_RGB(255,0,0),2,CV_AA);
		cvLine(rfimg[0],p3,p4,CV_RGB(255,0,0),2,CV_AA);
		cvLine(rfimg[0],p4,p1,CV_RGB(255,0,0),2,CV_AA);
// 
 		QString surftime = QString::number(t,'g',6);
 		ui.registratetime_4->setText(surftime);
		QString surfptpairs = QString::number(num2,'g',6);
		ui.ptpairsnum_4->setText(surfptpairs);
		QString surfmes = QString::number(mese2,'g',6);
		ui.mse_4->setText(surfmes);

		QString bigx = QString::number(bighx,'g',6);
		ui.shift_x_4->setText(bigx);
		QString bigy = QString::number(bighy,'g',6);
		ui.shift_y_4->setText(bigy);

		QString surfrotate = QString::number(rotate,'g',6);
		ui.rotate_4->setText(surfrotate);
		
		QString iscale1 = QString::number(sfx,'g',6);
		QString iscale2 = QString::number(sfy,'g',6);
		QString iscale = iscale1+"/"+iscale2;
		ui.scale_4->setText(iscale);
 		cvSaveImage( resultImg[2].c_str(), rfimg[0] );
	}

	OpenResultFile2(resultImg[2]);
	xishu.clear();
	lujing.clear();
}

//SURF+������+�ֿ��㷨

void sy_2::FenKuai_DSURF()
{
	//img.clear();
	//img[0] = cvLoadImage(refImg[1].c_str());
	//img[1] = cvLoadImage(senImg[1].c_str());

	string downref = refImg[0] + "DownRefImg.jpg";
	string downsen = senImg[0] + "DownSenImg.jpg";
	refLayer.push_back(downref);
	senLayer.push_back(downsen);
	double downrate = downScale_qt;//��ȡ����������/Ĭ��Ϊ1/4
	ResizeImage(refImg[1].c_str(),downref.c_str(),downrate);
	ResizeImage(senImg[1].c_str(),downsen.c_str(),downrate);

	fenImage.clear();
	//*�������ͼ�������ļ���*//
	string dir = refImg[0] + "FenKuaiResultFiles"; ///�ڲο�ͼ���ļ����´����ļ��У�����Сͼ�ʹ�ͼ����׼���
	if(_access(dir.c_str(), 0) != -1)   
		cout << "�ļ����Ѵ���" <<endl;  
	else  
	{  
		cout << "�ļ��в����ڣ������ļ���" << endl;  
		_mkdir(dir.c_str()); 
	} 
	string dir2 = refImg[0] + "FenKuai"; ///�ڲο�ͼ���ļ����´����ļ��У�����Сͼ�ʹ�ͼ����׼���
	if(_access(dir2.c_str(), 0) != -1)   
		cout << "�ļ����Ѵ���" <<endl;  
	else  
	{  
		cout << "�ļ��в����ڣ������ļ���" << endl;  
		_mkdir(dir2.c_str()); 
	} 
	dir2 = dir2+"/";
	fenImage.push_back(dir2);//fenImage[0]:�ֿ�ͼ�������ļ���
	string dir1 = dir + "/";

	string downResult = dir1 + "DownResult.jpg";	//ͼ��ƥ����ͼ
	string Result = dir1 + "Result.jpg";	//��ͼƥ����ͼ
	resultImg.clear(); 
	resultImg.push_back(dir1);				//resultImg[0]:���ͼ�������ļ���
	resultImg.push_back(downResult);	//resultImg[1]��������ͼ����׼���·��
	resultImg.push_back(Result);		//resultImg[2]����ͼ��׼���

	double t=(double)cvGetTickCount();//��ʼ��ʱ
	//	double t3=(double)cvGetTickCount();//��ʼ��ʱ	

	//���ȶԽ�������ͼ����зֿ�ƥ���㷨
	cv::initModule_nonfree();
	CvMat* H;//�����Ȳ�����ͼ���Сʱ�����⣬�ȰѴ�ͼ��׼���Ľ�
	CvMat h;
	CvMat* H1=&h;
	double a[9];
	double px,py;
	double sfx,sfy;
	double R11,R12,R21,R22,R13,R23,R33;
	vector<int>ptpairs2;
	CvMemStorage* storage2=cvCreateMemStorage(0);
	static CvScalar colors1[] = 
	{
		{{0,0,255}},
		{{0,128,255}},
		{{0,255,255}},
		{{0,255,0}},
		{{255,128,0}},
		{{255,255,0}},
		{{255,0,0}},
		{{255,0,255}},
		{{255,255,255}}
	};//���������ڵ�ɫ��Ķ���
	CvSURFParams params11 = cvSURFParams(1000, 0);

	int iscal = pow(2.0,iLayer1);

	if (downref == downsen)
	{
		QMessageBox::information( 0, "Tips", "Please open another image!", 0, 0 );
		return;
	}
	
	IplImage* rImg = cvLoadImage(downref.c_str());//�Խ�����ͼ����зֿ�
	IplImage* sImg = cvLoadImage(downsen.c_str());


	int width11 = rImg->width;
	int height11 = rImg->height;
	int width12 = sImg->width;
	int height12 = sImg->height;

	int m,n;   //n��m��

	m=width11/width12;
	n=height11/height12;
	if(width11%width12>0.5*width12)
		m++;
	if(height11%height12>0.5*height12)
		n++;
	if(flag11==1)
	{
		m+=flag1;
		n+=flag1;
	}
	if(flag22==1)
	{
		m-=flag2;
		n-=flag2;
	}

	ww=((double)width11)/((double)m);
	hh=((double)height11)/((double)n);
	IplImage* temp;
	char fenname[1024]={};
	char* bb;
	string fenkuaifilename;
	temp=cvCreateImage(cvSize(ww,hh) , IPL_DEPTH_8U, 3);
	setlocale(LC_ALL,"C"); 
	for(int j=0;j<n;j++)
	{
		for(int i=0;i<m;i++)
		{
			cvSetImageROI(rImg,cvRect(i*ww,j*hh,ww,hh));//��ԭʼͼ����в���
			cvCopy(rImg,temp);
			cvResetImageROI(rImg);
			sprintf(fenname,"%s%d_%d.jpg",dir2.c_str(),j,i);
			bb=fenname;
			fenkuaifilename = fenname;
			cvSaveImage(bb,temp);
			fenImage.push_back(fenkuaifilename);//fenImage[1]���Ժ󱣴�ֳɵ�С��ͼ��·��

		}

	}

	/**********************�õ�Сͼ�������**************************************/
	extern double	m_ColorPow;
	extern double	m_CuvePow;
	extern double	m_ShapePow;

	extern int m_KeyMainColorSig[111]; // ��ѯСͼ�������
	extern double m_KeyCuve[15];
	extern double m_KeyShape[8];
	extern int m_Color[111];//����ͼ������
	extern double m_Cuve[15];
	extern double m_Shape[8];
//	const char* strsmall;
	/**********************�õ�Сͼ�������**************************************/
	CString strsmall;

	//fen2Ӧ���Ǵ���׼ͼ��
	QString fen2b1name=QString::fromStdString(downsen);
	const QByteArray textfen2b1 = fen2b1name.toLocal8Bit();
	const char *fen2nameb1 = textfen2b1.data();
	strsmall=fen2nameb1;

	CxImage m_imgLoaded;
	int h0,w0;
	m_imgLoaded.Load(fen2nameb1);
	h0=m_imgLoaded.GetHeight();
	w0=m_imgLoaded.GetWidth();

	IplImage* img1111 = cvLoadImage(fen2nameb1, 1);//Сͼ
	int k;
	for(k=0; k<111; k++)
	{
		m_KeyMainColorSig[k] = 0;
	}
	for(k=0; k<15; k++)
	{
		m_KeyCuve[k]=0;
	}
	for(k=0;k<8;k++)
	{
		m_KeyShape[k]=0;
	}

	ColorSplit(m_imgLoaded);//ת����HSI�ռ�  ��ɫ
	CuveSplit(m_imgLoaded);    //ת�����ݶ�   ����
	ShapeSplit(m_imgLoaded);   //����canny�����Ե  ��״

	for(k=0;k<111;k++)
		m_KeyMainColorSig[k] = m_Color[k];
	for(k=0; k<15; k++)
		m_KeyCuve[k]=m_Cuve[k];
	for(k=0;k<8;k++)
		m_KeyShape[k]=m_Shape[k];
	///�����Ǵ�ͼ��һ����ͼB1������

	/*********************�õ�Сͼ�������***************************************/
	/***********************************************************************/

	ColorPow=m_ColorPow;
	CuvePow=m_CuvePow;
	ShapePow=m_ShapePow;
	if (ColorPow==0&&CuvePow==0&&ShapePow==0)
	{
		ColorPow=1;
		CuvePow=1;
		ShapePow=1;
	}
	double plus = ColorPow + CuvePow + ShapePow;
	ColorPow /= plus;
	CuvePow /= plus;
	ShapePow /= plus;

	//////////////////////���Ըĳ�ȫ�ֱ���
	QString m_strBeginPath;		//��ʼ·��
	QString m_strExtension;     //�����ļ����е��ļ�����
	m_strExtension = "*.jpg";
	//m_strBeginPath = "E:\\fen1";
	m_strBeginPath =QString::fromStdString(fenImage[0]);

	CariRecursive(m_strBeginPath, m_strExtension);//�����������Ƶ�


	int number=xishu.size();
	int minNum=0;
	double zuixiao;
	zuixiao=xishu[0];
	for (int i=0;i<number;i++)////�ҵ���С��ϵ��
	{

		if(zuixiao>xishu[i])
		{
			zuixiao=xishu[i];
			minNum=i;
		}			

	}


	QString tp;
	tp=lujing[minNum];

	QString dir_s=tp;
	const QByteArray text = dir_s.toLocal8Bit();
	const char *tp1 = text.data();


	IplImage* temp1=cvLoadImage(tp1,1);//ϵ����С���ƶ����

	int len1=strlen(tp1);

	QCharRef aaa=tp[len1-7];
	QCharRef bbb=tp[len1-5];
	QChar aaaa(aaa);
	char aaaaa=aaaa.toAscii();
	QChar bbbb(bbb);
	char bbbbb=bbbb.toAscii();
	int mm=aaaaa-'0';
	int nn=bbbbb-'0';

	/*cvSetImageROI(img[0], cvRect(nn*ww, mm*hh, ww, hh) );*/

	IplImage* img1=convert_to_gray8(/*img[0]*/temp1);//�ҵ���Сͼ
	IplImage* img2=convert_to_gray8(img1111);//B1

	CvSeq *PreKeypoints3 = 0, *PreDescriptors3 = 0;
	CvSeq *CurrentKeypoints3 = 0, *CurrentDescriptors3 = 0;

	////////////////////
//	CvMemStorage* storage2=cvCreateMemStorage(0);
//	CvSURFParams params11 = cvSURFParams(1000, 0);

	

	cvExtractSURF( img1, 0, &PreKeypoints3, &PreDescriptors3, storage2, params11);
	cvExtractSURF( img2, 0, &CurrentKeypoints3, &CurrentDescriptors3, storage2, params11);
//	vector<int>ptpairs2;
	findPairs3( CurrentKeypoints3, CurrentDescriptors3, PreKeypoints3, PreDescriptors3, ptpairs2 );
	num1=ptpairs2.size()/2;
	H = GetH3(ptpairs2,CurrentKeypoints3,PreKeypoints3);


	if( H )
	{

		mese1=GetRMSE3(ptpairs2,CurrentKeypoints3,PreKeypoints3,H);
		H_1.clear();
		H_1.push_back(H);


		double ppx,ppy,RR02,RR12,RRa,RRb,rotate;
		ppx=nn*ww;
		ppy=mm*hh;
		RR02=cvmGet(H,0,2)+ppx;
		cvmSet(H,0,2,RR02/downrate);
		RR12=cvmGet(H,1,2)+ppy;
		cvmSet(H,1,2,RR12/downrate);
		RRa = cvmGet(H,2,0);
		RRb = cvmGet(H,2,1);
		cvmSet(H,2,0,RRa*downrate);
		cvmSet(H,2,1,RRb*downrate);
		cvInitMatHeader(H1,3,3,CV_64FC1,a,CV_AUTOSTEP);
		cvTranspose(H,H1);
		Mat H2=H1;
		t=(double)(cvGetTickCount()-t)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����	
		R11=cvmGet(H1,0,0);
		R12=cvmGet(H1,0,1);
		R21=cvmGet(H1,1,0);
		R22=cvmGet(H1,1,1);
		R13=cvmGet(H1,0,2);
		R23=cvmGet(H1,1,2);
		R33=cvmGet(H1,2,2);
		px=cvmGet(H1,2,0);
		py=cvmGet(H1,2,1);
		sfx=cvSqrt(R11*R11+R12*R12);
		sfy=cvSqrt(R21*R21+R22*R22);
		rotate = (180*atan(R12/R11))/3.1415;
		CvPoint p1,p2,p3,p4; 

		p2.x=px;  p2.y=py;

		p1.x=(snimg[0]->width)*R11+px;  p1.y=R12*(snimg[0]->width)+py; 
		p3.x=R21*(snimg[0]->height)+px; p3.y=R22*(snimg[0]->height)+py; 
		p4.x=(snimg[0]->width)*R11+R21*(snimg[0]->height)+px;
		p4.y=R12*(snimg[0]->width)+R22*(snimg[0]->height)+py; 

		cvLine(rfimg[0],p1,p2,CV_RGB(255,0,0),2,CV_AA);
		cvLine(rfimg[0],p2,p3,CV_RGB(255,0,0),2,CV_AA);
		cvLine(rfimg[0],p3,p4,CV_RGB(255,0,0),2,CV_AA);
		cvLine(rfimg[0],p4,p1,CV_RGB(255,0,0),2,CV_AA);
		
		cvSaveImage( resultImg[2].c_str(), rfimg[0] );
		// 
		QString surftime = QString::number(t,'g',6);
		ui.registratetime_3->setText(surftime);
		QString surfptpairs = QString::number(num1,'g',6);
		ui.ptpairsnum_3->setText(surfptpairs);
		QString surfmes = QString::number(mese1,'g',6);
		ui.mse_3->setText(surfmes);

		QString bigx = QString::number(px,'g',6);
		ui.shift_x_3->setText(bigx);
		QString bigy = QString::number(py,'g',6);
		ui.shift_y_3->setText(bigy);

		QString surfrotate = QString::number(rotate,'g',6);
		ui.rotate_3->setText(surfrotate);

		QString iscale1 = QString::number(sfx,'g',6);
		QString iscale2 = QString::number(sfy,'g',6);
		QString iscale = iscale1+"/"+iscale2;
		ui.scale_3->setText(iscale);
	
		flag11=0;
		flag22=0;
		flag1=0;
		flag2=0;
		cvReleaseImage( &temp1 );
		cvReleaseImage( &img1 );
		cvReleaseImage( &img2 );
		cvReleaseImage( &temp );
	}
	else
	{
	//	MessageBox("�����㷨ƥ��ʧ�ܣ�");
		//t1=0;
		mese1=0;
		num1=0;
		flag11=0;
		flag22=0;
		flag1=0;
		flag2=0;
		return;
	}

	
	OpenResultFile2(resultImg[2]);
	
	//cvReleaseMemStorage(&storage2);
	//ptpairs2.clear();
	xishu.clear();
	lujing.clear();

}



void CariRecursive(QString strInitPath, QString strFileName)  //����
{
	//�ж�·���Ƿ����
	QDir dir(strInitPath);
	if(!dir.exists())
	{
		return;
	}
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	QFileInfoList list = dir.entryInfoList();

	int file_count = list.count();
	if(file_count <= 0)
	{
		return;
	}

	QStringList string_list;
	for(int i=0; i<file_count;i++)
	{
		QFileInfo file_info = list.at(i);
		QString suffix = file_info.suffix();
		if(QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0)   
		{   
			QString absolute_file_path = file_info.absoluteFilePath();
			string_list.append(absolute_file_path);
			Madhosi(absolute_file_path);
		}
	}


}


double Madhosi(QString strNameFile) //��ȡͼ������ֵ,���Ƚ�   ����
{
	QString strTemp;
	CxImage imageAsli1;
	/*const size_t strsize=(strNameFile.GetLength()+1)*2; // ���ַ��ĳ���;
    char* strNameFile1= new char[strsize]; //����ռ�;
    size_t sz=0;
    wcstombs_s(&sz,strNameFile1,strsize,strNameFile,_TRUNCATE);
    int atoiato=atoi((const char*)strNameFile1); // �ַ����Ѿ���ԭ����QString ת������ const char*/
	QString dir_s=strNameFile;
	const QByteArray text = dir_s.toLocal8Bit();
	const char *strNameFile1 = text.data();

	//imageAsli1.Load(strNameFile1,CXIMAGE_FORMAT_TIF);
	imageAsli1.Load(strNameFile1);

	double dTemp,dTempColor,dTempCuve,dTempShape;
	int i;
	double powCuve[15]={0.1,0.1,0.1,0.1,0.1,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05};
	double powShape[8]={0.2,0.2,0.15,0.15,0.1,0.05,0.05,01};
	
	ColorSplit(imageAsli1);
	CuveSplit(imageAsli1);
	ShapeSplit(imageAsli1);
	//
	//
	dTempColor = 0.0;
	dTempCuve=0.0;
	dTempShape=0.0;
	double dQueryNum = 0.0;
	for(i = 0; i < 111; i++ )
	{
		dTempColor += max1(m_KeyMainColorSig[i],m_Color[i]);
		dQueryNum += m_KeyMainColorSig[i];
	}

	//
	dTempColor = dQueryNum/dTempColor;

	for(i=0;i<15;i++)
	{
		if(m_KeyCuve[i]!=0||m_Cuve[i]!=0)
			dTempCuve = dTempCuve + powCuve[i]*fabs((m_KeyCuve[i]-m_Cuve[i])/max1(m_KeyCuve[i],m_Cuve[i]));
	}
	dTempCuve = 1 -dTempCuve;

	for(i=0;i<8;i++)
	{
		if(m_KeyShape[i]!=0&&m_Shape[i]!=0)
			dTempShape = dTempShape + powShape[i]*fabs((m_KeyShape[i]-m_Shape[i])/(m_KeyShape[i] > m_Shape[i]?m_KeyShape[i]:m_Shape[i]));

	}
	dTempShape = 1-dTempShape;

	dTemp = dTempColor*ColorPow + dTempCuve*CuvePow + dTempShape*ShapePow;
	dTemp = 1-dTemp;

	xishu.push_back(dTemp);/////������
	lujing.push_back(strNameFile);///��Ӧ��·��
	return dTemp;

}


//��ͼ����н������ĺ����� scrimg��ԭʼͼ��dstimg����������ͼ��·����scale������������ = ��������ߴ�/������ǰ�ߴ� 
void ResizeImage( const char* scrimg,const char* dstimg, double scale )
{
	IplImage *scr = 0;
	IplImage *dst = 0;
	CvSize dst_cvsize;

	if (scr = cvLoadImage(scrimg,-1))
	{
		dst_cvsize.width=(int)(scr->width*scale);
		dst_cvsize.height=(int)(scr->height*scale);
		dst=cvCreateImage(dst_cvsize,scr->depth,scr->nChannels);
		cvResize(scr,dst,CV_INTER_AREA);//
		//             CV_INTER_NN - ����ڲ�ֵ,
		//             CV_INTER_LINEAR - ˫���Բ�ֵ (ȱʡʹ��)
		//             CV_INTER_AREA - ʹ�����ع�ϵ�ز�������ͼ����Сʱ�򣬸÷������Ա��Ⲩ�Ƴ��֡�
		/*��ͼ��Ŵ�ʱ�������� CV_INTER_NN ����..*/
		//             CV_INTER_CUBIC - ������ֵ.

		cvSaveImage(dstimg,dst);
	}
	//cvRelease( & scr);
	cvReleaseImage( &scr );
	cvReleaseImage( &dst );
}




#include "sy_2.h"
#include <comutil.h>
#include <direct.h>  
#include <io.h>  
#include <Windows.h>  

//******������׼ʱ��ͼ��·��*****///
vector<IplImage*>img;

//*** �����ʼ�����ͼ��·�� ***//
vector<string> refImg;
vector<string> senImg;
vector<string> resultImg;

/*** ����������ļ�·�� ***/
vector<string> refLayer;
vector<string> senLayer;


//*** SURF�㷨����ص�ȫ�ֱ��� ***//
vector<CvPoint2D64f> rePoint;
vector<CvPoint2D64f> senPoint;
int num1;
int num2;
double mese2;//��ͼ��׼����
vector<CvMat*> H_1;///��ʱû���õ�
vector<CvMat*> H_2;///���Դ洢H����
CvSeq* objectKeypoints = 0, * objeceDescriptors = 0;  ///����ͼ��������㼰������Ϣ
CvSeq* imageKeypoints = 0, * imageDescriptors = 0;

vector<int> ptpairs_befores; //�洢��ѡ֮ǰ��ƥ����������
CvMat *OverLap1;
CvMat *OverLap2;



using namespace std;
using namespace cv;

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

	//���������Ϳؼ������
	//Map�ؼ�
	m_pToolbarControl1->SetBuddyControl(m_pMapControl1);
	m_pToolbarControl2->SetBuddyControl(m_pMapControl2);
	//	m_pToolbarControl3->SetBuddyControl(m_pMapControl3);
	m_pToolbarControl4->SetBuddyControl(m_pMapControl4);

	m_pTOCControl1->SetBuddyControl(m_pMapControl1);
	m_pTOCControl2->SetBuddyControl(m_pMapControl2);

	//�������ӹ���������

	AddMapToolBarCtr(m_pToolbarControl1);//map
	AddMapToolBarCtr(m_pToolbarControl2);//map
	//	AddMapToolBarCtr(m_pToolbarControl3);//map
	AddMapToolBarCtr(m_pToolbarControl4);//map

	//�Բ˵����д�ͼ��Ĳ�����������
	//��һ�������ö���
	openAction1 = new QAction( QIcon(":/images/doc-open"), tr("&�򿪲ο�ͼ��"), this );
	openAction1->setStatusTip(tr("Open the First Image"));//�������������ʱ�����ڴ������·�״̬����ʾ״̬
	openAction2 = new QAction( QIcon(":/images/doc-open"), tr("&�򿪴���׼ͼ��"), this );
	openAction2->setStatusTip(tr("Open the Second Image"));//ͬ��
	SurfAction = new QAction( QIcon("tree"), tr("&SURF"), this );
	SurfAction->setStatusTip(tr("SURF Registration"));//ͬ��
	DeleMapLayersAction = new QAction( QIcon(":/image/doc-close"),tr("&�����������"),this);
	DeleMapLayersAction->setStatusTip(tr("Clear all Datas"));

	//�ڶ������������뺯����������γ��ź�-�ۻ���
	connect(openAction1, SIGNAL(triggered()), this, SLOT(OpenFile1()));
	connect(openAction2, SIGNAL(triggered()), this, SLOT(OpenFile2()));
	connect(SurfAction, SIGNAL(triggered()), this, SLOT(BigMapRegistration()));
	connect(DeleMapLayersAction,SIGNAL(triggered()), this, SLOT(OnClearMapLayer()));

	//���������ڲ˵�����Ӧ�ط�����Ӷ���
	ui.menu_3->addAction(openAction1);
	ui.menu_3->addAction(openAction2);
	ui.menu_2->addAction(SurfAction);
	ui.menu->addAction(DeleMapLayersAction);

}

sy_2::~sy_2()
{
}



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



void sy_2::OpenFile1()
{
	PyramidBase pyramid1; 
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
	pyramid1.InFilePath = reImg_full.c_str();	//�ο�ͼ������·��
	pyramid1.FilePath = reImg_path.c_str();		//�ο�ͼ�������ļ���·��
	pyramid1.CheckPyramid();
	refLayer.clear();

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

		return;

	ILayerPtr pLyr(pRasterLy);

	m_pMapControl1->AddLayer(pLyr,0);

	IActiveViewPtr pActiveView(m_pMapControl1);

	m_pMapControl1->Refresh(esriViewAll);  

}

void sy_2::OpenFile2()
{
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
	pyramid2.CheckPyramid();

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

		return;

	ILayerPtr pLyr(pRasterLy);

	m_pMapControl2->AddLayer(pLyr,0);

	IActiveViewPtr pActiveView(m_pMapControl2);

	m_pMapControl2->Refresh(esriViewAll);  
}


//void sy_2::OpenResultFile1(string DstImagePath)
//{
//	PyramidBase pyramid1; 
//	pyramid1.InFilePath = DstImagePath.c_str(); //��׼���ͼ������·��
//	pyramid1.FilePath = resultImg[0].c_str();
//	pyramid1.CheckPyramid();
//
//	BSTR bstr_str = _bstr_t(DstImagePath.c_str());
//	
//	HRESULT hr;
//
//	IRasterLayerPtr pRasterLy(CLSID_RasterLayer);
//
//	hr=pRasterLy->CreateFromFilePath(bstr_str);
//
//	if(FAILED(hr))
//
//		return;
//
//	ILayerPtr pLyr(pRasterLy);
//
//	m_pMapControl4->AddLayer(pLyr,0);
//
//	IActiveViewPtr pActiveView(m_pMapControl4);
//
//	m_pMapControl4->Refresh(esriViewAll);  
//}

//��ͼ��׼
void sy_2::BigMapRegistration()
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
		
		refImage = refLayer[i];				//ȡ�������ڶ���ͼ����Ϊ����������ͼ���СΪ512*512.
		senImage = senLayer[i];		
		//
//		double t4=(double)cvGetTickCount();//��ʼ��ʱ
		//t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		SURF_ON_TWO(refImage,senImage,sImageResultPath);//��ȡ��������������С��1000��ͼ�����ƥ��
		//double t4=(double)cvGetTickCount();//��ʼ��ʱ
//		t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
		//��i������1����˵��Ҫ�Խ�������ĳһ��ͼ�������׼�������Ҫ���вü���ƥ��Ȳ���

		Find_OverlapArea(i);//�Խ��ͼ����ƥ����������ͳ�ƣ���������Ȥ������в��в�����

		//t3=(double)(cvGetTickCount()-t3)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����

		//	double t4=(double)cvGetTickCount();//��ʼ��ʱ
		//���ŶԸ���Ȥ�����ͼ�����surfƥ��
		//int pathcount = simage_repath.size();
		string OverlapImage1 = refImg[2];
		string OverlapImage2 = senImg[2];
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

		int h1 = abs(bighx) + 0.5;
		int h2 = abs(bighy) + 0.5;
		//�Դ�ͼ��ı任�����е�ƽ�������и���
		H1 = H;
		cvmSet(H1,0,2,bighx);
		cvmSet(H1,1,2,bighy);
		t=(double)(cvGetTickCount()-t)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
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
	ui.registratetime->setText(s1);
	QString mse2 = QString::number(mese2,'g',6);
	ui.mse->setText(mse2);
	QString s2 = QString::number(num2,'g',6);
	ui.ptpairsnum->setText(s2);
	//QString size1 = QString::number(t3,'g',6);
	//ui.refImageSize->setText(size1);
	//QString size2 = QString::number(t3,'g',6);
	//ui.senImageSize->setText(size2);

	OpenResultFile1(resultImg[2]);
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
			double px, py;
			int h1, h2;
			px = cvmGet( H, 0, 2 );
			py = cvmGet( H, 1, 2 );
			h1 = abs(px) + 0.5;
			h2 = abs(py) + 0.5;
			IplImage* xformed;
			xformed = cvCreateImage( cvSize( h1 + img[1]->width, h2 + img[1]->height ), IPL_DEPTH_8U, 3 );
			cvWarpPerspective( img[1], xformed, H, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
			//͸��ͶӰ
			cvSetImageROI(xformed, cvRect( 0, 0, img[0]->width, img[0]->height ) );
			cvAddWeighted( img[0], 1, xformed, 0, 0, xformed );
			//Ȩ�غ�������������ֱ��ǣ���һ�����飬Ȩֵ���ڶ������飬Ȩֵ�����ʱ����ӳ�����������
			cvResetImageROI( xformed );

			cvSaveImage( DstImagePath.c_str(), xformed );

			//cvShowImage(xformed)
			cvReleaseImage(&xformed);

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

	//�����ü�ͼ���·��
	string Overlap1 = refImg[0] + "refImage_Overlap.jpg";
	string Overlap2 = senImg[0] + "senImage_Overlap.jpg";
	refImg.push_back(Overlap1);//refImg[2]�б���ο�ͼ���ص�����ü����ͼ��·��
	senImg.push_back(Overlap2);//senImg[2]�б������׼ͼ�����ص�����ü���ͼ��·��

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
	int sdx = dx/3;
	int sdy = dy/3;
	Mat overlap = Mat::zeros((int)image2_ysize,(int)image2_xsize,CV_8UC1);
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
	//t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����

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
	HRESULT DeleteLayer1,DeleteLayer2,DeleteLayer3,DeleteLayer4;
	DeleteLayer1 = m_pMapControl1->DeleteLayer(0);////����������������������ݡ�
	DeleteLayer2 = m_pMapControl2->ClearLayers();
	//	DeleteLayer3 = m_pMapControl3->ClearLayers();
	DeleteLayer4 = m_pMapControl4->DeleteLayer(0);////����������������������ݡ�
	if (img.size())
	{
		for ( int i = 0; i < img.size(); i ++ )
		{
			cvReleaseImage(&img[i]);
		}
		img.clear();
	}
	//ui.bigmapmse->clear();
	//ui.bigmaptime->clear();
//	ui.bigmaptime2->clear();
	refImg.clear();
	senImg.clear();///�ֱ𱣴����ڳ���ƥ���Сͼ��·��
	resultImg.clear();
	rePoint.clear();
	senPoint.clear();
}

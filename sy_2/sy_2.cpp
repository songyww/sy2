#include "sy_2.h"
#include <comutil.h>
#include <direct.h>  
#include <io.h>  
#include <Windows.h>  

//******保存配准时的图像路径*****///
vector<IplImage*>img;

//*** 保存初始读入的图像路径 ***//
vector<string> refImg;
vector<string> senImg;
vector<string> resultImg;

/*** 保存金字塔文件路径 ***/
vector<string> refLayer;
vector<string> senLayer;


//*** SURF算法中相关的全局变量 ***//
vector<CvPoint2D64f> rePoint;
vector<CvPoint2D64f> senPoint;
int num1;
int num2;
double mese2;//大图配准精度
vector<CvMat*> H_1;///暂时没有用到
vector<CvMat*> H_2;///用以存储H矩阵
CvSeq* objectKeypoints = 0, * objeceDescriptors = 0;  ///保存图像的特征点及描述信息
CvSeq* imageKeypoints = 0, * imageDescriptors = 0;

vector<int> ptpairs_befores; //存储挑选之前的匹配的特征点对
CvMat *OverLap1;
CvMat *OverLap2;



using namespace std;
using namespace cv;

sy_2::sy_2(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	//Map控件设置
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




	//将ui界面的控件和定义的指针关联
	//Map控件
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

	//将工具条和控件相关联
	//Map控件
	m_pToolbarControl1->SetBuddyControl(m_pMapControl1);
	m_pToolbarControl2->SetBuddyControl(m_pMapControl2);
	//	m_pToolbarControl3->SetBuddyControl(m_pMapControl3);
	m_pToolbarControl4->SetBuddyControl(m_pMapControl4);

	m_pTOCControl1->SetBuddyControl(m_pMapControl1);
	m_pTOCControl2->SetBuddyControl(m_pMapControl2);

	//调用增加工具条函数

	AddMapToolBarCtr(m_pToolbarControl1);//map
	AddMapToolBarCtr(m_pToolbarControl2);//map
	//	AddMapToolBarCtr(m_pToolbarControl3);//map
	AddMapToolBarCtr(m_pToolbarControl4);//map

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
	String reImg_full,reImg_path;	//reimg_full:图像的完整路径；reimg_path：图像所在文件夹路径

	QFileInfo fi;		  
	file_full1 = QFileDialog::getOpenFileName(this,"open the second picture",file_path1);  
	fi = QFileInfo(file_full1);  
	m_strFileName1=fi.absoluteFilePath();

	reImg_full = m_strFileName1.toStdString();////QString 转换为string///图像完整路径
	int po = reImg_full.find_last_of('/');
	reImg_path = reImg_full.substr( 0,po+1); ///文件夹路径
	
	//将参考图像所在文件夹路径以及图像路径存放入路径容器
	refImg.push_back(reImg_path);//refImg[0]:参考图像所在文件夹路径
	refImg.push_back(reImg_full);//refImg[1]:参考图像完整路径
	
	//将参考图像保存入金字塔构建的路径中
	pyramid1.InFilePath = reImg_full.c_str();	//参考图像完整路径
	pyramid1.FilePath = reImg_path.c_str();		//参考图像所在文件夹路径
	pyramid1.CheckPyramid();
	refLayer.clear();

	char dir[1024];
	sprintf(dir,"%s%s",pyramid1.FilePath,"\\refPyramidLayers");//filepath:金字塔层所在文件夹
	//判断该文件夹是否存在，若存在，则不用重新建立；若不存在，则创建该文件夹
	if(_access(dir, 0) != -1)  
	{  
		cout << "文件夹已存在" <<endl;  
		//system("pause");
	}  
	else  
	{  
		cout << "文件夹不存在，创建文件夹" << endl;  
		_mkdir(dir); 
		//system("pause");
	} 
	pyramid1.LayerPath = dir;
	refLayer.push_back(dir);		//refLayer[0]:参考图像金字塔文件夹路径

	for (int i = 0;i < pyramid1.iOverViewCount; i++)
	{
			
		char filepath[1024] = "";			//filepath[1024]:用以保存函数返回的金字塔每一层图像路径
		pyramid1.SaveSingleBand(i,filepath);
		refLayer.push_back(filepath);	//refLayer[i+1]:参考图像金字塔第i层图像路径；其中refLayer[1]保存第0层(原图像素的1/4 * 1/4)
	}

	/*Map控件显示参考图像*/
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
	String sen_full,sen_path;	//sen_full:待配准图像完整路径；sen_path：待配准图像所在文件夹路径

	QFileInfo fi;		  
	file_full2 = QFileDialog::getOpenFileName(this,"open the second picture",file_path2);  
	fi = QFileInfo(file_full2);  
	m_strFileName2=fi.absoluteFilePath();

	sen_full = m_strFileName2.toStdString();
	int po = sen_full.find_last_of('/');
	sen_path = sen_full.substr( 0, po + 1 );

	/*将待配准图像路径存如全局路径容器中*/
	senImg.push_back(sen_path);		//senImg[0]:待配准图像所在文件夹路径
	senImg.push_back(sen_full);		//senImg[1]：待配准图像完整路径
	
	/*将待配准图像路径传递给金字塔类*/
	pyramid2.InFilePath = sen_full.c_str();	//待配准图像完整路径
	pyramid2.FilePath = sen_path.c_str();	//待配准图像文件夹路径
	pyramid2.CheckPyramid();

	char dir[1024];
	sprintf(dir,"%s%s",pyramid2.FilePath,"\\senPyramidLayers");
	pyramid2.LayerPath = dir;			//待配准图像金字塔图像所在文件夹路径
	senLayer.push_back(dir);			//senLayer[0]:待配准图像金字塔层图像所在文件夹
	for (int i = 0;i < pyramid2.iOverViewCount; i++)
	{
		char filepath[1024] = "";
		pyramid2.SaveSingleBand(i,filepath);
		senLayer.push_back(filepath);//senLayer[i+1]:金字塔第i层图像，其中[1]保存第0层(原图像素的1/4 * 1/4)，[3]保存第1层(原图像素的1/8 * 1/8)
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


void sy_2::OpenResultFile1(string DstImagePath)
{
	PyramidBase pyramid1; 
	pyramid1.InFilePath = DstImagePath.c_str(); //配准结果图像完整路径
	pyramid1.FilePath = resultImg[0].c_str();
	pyramid1.CheckPyramid();

	BSTR bstr_str = _bstr_t(DstImagePath.c_str());
	
	HRESULT hr;

	IRasterLayerPtr pRasterLy(CLSID_RasterLayer);

	hr=pRasterLy->CreateFromFilePath(bstr_str);

	if(FAILED(hr))

		return;

	ILayerPtr pLyr(pRasterLy);

	m_pMapControl4->AddLayer(pLyr,0);

	IActiveViewPtr pActiveView(m_pMapControl4);

	m_pMapControl4->Refresh(esriViewAll);  
}

//大图配准
void sy_2::BigMapRegistration()
{
	//*创建结果图像所在文件夹*//
	string dir = senImg[0] + "BigMapResultFiles"; ///在参考图像文件夹下创建文件夹，保存小图和大图的配准结果
	if(_access(dir.c_str(), 0) != -1)  
	{  
		cout << "文件夹已存在" <<endl;  
		//system("pause");
	}  
	else  
	{  
		cout << "文件夹不存在，创建文件夹" << endl;  
		_mkdir(dir.c_str()); 
		//system("pause");
	} 
	string dir1 = dir + "\\";

	string sImageResultPath = dir1 + "PyramidImageRegResult.jpg";	//裁剪图像匹配结果图
	string BigMapResult = dir1 + "BigMapRegistrationResult.tif";	//大图匹配结果图
	resultImg.clear(); 
	resultImg.push_back(dir1);				//resultImg[0]:结果图像所在文件夹
	resultImg.push_back(sImageResultPath);	//resultImg[1]：裁剪图像配准结果路径
	resultImg.push_back(BigMapResult);		//resultImg[2]：大图配准结果
	
	int i = refLayer.size()-2;
	string refImage,senImage;	//这三个量分别用于临时存储用以匹配的图像路径，并传递给图像配准的函数

	//如果前面对金字塔层数进行了选择，或者改变，这个函数就需要能够适应这种改变
	//首先应该对i进行判断，当i=-1时，表明没有金字塔层无需进行裁剪，直接进行匹配，
	CvMat* H;//现在先不讨论图像大小时的问题，先把大图配准给改进
	if (i <= 0)
	{
		refImage = refImg[1];
		senImage = senImg[1];
		//下面进行的是对小图的匹配
		SURF_ON_TWO(refImage,senImage,sImageResultPath);
		H = H_2[0];
	}
	else
	{
		
		refImage = senLayer[i];				//取出倒数第二层图像。因为金字塔顶层图像大小为512*512.
		senImage = refLayer[i];				//
		SURF_ON_TWO(refImage,senImage,sImageResultPath);//调取金字塔中像素数小于1000的图像进行匹配

		//若i不等于1，则说明要对金字塔的某一层图像进行配准，这就需要进行裁剪重匹配等操作

		Cut_Count_Overlap(i);//对结果图像中匹配特征进行统计，并将感兴趣区域进行裁切并保存

		//t3=(double)(cvGetTickCount()-t3)/(cvGetTickFrequency()*1000*1000.); ///计时结束

		//	double t4=(double)cvGetTickCount();//开始计时
		//接着对感兴趣区域的图像进行surf匹配
		int pathcount = simage_repath.size();
		string OverlapImage1 = simage_repath[pathcount-1];
		string OverlapImage2 = simage_senpath[pathcount-1];
		string OverlapResult = simage_repath[1] + "OverLapResultImage.jpg";
		SURF_ON_TWO(OverlapImage1,OverlapImage2,OverlapResult); //该函数进行完毕后，在H_2[0]中保存的是当前图像间变换后的H矩阵
		//	t4=(double)(cvGetTickCount()-t4)/(cvGetTickFrequency()*1000*1000.); ///计时结束
		//Cut_Count_Overlap();//对结果图像中匹配特征进行统计，并将感兴趣区域进行裁切并保存
		//下面从变换后的H矩阵中提取变换参数，用以对大图进行变换
		H = H_2[0];

		double smallhx,smallhy;///该参数表示两个裁剪出来的小图的平移参数
		double bighx,bighy;//该参数表示待配准图像相对于参考图像的平移参数
		//px = cvmGet( H, 0, 2 );
		//py = cvmGet( H, 1, 2 );
		smallhx = cvmGet( H, 0, 2 ) + rePoint[0].x; //这里算出的是待配准图像中感兴趣区域相对于参考图像的平移距离
		smallhy = cvmGet( H, 1, 2 ) + rePoint[0].y;
		//现在还需要知道 参考图像中的小图相对于大图的位置或者坐标
		//即参考图像中裁剪出的小图左上角的坐标
		CvMat *H1; //定义大图之间的变换矩阵，尤其是平移量进行更新

		//下面两句根据裁剪出的待配准图像中的小图在参考图像中的位置，计算出待配准图像的原图的左上角坐标在参考图像坐标系中的位置（或者说是待配准图像左上角顶点的平移量）
		bighx = smallhx - senPoint[0].x;
		bighy = smallhy - senPoint[0].y;

		int h1 = abs(bighx) + 0.5;
		int h2 = abs(bighy) + 0.5;
		//对大图间的变换矩阵中的平移量进行更新
		H1 = H;
		cvmSet(H1,0,2,bighx);
		cvmSet(H1,1,2,bighy);
		t2=(double)(cvGetTickCount()-t2)/(cvGetTickFrequency()*1000*1000.); ///计时结束
		//得到变换参数之后，下面就要建立一个新的更大的图，用以存储两个大图配准的结果

		//ResultImagePath.push_back(BigMapResult);
		GDALAllRegister();
		//将两个大图读入数据集中
		GDALDataset* pSrcDS1 = ( GDALDataset* )GDALOpen(simage_repath[0].c_str(),GA_ReadOnly); 
		GDALDataset* pSrcDS2 = ( GDALDataset* )GDALOpen(simage_senpath[0].c_str(),GA_ReadOnly);

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

			//创建配准结果图像
			GDALDataset *pDstDS = poDriver->Create(BigMapResult.c_str(), iDstWidth, iDstHeight, iBandCount1, eDT1, NULL);
			pDstDS->SetGeoTransform(adfGeoTransform);
			pDstDS->SetProjection(pSrcDS1->GetProjectionRef());

			int *pBandMap = new int[iBandCount1];
			for (int i=0; i<iBandCount1; i++)
				pBandMap[i] = i+1;

			// 下面这里需要根据数据类型来判断，申请不同类型的缓存来进行处理，这里仅仅以8bit图像进行演示
			//下面操作是将两个图像都变换到配准结果图像中，只不过用的是分块的思想
			if (eDT1 == GDT_Byte)	// 如果是8bit图像
			{
				char *pDataBuff = new char[xsize1/2*ysize1/2*iBandCount1];
				//将参考图像变换到结果图像中
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

				t3=(double)(cvGetTickCount()-t3)/(cvGetTickFrequency()*1000*1000.); ///计时结束

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

	QString s1 = QString::number(t2,'g',6);
	ui.bigmaptime->setText(s1);
	QString mse = QString::number(mese2,'g',6);
	ui.bigmapmse->setText(mse);
	QString s2 = QString::number(t3,'g',6);
	ui.bigmaptime2->setText(s2);



	OpenResultFile1(ResultImagePath[2]);
}



void SURF_ON_TWO(string SrcReImagePath, string SrcSenImagePath, string DstImagePath )
{
	cv::initModule_nonfree();
	img.clear();
	img.push_back(cvLoadImage(SrcReImagePath.c_str())); //这里对创建的金字塔层中第一层(金字塔是从第0层开始创建的)进行的配准；；之所以不采用对第一层进行初配准，原因是这两种操作达到的目的都是获取重叠区域感兴趣的部分，但是时间却不一样，而且，在遇到很大的图像的时候，即使是第0层图像也无法用opencv来处理。综合来讲，使用第1层是最安全且有效的。
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
	CvMemStorage* storage = cvCreateMemStorage(0);//内存存储器
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
	};//建立类似于调色版的东西
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
		H = GetH3( ptpairs, imageKeypoints, objectKeypoints);////在该函数调用过程中进行了RANSAC算法

		if ( H )
		{
			mese2 = GetRMSE3( ptpairs, imageKeypoints, objectKeypoints, H );
			ptpairs_befores = ptpairs;////把经过RANSAC算法之后的内点序列保存为用于挑选的特征点
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
			//透视投影
			cvSetImageROI(xformed, cvRect( 0, 0, img[0]->width, img[0]->height ) );
			cvAddWeighted( img[0], 1, xformed, 0, 0, xformed );
			//权重函数，参数含义分别是：第一组数组，权值，第二组数组，权值，输出时候添加常数项，输出数组
			cvResetImageROI( xformed );

			cvSaveImage( DstImagePath.c_str(), xformed );

			//cvShowImage(xformed)
			cvReleaseImage(&xformed);

		}

	}

}

void sy_1::Openbigresult()
{
	OpenResultFile1(ResultImagePath[2]);
}
void sy_1::OpenResultFile1(string DstImagePath)
{
	PyramidBase pyramid1; 
	pyramid1.InFilePath = DstImagePath.c_str(); 
	pyramid1.Path = ResultImagePath[0].c_str();
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

	//	QMessageBox::information(this, tr("Information"),tr("宋蕴"));
}


///设计函数获取图像的重叠区域，并对其进行分区域统计，最后获取统计后最好的区域（特征点最多的区域）

void Cut_Count_Overlap(int ilayer)		//传入的参数ilayer表示是对simage_repath[ilayer]路径中的图像进行处理
{

	//通过传递的参数ilayer计算从缩略图到源图的放大倍数

	int iscale = static_cast<int>(pow(2.0,ilayer)); 


	rePoint.clear(); //保存参考图像中裁剪出来的小图的左上角坐标
	senPoint.clear();//保存待配准图像中裁剪出来的小图的左上角坐标

	//给出裁切出的图像的路径
	string Overlap1 = simage_repath[1] + "reImage_Overlap.jpg";
	string Overlap2 = simage_senpath[1] + "senImage_Overlap.jpg";
	simage_repath.push_back(Overlap1);//
	simage_senpath.push_back(Overlap2);//

	vector<int> ptpairs = ptpairs_befores;
	int num  = ptpairs.size()/2;

	int image1_xsize,image1_ysize; ///定义参考图像大小信息
	int image2_xsize,image2_ysize;//定义待配准图像大小信息

	int areaCount1[10] = {0,0,0,0,0,0,0,0,0,0};///每一个数字代表一块区域
	int areaCount2[10] = {0,0,0,0,0,0,0,0,0,0};///
	CvMat* H;///用于临时保存H矩阵
	H = H_2[0]; //将第一次配准结果中的H矩阵传递过来。//此处也可以后续设计为一个函数传递的参数
	double px, py; //待配准图像相对于参考图像平移的距离，也可作为重叠区域左上角或者左下角的坐标
	double h1, h2, h3, h4;
	double dx,dy;///定义重叠区域的大小
	double hx,hy;///定义分块统计时，x和y方向每一小块的大小；
	px = cvmGet( H, 0, 2 );
	py = cvmGet( H, 1, 2 );
	h1 = abs(px) + 0.5;////为了四舍五入所以加上0.5，这一点还没有想明白
	h2 = abs(py) + 0.5;///故重叠区域左上角或者左下角的坐标即为：(h1,h2)

	///这里需要获取图像的大小参数
	//先获取参考图像这边的信息
	image1_xsize = img[0]->width;
	image1_ysize = img[0]->height;

	//再获取待配准图像的信息
	image2_xsize = img[1]->width;
	image2_ysize = img[1]->height;


	//计算重叠区域的大小
	dx = image1_xsize - h1;
	dy = image1_ysize - h2;

	//下面计算hx,hy
	double fenkuai3 = 3;
	hx = dx/fenkuai3;
	hy = dy/fenkuai3;

	//IplImage* image0 = cvLoadImage(simage_repath[2].c_str());
	//Mat image0 = cvLoadImage(simage_repath[4].c_str());
	//Mat image1 = cvLoadImage(simage_senpath[4].c_str());

	////画出重叠区域的矩形框
	//cvRectangle(img[0],cvPoint(h1,h2),cvPoint(image1_xsize,image1_ysize),cvScalar(0,0,255),3,4,0 );
	//cvNamedWindow("OverLapArea",CV_WINDOW_AUTOSIZE);
	//cvShowImage("img[0]WithOverLapArea",img[0]);
	//cvWaitKey(0);
	//cvReleaseImage(&img[0]);

	CvSeq *CurrentKeypoints = objectKeypoints;
	//CvSeq *PreKeypoints = imageKeypoints;
	//CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem(CurrentKeypoints, ptpairs[i] );
	//下面对参考图像中已经匹配的特征点进行统计
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

	//统计特征点个数的段落 

	for (int i = 0; i < ptpairs.size(); i++)
	{
		CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem(CurrentKeypoints, ptpairs[i] );


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
		i++;//因为ptpairs中存储的是一对一对的特征点的索引号，这里值需要提取出其中参考图像中特征点的序号，故应该隔一个一取
	}


	int maxvaule1 = areaCount1[0]; //表示数组中的最大值
	int maxnum1 = 0;	//标记数组中最大值的序号，也即对应区域
	//循环结束，统计数量最大的区域,并画出来
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
	//得到最多特征点的区域的序号，就对应地画出来
	//rectangle(img[0],cvPoint(h1,h2),cvPoint(h1+hx,h2+hy),cvScalar(0,0,255),3,4,0 );

	switch (maxnum1)
	{
	case 0:
		//参考图像的裁剪
		CvPoint2D64f point0;
		point0.x= iscale*h1;///不行，应该把这些数都改为浮点数
		point0.y = iscale*h2;
		rePoint.push_back(point0);

		//	rectangle(img[0],cvPoint(h1,h2),cvPoint(h1+hx,h2+hy),cvScalar(0,255,255),2,4,0 );
		//	imshow(atom_window0,img[0]);
		ImageCut(simage_repath[0].c_str(),Overlap1.c_str(),point0.x,point0.y,iscale*hx,iscale*hy,"GTiff");


		//待配准图像的裁剪
		CvPoint2D64f point00;
		point0.x = 0;///不行，应该把这些数都改为浮点数
		point0.y = 0;
		senPoint.push_back(point00);

		//	rectangle(img[1],cvPoint(0,0),cvPoint(hx,hy),cvScalar(255,255,0),2,4,0 );
		//	imshow(atom1_window0,img[1]);
		ImageCut(simage_senpath[0].c_str(),Overlap2.c_str(),0,0,iscale*hx,iscale*hy,"GTiff");

		//waitKey(0);
		break;
	case 1:
		CvPoint2D64f point1;
		point1.x = iscale*(h1+hx);///不行，应该把这些数都改为浮点数
		point1.y = iscale*h2;
		rePoint.push_back(point1);

		//	rectangle(img[0],cvPoint(h1+hx,h2),cvPoint(image1_xsize-hx,h2+hy),cvScalar(0,255,255),2,4,0 );
		//	imshow(atom_window1,img[0]);
		ImageCut(simage_repath[0].c_str(),Overlap1.c_str(),point1.x,point1.y,iscale*(image1_xsize-2*hx),iscale*hy,"GTiff");


		CvPoint2D64f point11;
		point11.x = iscale*hx;///不行，应该把这些数都改为浮点数
		point11.y = 0;
		senPoint.push_back(point11);

		//	rectangle(img[1],cvPoint(hx,0),cvPoint(dx-hx,hy),cvScalar(255,255,0),2,4,0 );
		//	imshow(atom1_window1,img[1]);
		ImageCut(simage_senpath[0].c_str(),Overlap2.c_str(),point11.x,point11.y,iscale*(dx-hx*2),iscale*hy,"GTiff");



		//waitKey(0);
		break;
	case 2:
		CvPoint2D64f point2;
		point2.x =iscale*(image1_xsize-hx);///不行，应该把这些数都改为浮点数
		point2.y = iscale*h2;
		rePoint.push_back(point2);

		//	rectangle(img[0],cvPoint(image1_xsize-hx,h2),cvPoint(image1_xsize,h2+hy),cvScalar(0,255,255),2,4,0 );
		//	imshow(atom_window2,img[0]);
		ImageCut(simage_repath[0].c_str(),Overlap1.c_str(),point2.x,point2.y,iscale*hx,iscale*hy,"GTiff");


		CvPoint2D64f point22;
		point22.x = iscale*(dx-hx);///不行，应该把这些数都改为浮点数
		point22.y = 0;
		senPoint.push_back(point22);

		//	rectangle(img[1],cvPoint(dx-hx,0),cvPoint(dx,hy),cvScalar(255,255,0),2,4,0 );
		//	imshow(atom1_window2,img[1]);
		ImageCut(simage_senpath[0].c_str(),Overlap2.c_str(),point22.x,point22.y,iscale*hx,iscale*hy,"GTiff");


		//waitKey(0);
		break;
	case 3:
		CvPoint2D64f point3;
		point3.x = iscale*h1;///不行，应该把这些数都改为浮点数
		point3.y = iscale*(h2+hy);
		rePoint.push_back(point3);

		//	rectangle(img[0],cvPoint(h1,h2+hy),cvPoint(h1+hx,image1_ysize-hy),cvScalar(0,255,255),2,4,0 );
		//	imshow(atom_window3,img[0]);
		ImageCut(simage_repath[0].c_str(),Overlap1.c_str(),point3.x,point3.y,iscale*hx,iscale*hy,"GTiff");

		CvPoint2D64f point33;
		point33.x = 0;///不行，应该把这些数都改为浮点数
		point33.y = iscale*hy;
		senPoint.push_back(point33);

		//	rectangle(img[1],cvPoint(0,hy),cvPoint(hx,dy-hy),cvScalar(255,255,0),2,4,0 );
		//	imshow(atom1_window3,img[1]);
		ImageCut(simage_senpath[0].c_str(),Overlap2.c_str(),point33.x,point33.y,iscale*hx,iscale*(dy-2*hy),"GTiff");


		//waitKey(0);
		break;
	case 4:	
		CvPoint2D64f point4;
		point4.x = iscale*(h1+hx);///不行，应该把这些数都改为浮点数
		point4.y = iscale*(h2+hy);
		rePoint.push_back(point4);

		//	rectangle(img[0],cvPoint(h1+hx,h2+hy),cvPoint(image1_xsize-hx,image1_ysize-hy),cvScalar(0,255,255),2,4,0 );
		//	imshow(atom_window4,img[0]);
		ImageCut(simage_repath[0].c_str(),Overlap1.c_str(),point4.x,point4.y,iscale*(image1_xsize-2*hx),iscale*(image1_ysize-2*hy),"GTiff");

		CvPoint2D64f point44;
		point44.x = iscale*hx;///不行，应该把这些数都改为浮点数
		point44.y = iscale*hy;
		senPoint.push_back(point44);

		//		rectangle(img[1],cvPoint(hx,hy),cvPoint(dx-hx,dy-hy),cvScalar(255,255,0),2,4,0 );
		//		imshow(atom1_window4,img[1]);
		ImageCut(simage_senpath[0].c_str(),Overlap2.c_str(),point44.x,point44.y,iscale*(dx-2*hx),iscale*(dy-2*hy),"GTiff");




		//waitKey(0);
		break;
	case 5:
		CvPoint2D64f point5;
		point5.x = iscale*(image1_xsize-hx);///不行，应该把这些数都改为浮点数
		point5.y = iscale*(h2+hy);
		rePoint.push_back(point5);

		//		rectangle(img[0],cvPoint(image1_xsize-hx,h2+hy),cvPoint(image1_xsize,image1_ysize-hy),cvScalar(0,255,255),2,4,0 );
		//		imshow(atom_window5,img[0]);
		ImageCut(simage_repath[0].c_str(),Overlap1.c_str(),point5.x,point5.y,iscale*hx,iscale*(image1_ysize-2*hy),"GTiff");

		CvPoint2D64f point55;
		point55.x = iscale*(dx-hx);///不行，应该把这些数都改为浮点数
		point55.y = iscale*hy;
		senPoint.push_back(point55);
		//		rectangle(img[1],cvPoint(dx-hx,hy),cvPoint(dx,dy-hy),cvScalar(255,255,0),2,4,0 );
		//		imshow(atom1_window5,img[1]);
		ImageCut(simage_senpath[0].c_str(),Overlap2.c_str(),point55.x,point55.y,iscale*hx,iscale*(dy-2*hy),"GTiff");


		//waitKey(0);
		break;
	case 6:
		CvPoint2D64f point6;
		point6.x = iscale*h1;///不行，应该把这些数都改为浮点数
		point6.y = iscale*(image1_ysize-hy);
		rePoint.push_back(point6);

		//		rectangle(img[0],cvPoint(h1,image1_ysize-hy),cvPoint(h1+hx,image1_ysize),cvScalar(0,255,255),2,4,0 );
		//		imshow(atom_window6,img[0]);
		ImageCut(simage_repath[0].c_str(),Overlap1.c_str(),point6.x,point6.y,iscale*hx,iscale*hy,"GTiff");

		CvPoint2D64f point66;
		point66.x = 0;///不行，应该把这些数都改为浮点数
		point66.y = iscale*(dy-hy);
		senPoint.push_back(point66);
		//		rectangle(img[1],cvPoint(0,dy-hy),cvPoint(hx,dy),cvScalar(255,255,0),2,4,0 );
		//		imshow(atom1_window6,img[1]);
		ImageCut(simage_senpath[0].c_str(),Overlap2.c_str(),point66.x,point66.y,iscale*hx,iscale*hy,"GTiff");

		//	waitKey(0);
		break;
	case 7:
		CvPoint2D64f point7;
		point7.x = iscale*(h1+hx);///不行，应该把这些数都改为浮点数
		point7.y = iscale*(image1_ysize-hy);
		rePoint.push_back(point7);

		//		rectangle(img[0],cvPoint(h1+hx,image1_ysize-hy),cvPoint(image1_xsize-hx,image1_ysize),cvScalar(0,255,255),2,4,0 );
		//		imshow(atom_window7,img[0]);
		ImageCut(simage_repath[0].c_str(),Overlap1.c_str(),point7.x,point7.y,iscale*(image1_xsize-2*hx),iscale*hy,"GTiff");

		CvPoint2D64f point77;
		point77.x = iscale*hx;///不行，应该把这些数都改为浮点数
		point77.y = iscale*(dy-hy);
		senPoint.push_back(point77);
		//		rectangle(img[1],cvPoint(hx,dy-hy),cvPoint(dx-hx,dy),cvScalar(255,255,0),2,4,0 );
		//		imshow(atom1_window7,img[1]);
		ImageCut(simage_senpath[0].c_str(),Overlap2.c_str(),point77.x,point77.y,iscale*(dx-2*hx),iscale*hy,"GTiff");


		//waitKey(0);
		break;
	case 8:
		CvPoint2D64f point8;
		point8.x = iscale*(image1_xsize-hx);///不行，应该把这些数都改为浮点数
		point8.y = iscale*(image1_ysize-hy);
		rePoint.push_back(point8);

		//	rectangle(img[0],cvPoint(image1_xsize-hx,image1_ysize-hy),cvPoint(image1_xsize,image1_ysize),cvScalar(0,255,255),2,4,0 );
		//	imshow(atom_window8,img[0]);
		ImageCut(simage_repath[0].c_str(),Overlap1.c_str(),point8.x,point8.y,iscale*hx,iscale*hy,"GTiff");

		CvPoint2D64f point88;
		point88.x = iscale*(dx-hx);///不行，应该把这些数都改为浮点数
		point88.y = iscale*(dy-hy);
		senPoint.push_back(point88);
		//	rectangle(img[1],cvPoint(dx-hx,dy-hy),cvPoint(dx,dy),cvScalar(255,255,0),2,4,0 );
		//	imshow(atom1_window8,img[1]);
		ImageCut(simage_senpath[0].c_str(),Overlap2.c_str(),point88.x,point88.y,iscale*hx,iscale*hy,"GTiff");


		//waitKey(0);
		break;
	}


}



void ImageCut(const char* pszSrcFile, const char* pszDstFile, int iStartX, int iStartY, int iSizeX, int iSizeY,const char* pszFormat)
{
	GDALAllRegister();

	GDALDataset *pSrcDS = (GDALDataset*)GDALOpen(pszSrcFile, GA_ReadOnly);
	GDALDataType eDT = pSrcDS->GetRasterBand(1)->GetRasterDataType();

	int iBandCount = pSrcDS->GetRasterCount();

	// 根据裁切范围确定裁切后的图像宽高
	int iDstWidth  = iSizeX;
	int iDstHeight = iSizeY;

	double adfGeoTransform[6] = {0};
	pSrcDS->GetGeoTransform(adfGeoTransform);

	// 创建输出文件并设置空间参考和坐标信息
	GDALDriver *poDriver = (GDALDriver *) GDALGetDriverByName(pszFormat);
	GDALDataset *pDstDS = poDriver->Create(pszDstFile, iDstWidth, iDstHeight, iBandCount, eDT, NULL);
	pDstDS->SetGeoTransform(adfGeoTransform);
	pDstDS->SetProjection(pSrcDS->GetProjectionRef());

	int *pBandMap = new int[iBandCount];
	for (int i=0; i<iBandCount; i++)
		pBandMap[i] = i+1;

	if (eDT == GDT_Byte)	// 如果是8bit图像
	{
		// 申请所有数据所需要的缓存，如果图像太大应该用分块处理
		char *pDataBuff = new char[iDstWidth*iDstHeight*iBandCount];	

		pSrcDS->RasterIO(GF_Read, iStartX, iStartY, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);
		pDstDS->RasterIO(GF_Write, 0, 0, iSizeX, iSizeY, pDataBuff, iSizeX, iSizeY, eDT, iBandCount, pBandMap, 0, 0, 0);

		delete(pDataBuff);
	}
	else
	{
		// 其他类型的图像，与8bit类似，就是申请的缓存类型不同而已
	}

	delete(pBandMap);

	GDALClose((GDALDatasetH)pSrcDS);
	GDALClose((GDALDatasetH)pDstDS);
	return  ;

}


void Find_OverlapArea ( int ilayer )
{
	//计算从缩略图到原图的缩小倍数
	int iscale = static_cast<int>(pow(2.0,ilayer+1));

	rePoint.clear();	//参考图像中裁剪出来的小图左上角在参考图像中的坐标
	senPoint.clear();	//待配准图像中裁剪的小图左上角在待配准图像中的坐标

	//给出裁剪图像的路径
	string Overlap1 = refImg[0] + "refImage_Overlap.jpg";
	string Overlap2 = senImg[0] + "senImage_Overlap.jpg";
	refImg.push_back(Overlap1);
	senImg.push_back(Overlap2);

	int image1_xsize,image1_ysize; ///定义参考图像大小信息
	int image2_xsize,image2_ysize;//定义待配准图像大小信息

	CvMat* H;///用于临时保存H矩阵
	H = H_2[0]; //将第一次配准结果中的H矩阵传递过来。//此处也可以后续设计为一个函数传递的参数
	double px, py; //待配准图像相对于参考图像平移的距离，也可作为重叠区域左上角或者左下角的坐标
	double h1, h2, h3, h4;
	double dx,dy;///定义重叠区域的大小
	double hx,hy;///定义分块统计时，x和y方向每一小块的大小；
	px = cvmGet( H, 0, 2 );
	py = cvmGet( H, 1, 2 );
	h1 = abs(px) + 0.5;////为了四舍五入所以加上0.5，这一点还没有想明白
	h2 = abs(py) + 0.5;///故重叠区域左上角或者左下角的坐标即为：(h1,h2)

	///这里需要获取图像的大小参数
	//先获取参考图像这边的信息
	image1_xsize = img[0]->width;
	image1_ysize = img[0]->height;

	//再获取待配准图像的信息
	image2_xsize = img[1]->width;
	image2_ysize = img[1]->height;


	//计算重叠区域的大小
	dx = image1_xsize - h1;
	dy = image1_ysize - h2;
	Mat overlap((int)dx,(int)dy,CV_8UC1);
	Mat result;
	

	CvSeq *CurrentKeypoints = objectKeypoints;
	vector<int>ptpairs = ptpairs_befores;
	int num = ptpairs.size()/2;
	CvSURFPoint* r1;

	int image1_xsize,image1_ysize; ///定义参考图像大小信息
	int image2_xsize,image2_ysize;//定义待配准图像大小信息

	for (int i = 0; i < ptpairs.size(); i++)
	{
		r1 = (CvSURFPoint*)cvGetSeqElem(CurrentKeypoints, ptpairs[i] );
		cvmSet(overlap,r1->pt.x,r1->pt.y,1);
		i++;
	}

	integral(overlap,result,1);




}

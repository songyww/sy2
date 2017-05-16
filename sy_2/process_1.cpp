#include "process_1.h"



void OpenFile1()
{
	PyramidBase pyramid1; 
	simage_repath.clear();
	QString file_full1,file_name1,m_strFileName1;
	QString file_path1 = NULL;

	QFileInfo fi;		  
	file_full1 = QFileDialog::getOpenFileName(this,"open the second picture",file_path1);  
	fi = QFileInfo(file_full1);  
	m_strFileName1=fi.absoluteFilePath();

	reImage_full = m_strFileName1.toStdString();////QString 转换为string///图像完整路径
	int po = reImage_full.find_last_of('/');
	reFile_path = reImage_full.substr( 0,po+1); ///文件夹路径

	simage_repath.push_back(reImage_full); ///[0]中保存原始图像路径
	simage_repath.push_back(reFile_path);
	/*img.push_back(cvLoadImage(reImage_full.c_str()));*/
	pyramid1.InFilePath = reImage_full.c_str(); 

	pyramid1.Path = reFile_path.c_str();
	pyramid1.CheckPyramid();



	//下面这两句话可以放在配准过程中；暂定于此。待修改
		//String  str3("121212.jpg");
		//String str4 = reFile_path + str3; //结果图像为1111.jpg;str4表示结果图像的完整路径
		//resultImage_path = str4;

	//QString path1 = QString::fromStdString(str2);  ////string转换为Qstring




	/*QString str5 = QString::fromStdString(str4);*/
	//resultfile = cst_to_conchar(str4);

	//下面将QString转换为const char*类型
	//QByteArray aByteArray1 = m_strFileName1.toAscii();
	//InfilePath1 = aByteArray1.constData();//获取文件的完整路径

	/*InfilePath1 = str1.c_str();  *////将string类型直接转换为从const char*
	

	/*QByteArray aByteArray2 = path1.toAscii();*/
	/*pyramid1.Path =  aByteArray2.constData();*/	//获取文件夹的路径，用以保存金字塔文件


	//QByteArray aByteArray3 = str5.toAscii();
	////resultfile = aByteArray3.constData();///获取需要保存的文件路径
	//resultfile = string(aByteArray3.constData());
	


	for (int i = 0;i < pyramid1.iOverViewCount; i++)
	{
		char filepath[1024];
		//sprintf(filepath,"%s%d%s%d%s",reFile_path.c_str(),i,"_",i,".tif");
		//char* layerpath;
		pyramid1.ReadPyramidFile(1,i,filepath);
		string repath1(filepath);
		simage_repath.push_back(repath1);//[2]开始保存金字塔第0层图像
		//simage_repath[i] = pyramid1.PyramidLayer_Path[i];
		/*string strpath (pyramid1.PyramidLayer_Path[i]);
		simage_repath[i] = & strpath;*/
		//simage_repath[i] = layerpath;
	}
	
	

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

//	QMessageBox::information(this, tr("Information"),tr("王文"));
}

void OpenFile2()
{
	PyramidBase pyramid2; 
	simage_senpath.clear();
	QString file_full2,file_name2,m_strFileName2;
	QString file_path2 = NULL;

	QFileInfo fi;		  
	file_full2 = QFileDialog::getOpenFileName(this,"open the second picture",file_path2);  
	fi = QFileInfo(file_full2);  
	m_strFileName2=fi.absoluteFilePath();
	
	senImage_full = m_strFileName2.toStdString();
	int po = senImage_full.find_last_of('/');
	senFile_path = senImage_full.substr( 0, po + 1 );
	simage_senpath.push_back(senImage_full);///[0]保存文件路径
	simage_senpath.push_back(senFile_path);///[1]保存文件所在文件夹路径

	/*img.push_back(cvLoadImage(senImage_full.c_str())); *////在这一步骤中就已经将文件读入存储图像的容器中了
	pyramid2.InFilePath = senImage_full.c_str();

	pyramid2.Path = senFile_path.c_str();
	pyramid2.CheckPyramid();

	for (int i = 0;i < pyramid2.iOverViewCount; i++)
	{
		char filepath[1024];
		sprintf(filepath,"%s%d%s%d%s",senFile_path.c_str(),i,"_",i,".tif");
		//char * layerpath ;
		pyramid2.ReadPyramidFile(1,i,filepath);
		string senpath(filepath);
		simage_senpath.push_back(senpath);//[2]及以后保存金字塔各层图像，其中[2]保存第0层(原图像素的1/4 * 1/4)，[3]保存第1层(原图像素的1/8 * 1/8)
		//simage_senpath[i] = pyramid2.PyramidLayer_Path[i];
		
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

//	QMessageBox::information(this, tr("Information"),tr("宋蕴"));
}
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

	reImage_full = m_strFileName1.toStdString();////QString ת��Ϊstring///ͼ������·��
	int po = reImage_full.find_last_of('/');
	reFile_path = reImage_full.substr( 0,po+1); ///�ļ���·��

	simage_repath.push_back(reImage_full); ///[0]�б���ԭʼͼ��·��
	simage_repath.push_back(reFile_path);
	/*img.push_back(cvLoadImage(reImage_full.c_str()));*/
	pyramid1.InFilePath = reImage_full.c_str(); 

	pyramid1.Path = reFile_path.c_str();
	pyramid1.CheckPyramid();



	//���������仰���Է�����׼�����У��ݶ��ڴˡ����޸�
		//String  str3("121212.jpg");
		//String str4 = reFile_path + str3; //���ͼ��Ϊ1111.jpg;str4��ʾ���ͼ�������·��
		//resultImage_path = str4;

	//QString path1 = QString::fromStdString(str2);  ////stringת��ΪQstring




	/*QString str5 = QString::fromStdString(str4);*/
	//resultfile = cst_to_conchar(str4);

	//���潫QStringת��Ϊconst char*����
	//QByteArray aByteArray1 = m_strFileName1.toAscii();
	//InfilePath1 = aByteArray1.constData();//��ȡ�ļ�������·��

	/*InfilePath1 = str1.c_str();  *////��string����ֱ��ת��Ϊ��const char*
	

	/*QByteArray aByteArray2 = path1.toAscii();*/
	/*pyramid1.Path =  aByteArray2.constData();*/	//��ȡ�ļ��е�·�������Ա���������ļ�


	//QByteArray aByteArray3 = str5.toAscii();
	////resultfile = aByteArray3.constData();///��ȡ��Ҫ������ļ�·��
	//resultfile = string(aByteArray3.constData());
	


	for (int i = 0;i < pyramid1.iOverViewCount; i++)
	{
		char filepath[1024];
		//sprintf(filepath,"%s%d%s%d%s",reFile_path.c_str(),i,"_",i,".tif");
		//char* layerpath;
		pyramid1.ReadPyramidFile(1,i,filepath);
		string repath1(filepath);
		simage_repath.push_back(repath1);//[2]��ʼ�����������0��ͼ��
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

//	QMessageBox::information(this, tr("Information"),tr("����"));
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
	simage_senpath.push_back(senImage_full);///[0]�����ļ�·��
	simage_senpath.push_back(senFile_path);///[1]�����ļ������ļ���·��

	/*img.push_back(cvLoadImage(senImage_full.c_str())); *////����һ�����о��Ѿ����ļ�����洢ͼ�����������
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
		simage_senpath.push_back(senpath);//[2]���Ժ󱣴����������ͼ������[2]�����0��(ԭͼ���ص�1/4 * 1/4)��[3]�����1��(ԭͼ���ص�1/8 * 1/8)
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

//	QMessageBox::information(this, tr("Information"),tr("����"));
}
/*
 *	@brief Pyramid���Ա������ʵ��
 */

#include "Pyramid_with_GDAL.h"

bool PyramidBase::CheckPyramid()
{
	GDALAllRegister();
	InFile = (GDALDataset*)GDALOpen(InFilePath, GA_ReadOnly);		//��·���е��ļ����뵽���ݼ���
	//�ж�ͼ���Ƿ�������ݼ���
	if (InFile == NULL)
	{
		printf("��ͼ��ʧ�ܣ�����ͼ���Ƿ���ڻ��ļ��Ƿ���ͼ���ļ���");
		return false;
	}
	iBandCount = InFile->GetRasterCount();		//��ʼ������ͼ��Ĳ�����
	InRasterBand = InFile->GetRasterBand(1);		//��ʼ������ͼ��ĵ�һ�����Σ����Ժ����ȡ����������Ϣ
	iWidth = InFile->GetRasterXSize();		//��ȡ����ͼ����X����Ĵ�С
	iHeight = InFile->GetRasterYSize();		//��ȡ����ͼ�����Y����Ĵ�С
	InProjectionRef = InFile->GetProjectionRef();		//��ȡ����ͼ���ͶӰ��Ϣ
	InFile->GetGeoTransform(InGeotransform);		//��ȡ����ͼ��ı任����
	hDriver = GDALGetDatasetDriver(InFile);			//��ȡͼ������
	InFormat = GDALGetDriverShortName(hDriver);		//��ȡͼ������������
	iPixelNum = iWidth * iHeight;		//����ͼ��������Ԫ����
	iCurNum = iPixelNum/4;
	iOverViewCount = InRasterBand->GetOverviewCount();
	//�����ж��ļ����Ƿ��н�����,��������н���������iOverViewCount=0
	if ( iOverViewCount == 0 )	//���������н��������򷵻���Ϣ��
	{
		printf("ͼ��û�д����������ļ����봴����������");
		CreatePyramid();
		InRasterBand = InFile->GetRasterBand(1);
		iOverViewCount = InRasterBand->GetOverviewCount();
		return FALSE;
		
	}
	else
	{
		printf("ͼ���Ѿ�������������");
		return TRUE;
	}
}

bool PyramidBase::CreatePyramid()
{
	//////////////////////////////////////////////////////////////////////////
	////�÷��������Ľ���������[0]�������ͼ��x��y ����Ĵ�С�ֱ���ԭͼ��4��֮1���ӵ�[1]�㿪ʼ����Ľ�����ͼ������һ��ͼ��x��y ��2��֮1
	//iTopNum = 4096;		//�����������С��64*64
	//iTopNum = 16384;		//�����������С��128*128
	//iTopNum = 65536;		//�����������С��256*256
	iTopNum = 262144;		//�����������С��512*512
	int anLevels[1024] ={ 0 };//�����������б����ֱ�ʾ��ǰ����������ͼ���СΪԭͼ��N��֮һ
	int nLevelCount = 0;		//����������

	//���ȳ�ʼ��һ����������
	CConsoleProcess *pProcess = new CConsoleProcess;

	//���濪ʼ����������
	if (pProcess != NULL)  
	{  
		pProcess->SetMessage("����������");  

	} 
	GDALAllRegister();

	//����һ�䣬���ô����Ľ������ļ�������
	CPLSetConfigOption("USE_RRD","NO");		//��ΪNO���򴴽�.ovr��ʽ�Ľ���������ΪYES���򴴽�.aus��ʽ�Ľ�����

	//����ļ���Erdas��img����PCI��pix��ʽ�������ڽ������������Ĵ����������
	if (EQUAL(InFormat, "HFA") || EQUAL(InFormat,"PCIDSK"))
	{
		GDALClose(InFile);
		InFile = (GDALDataset*)GDALOpen(InFilePath, GA_Update);
		if (InFile == NULL)
		{
			printf("��ͼ��ʧ�ܣ�����ͼ���Ƿ���ڻ��ļ��Ƿ���ͼ���ļ���");
			return false;
		}
	}


	//���濪ʼ����������
	do    //����������������ӵڶ���������  
	{  
		anLevels[nLevelCount] = static_cast<int>(pow(2.0, nLevelCount+2));  
		nLevelCount ++;  
		iCurNum /= 4;  
	} while (iCurNum > iTopNum); 

	const char      *pszResampling = "nearest"; //������ʽ  
	GDALProgressFunc pfnProgress = GDALTermProgress;//������  

	/* -------------------------------------------------------------------- */  
	/*      Generate overviews.                                             */  
	/* -------------------------------------------------------------------- */  
	if (nLevelCount > 0 &&  
		GDALBuildOverviews( InFile,pszResampling, nLevelCount, anLevels,  
		0, NULL, pfnProgress, pProcess ) != CE_None )  
	{  
		if (pProcess != NULL)  
			return true ;  
	}  

	if (pProcess != NULL)  
		pProcess->SetMessage("������������ɣ�");  

	return true;  
}


int PyramidBase::p_pow(int a,int b)//�ú���ʵ��a��b�η�
{
	int c = 1;
	for(int i = 0;i < b;i++)
	{
		c = c*a;
	}
	return c;

}

//�������ڶ�ȡ������ĳһ��ĺ������Լ���ȡʱ��ͼ�������ã�m:��ͼ���m�Σ�n��ʾ��ȡ�ڼ��������
void PyramidBase::ReadPyramidFile(int m, int n,  char * filepath)
{
	GDALAllRegister();
	//�����ȡָ����ʽ�����������ڴ���ָ����ʽ��ͼ��,InFormatΪǰ���Ѿ���ȡ������ͼ�����������
	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(InFormat);

	/*
	 *	@brief ��������ļ���·����ͶӰ��Ϣ���Լ��任����
	 */
	
	sprintf(filepath,"%s%s%d%s%d%s",FilePath,"//",m,"_",n,".jpg");
	const char* OutFilepath = filepath;
	//PyramidLayer_Path[n] = filepath;	//����������n��ͼ��·�����ݸ����������еı����������ͼ��·���ĳ�Ա������
	
	InPyramidBand = InRasterBand->GetOverview(n);			//InPyramidBand�д������ͼ���һ�����ε�n�����������Ҫ���г�ʼ��
	GDALDataset* OutFile = poDriver->Create(OutFilepath, InPyramidBand->GetXSize(),InPyramidBand->GetYSize(),iBandCount, GDT_Byte, NULL ) ;
	OutFile->SetProjection(InProjectionRef);
	OutFile->SetGeoTransform(InGeotransform);

	//�ֲ��Σ��ֿ��ȡ�ļ�
	int nXBlocks, nYBlocks, nXBlockSize, nYBlockSize;//nXBlocks |nYBlocks ��XY����ֿ����Ŀ��nXBlockSize, nYBlockSize���ֵĿ���xy�����ϵĴ�С
	int iXBlock, iYBlock;
	GByte *pabyData;//���ݷֵĿ�Ĵ�СΪ�û��������Ӧ�Ŀռ�

	//���ü���ֿ����Ŀ���������Ϊm
	nXBlocks = InPyramidBand->GetXSize()/4000;	//����x����ֵĿ���
	nYBlocks = InPyramidBand->GetYSize()/4000;	//xy����ֵĿ���һ����

	for ( int i = 1; i <= iBandCount; i++ )
	{
		InPyramidBand = InFile->GetRasterBand(i)->GetOverview(n);	//����Ĳ���nΪ��ȡ��һ�������
		GDALRasterBand* OutBand = OutFile->GetRasterBand(i);
		iOverViewCount = InFile->GetRasterBand(i)->GetOverviewCount();
		CPLAssert( InPyramidBand->GetRasterDataType()  ==  GDT_Byte);
		nYBlockSize = InPyramidBand->GetYSize() / nYBlocks;
		nXBlockSize = InPyramidBand->GetXSize() / nXBlocks;
		pabyData = (GByte *)CPLMalloc( nXBlockSize * nYBlockSize );

		for ( iYBlock = 0;iYBlock < nYBlocks; iYBlock++ )
		{
			for ( iXBlock = 0; iXBlock < nXBlocks; iXBlock++ )
			{
				InPyramidBand->RasterIO(GF_Read, iXBlock *nXBlockSize,iYBlock*nYBlockSize,nXBlockSize,nYBlockSize,pabyData,nXBlockSize, nYBlockSize, GDT_Byte, 0,0);

				OutBand->RasterIO(GF_Write,iXBlock*nXBlockSize,iYBlock*nYBlockSize,nXBlockSize,nYBlockSize,pabyData,nXBlockSize,nYBlockSize, GDT_Byte,0,0);
			}

		}

	}

}


//���浥�����εĽ�����ĳһ���ͼ��,�� n:��n��������� filepath:���������ͼ��·��
void PyramidBase::SaveSingleBand(int n, char *filepath)//,GDALDataset *pInFile)
{

	GDALAllRegister();
	//�����ȡָ����ʽ�����������ڴ���ָ����ʽ��ͼ��,InFormatΪǰ���Ѿ���ȡ������ͼ�����������
	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(InFormat);

	/*
	 *	@brief ��������ļ���·����ͶӰ��Ϣ���Լ��任����
	 */
	//sprintf(filepath,"%s%d%s%d%s",Path,m,"_",n,".tif");
	sprintf(filepath,"%s%s%d%s",LayerPath,"/",n,".jpg");
	const char* OutFilepath = filepath;
	//PyramidLayer_Path[n] = filepath;	//����������n��ͼ��·�����ݸ����������еı����������ͼ��·���ĳ�Ա������
	
	InPyramidBand = InRasterBand->GetOverview(n);//InPyramidBand->GetOverview(n);	//����Ĳ���nΪ��ȡ��һ�������//InPyramidBand = InRasterBand->GetOverview(n);			//InPyramidBand�д������ͼ���һ�����ε�n�����������Ҫ���г�ʼ��
	GDALDataset* OutFile = poDriver->Create(OutFilepath, InPyramidBand->GetXSize(),InPyramidBand->GetYSize(),1, GDT_Byte, NULL ) ;
	OutFile->SetProjection(InProjectionRef);
	OutFile->SetGeoTransform(InGeotransform);


	//�ֲ��Σ��ֿ��ȡ�ļ�
	int nXBlocks, nYBlocks, nXBlockSize, nYBlockSize;//nXBlocks |nYBlocks ��XY����ֿ����Ŀ��nXBlockSize, nYBlockSize���ֵĿ���xy�����ϵĴ�С
	int iXBlock, iYBlock;
	GByte *pabyData;//���ݷֵĿ�Ĵ�СΪ�û��������Ӧ�Ŀռ�

	InPyramidBand = InRasterBand->GetOverview(n);
	//���ü���ֿ����Ŀ���������Ϊm
	nXBlocks = 2;//InPyramidBand->GetXSize()/4000;//���ͼ��ߴ����4000,���������4000�Ŀ���ж�д
	nYBlocks = 2;//InPyramidBand->GetYSize()/4000;;//
	
	
	GDALRasterBand* OutBand = OutFile->GetRasterBand(1);
	//iOverViewCount = InPyramidBand->GetOverviewCount();
	CPLAssert( InPyramidBand->GetRasterDataType()  ==  GDT_Byte);
	nYBlockSize = InPyramidBand->GetYSize() / nYBlocks;
	nXBlockSize = InPyramidBand->GetXSize() / nXBlocks;
	pabyData = (GByte *)CPLMalloc( nXBlockSize * nYBlockSize );

	for ( iYBlock = 0;iYBlock < nYBlocks; iYBlock++ )
	{
		for ( iXBlock = 0; iXBlock < nXBlocks; iXBlock++ )
		{
			InPyramidBand->RasterIO(GF_Read, iXBlock *nXBlockSize,iYBlock*nYBlockSize,nXBlockSize,nYBlockSize,pabyData,nXBlockSize, nYBlockSize, GDT_Byte, 0,0);

			OutBand->RasterIO(GF_Write,iXBlock*nXBlockSize,iYBlock*nYBlockSize,nXBlockSize,nYBlockSize,pabyData,nXBlockSize,nYBlockSize, GDT_Byte,0,0);
		}

	}

}
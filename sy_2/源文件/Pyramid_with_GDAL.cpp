/*
 *	@brief Pyramid类成员函数的实现
 */

#include "Pyramid_with_GDAL.h"

bool PyramidBase::CheckPyramid()
{
	GDALAllRegister();
	//InFile = (GDALDataset*)GDALOpen(InFilePath, GA_ReadOnly);		//将路径中的文件读入到数据集中
	//判断图像是否读入数据集中
	if (InFile == NULL)
	{
		//QMessageBox::information(this, tr("Information"),tr("打开图像失败，请选择正确路径！"));
		return false;
	}
	iBandCount = InFile->GetRasterCount();		//初始化输入图像的波段数
	InRasterBand = InFile->GetRasterBand(1);		//初始化输入图像的第一个波段；用以后面获取金字塔的信息
	iWidth = InFile->GetRasterXSize();		//获取输入图像本身X方向的大小
	iHeight = InFile->GetRasterYSize();		//获取输入图像本身的Y方向的大小
	InProjectionRef = InFile->GetProjectionRef();		//获取输入图像的投影信息
	InFile->GetGeoTransform(InGeotransform);		//获取输入图像的变换参数
	hDriver = GDALGetDatasetDriver(InFile);			//获取图像驱动
	InFormat = GDALGetDriverShortName(hDriver);		//获取图像驱动的名称
	iPixelNum = iWidth * iHeight;		//输入图像中总像元个数
	iCurNum = iPixelNum/4;
	iOverViewCount = InRasterBand->GetOverviewCount();
	//下面判断文件中是否含有金字塔,如果不含有金字塔，则iOverViewCount=0
	if ( iOverViewCount == 0 )	//表明不含有金字塔，则返回信息；
	{
		printf("图像没有创建金字塔文件，请创建金字塔！");
		CreatePyramid();
		InRasterBand = InFile->GetRasterBand(1);
		iOverViewCount = InRasterBand->GetOverviewCount();
		return FALSE;
		
	}
	else
	{
		printf("图像已经创建金字塔！");
		return TRUE;
	}
}

bool PyramidBase::CreatePyramid()
{
	//////////////////////////////////////////////////////////////////////////
	////该方法创建的金字塔，第[0]层金字塔图像x和y 方向的大小分别是原图的4分之1，从第[1]层开始往后的金字塔图像是上一层图像x和y 的2分之1
	//iTopNum = 4096;		//顶层金字塔大小，64*64
	//iTopNum = 16384;		//顶层金字塔大小，128*128
	//iTopNum = 65536;		//顶层金字塔大小，256*256
	iTopNum = 262144;		//顶层金字塔大小，512*512
	int anLevels[1024] ={ 0 };//金字塔级别列表，数字表示当前金字塔层中图像大小为原图的N分之一
	int nLevelCount = 0;		//金字塔层数

	//首先初始化一个进度条；
	CConsoleProcess *pProcess = new CConsoleProcess;

	//下面开始创建金字塔
	if (pProcess != NULL)  
	{  
		pProcess->SetMessage("创建金字塔");  

	} 
	GDALAllRegister();

	//下面一句，设置创建的金字塔文件的类型
	CPLSetConfigOption("USE_RRD","NO");		//若为NO，则创建.ovr格式的金字塔；若为YES，则创建.aus格式的金字塔

	//如果文件是Erdas的img或者PCI的pix格式，创建内金字塔，其他的创建外金字塔
	if (EQUAL(InFormat, "HFA") || EQUAL(InFormat,"PCIDSK"))
	{
		GDALClose(InFile);
		InFile = (GDALDataset*)GDALOpen(InFilePath, GA_Update);
		if (InFile == NULL)
		{
			printf("打开图像失败，请检查图像是否存在或文件是否是图像文件！");
			return false;
		}
	}


	//下面开始构建金字塔
	do    //计算金字塔级数，从第二级到顶层  
	{  
		anLevels[nLevelCount] = static_cast<int>(pow(2.0, nLevelCount+2));  
		nLevelCount ++;  
		iCurNum /= 4;  
	} while (iCurNum > iTopNum); 

	const char      *pszResampling = "nearest"; //采样方式  
	GDALProgressFunc pfnProgress = GDALTermProgress;//进度条  

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
		pProcess->SetMessage("创建金字塔完成！");  

	return true;  
}



bool PyramidBase::sCheckPyramid()
{
	GDALAllRegister();
	//InFile = (GDALDataset*)GDALOpen(InFilePath, GA_ReadOnly);		//将路径中的文件读入到数据集中
	//判断图像是否读入数据集中
	if (InFile == NULL)
	{
		//QMessageBox::information(this, tr("Information"),tr("打开图像失败，请选择正确路径！"));
		return false;
	}
	iBandCount = InFile->GetRasterCount();		//初始化输入图像的波段数
	InRasterBand = InFile->GetRasterBand(1);		//初始化输入图像的第一个波段；用以后面获取金字塔的信息
	iWidth = InFile->GetRasterXSize();		//获取输入图像本身X方向的大小
	iHeight = InFile->GetRasterYSize();		//获取输入图像本身的Y方向的大小
	InProjectionRef = InFile->GetProjectionRef();		//获取输入图像的投影信息
	InFile->GetGeoTransform(InGeotransform);		//获取输入图像的变换参数
	hDriver = GDALGetDatasetDriver(InFile);			//获取图像驱动
	InFormat = GDALGetDriverShortName(hDriver);		//获取图像驱动的名称
	iPixelNum = iWidth * iHeight;		//输入图像中总像元个数
	iCurNum = iPixelNum;
	iOverViewCount = InRasterBand->GetOverviewCount();
	//下面判断文件中是否含有金字塔,如果不含有金字塔，则iOverViewCount=0
// 	if ( iOverViewCount == 0 )	//表明不含有金字塔，则返回信息；
// 	{
		//printf("图像没有创建金字塔文件，请创建金字塔！");
		sCreatePyramid();
		InRasterBand = InFile->GetRasterBand(1);
		iOverViewCount = InRasterBand->GetOverviewCount();
		return FALSE;
// 
// 	}
// 	else
// 	{
// 		printf("图像已经创建金字塔！");
// 		return TRUE;
// 	}
}
bool PyramidBase::sCheckPyramid( int TopLayerSize)
{
	GDALAllRegister();
	//InFile = (GDALDataset*)GDALOpen(InFilePath, GA_ReadOnly);		//将路径中的文件读入到数据集中
	//判断图像是否读入数据集中
	if (InFile == NULL)
	{
		//QMessageBox::information(this, tr("Information"),tr("打开图像失败，请选择正确路径！"));
		return false;
	}
	iBandCount = InFile->GetRasterCount();		//初始化输入图像的波段数
	InRasterBand = InFile->GetRasterBand(1);		//初始化输入图像的第一个波段；用以后面获取金字塔的信息
	iWidth = InFile->GetRasterXSize();		//获取输入图像本身X方向的大小
	iHeight = InFile->GetRasterYSize();		//获取输入图像本身的Y方向的大小
	InProjectionRef = InFile->GetProjectionRef();		//获取输入图像的投影信息
	InFile->GetGeoTransform(InGeotransform);		//获取输入图像的变换参数
	hDriver = GDALGetDatasetDriver(InFile);			//获取图像驱动
	InFormat = GDALGetDriverShortName(hDriver);		//获取图像驱动的名称
	iPixelNum = iWidth * iHeight;		//输入图像中总像元个数
	iCurNum = iPixelNum;
	iOverViewCount = InRasterBand->GetOverviewCount();
	//下面判断文件中是否含有金字塔,如果不含有金字塔，则iOverViewCount=0
	// 	if ( iOverViewCount == 0 )	//表明不含有金字塔，则返回信息；
	// 	{
	//printf("图像没有创建金字塔文件，请创建金字塔！");
	sCreatePyramid(TopLayerSize);
	InRasterBand = InFile->GetRasterBand(1);
	iOverViewCount = InRasterBand->GetOverviewCount();
	return FALSE;
	// 
	// 	}
	// 	else
	// 	{
	// 		printf("图像已经创建金字塔！");
	// 		return TRUE;
	// 	}
}

bool PyramidBase::sCreatePyramid()
{
	//////////////////////////////////////////////////////////////////////////
	////该方法创建的金字塔，第[0]层金字塔图像x和y 方向的大小分别是原图的4分之1，从第[1]层开始往后的金字塔图像是上一层图像x和y 的2分之1
	//iTopNum = 4096;		//顶层金字塔大小，64*64
	iTopNum = 16384;		//顶层金字塔大小，128*128
	//iTopNum = 65536;		//顶层金字塔大小，256*256
	//iTopNum = 262144;		//顶层金字塔大小，512*512
	int anLevels[1024] ={ 0 };//金字塔级别列表，数字表示当前金字塔层中图像大小为原图的N分之一
	int nLevelCount = 0;		//金字塔层数

	//首先初始化一个进度条；
	CConsoleProcess *pProcess = new CConsoleProcess;

	//下面开始创建金字塔
	if (pProcess != NULL)  
	{  
		pProcess->SetMessage("创建金字塔");  

	} 
	GDALAllRegister();

	//下面一句，设置创建的金字塔文件的类型
	CPLSetConfigOption("USE_RRD","NO");		//若为NO，则创建.ovr格式的金字塔；若为YES，则创建.aus格式的金字塔

	//如果文件是Erdas的img或者PCI的pix格式，创建内金字塔，其他的创建外金字塔
	if (EQUAL(InFormat, "HFA") || EQUAL(InFormat,"PCIDSK"))
	{
		GDALClose(InFile);
		InFile = (GDALDataset*)GDALOpen(InFilePath, GA_Update);
		if (InFile == NULL)
		{
			printf("打开图像失败，请检查图像是否存在或文件是否是图像文件！");
			return false;
		}
	}


	//下面开始构建金字塔
	do    //计算金字塔级数，从第二级到顶层  
	{  
		anLevels[nLevelCount] = static_cast<int>(pow(2.0, nLevelCount+1));  
		nLevelCount ++;  
		iCurNum /= 4;  
	} while (iCurNum > iTopNum); 

	const char      *pszResampling = "nearest"; //采样方式  
	GDALProgressFunc pfnProgress = GDALTermProgress;//进度条  

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
		pProcess->SetMessage("创建金字塔完成！");  

	return true;  
}

bool PyramidBase::sCreatePyramid(int TopLayerSize)
{
	//////////////////////////////////////////////////////////////////////////
	////该方法创建的金字塔，第[0]层金字塔图像x和y 方向的大小分别是原图的4分之1，从第[1]层开始往后的金字塔图像是上一层图像x和y 的2分之1
	//iTopNum = 4096;		//顶层金字塔大小，64*64
	iTopNum = TopLayerSize;		//顶层金字塔大小，128*128
	//iTopNum = 65536;		//顶层金字塔大小，256*256
	//iTopNum = 262144;		//顶层金字塔大小，512*512
	int anLevels[1024] ={ 0 };//金字塔级别列表，数字表示当前金字塔层中图像大小为原图的N分之一
	int nLevelCount = 0;		//金字塔层数

	//首先初始化一个进度条；
	CConsoleProcess *pProcess = new CConsoleProcess;

	//下面开始创建金字塔
	if (pProcess != NULL)  
	{  
		pProcess->SetMessage("创建金字塔");  

	} 
	GDALAllRegister();

	//下面一句，设置创建的金字塔文件的类型
	CPLSetConfigOption("USE_RRD","NO");		//若为NO，则创建.ovr格式的金字塔；若为YES，则创建.aus格式的金字塔

	//如果文件是Erdas的img或者PCI的pix格式，创建内金字塔，其他的创建外金字塔
	if (EQUAL(InFormat, "HFA") || EQUAL(InFormat,"PCIDSK"))
	{
		GDALClose(InFile);
		InFile = (GDALDataset*)GDALOpen(InFilePath, GA_Update);
		if (InFile == NULL)
		{
			printf("打开图像失败，请检查图像是否存在或文件是否是图像文件！");
			return false;
		}
	}


	//下面开始构建金字塔
	do    //计算金字塔级数，从第二级到顶层  
	{  
		anLevels[nLevelCount] = static_cast<int>(pow(2.0, nLevelCount+1));  
		nLevelCount ++;  
		iCurNum /= 4;  
	} while (iCurNum > iTopNum); 

	const char      *pszResampling = "nearest"; //采样方式  
	GDALProgressFunc pfnProgress = GDALTermProgress;//进度条  

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
		pProcess->SetMessage("创建金字塔完成！");  

	return true;  
}



int PyramidBase::p_pow(int a,int b)//该函数实现a的b次方
{
	int c = 1;
	for(int i = 0;i < b;i++)
	{
		c = c*a;
	}
	return c;

}

//定义用于读取金字塔某一层的函数，以及读取时的图像块的设置；m:将图像分m次；n表示读取第几层金字塔
void PyramidBase::ReadPyramidFile(int m, int n,  char * filepath)
{
	GDALAllRegister();
	//下面获取指定格式的驱动，用于创建指定格式的图像,InFormat为前面已经获取的输入图像的驱动名称
	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(InFormat);

	/*
	 *	@brief 设置输出文件的路径、投影信息、以及变换参数
	 */
	
	sprintf(filepath,"%s%s%d%s%d%s",FilePath,"//",m,"_",n,".jpg");
	const char* OutFilepath = filepath;
	//PyramidLayer_Path[n] = filepath;	//将金字塔第n层图像路径传递给金字塔类中的保存金字塔层图像路径的成员变量，
	
	InPyramidBand = InRasterBand->GetOverview(n);			//InPyramidBand中存放输入图像第一个波段第n层金字塔；需要进行初始化
	GDALDataset* OutFile = poDriver->Create(OutFilepath, InPyramidBand->GetXSize(),InPyramidBand->GetYSize(),iBandCount, GDT_Byte, NULL ) ;
	OutFile->SetProjection(InProjectionRef);
	OutFile->SetGeoTransform(InGeotransform);

	//分波段，分块读取文件
	int nXBlocks, nYBlocks, nXBlockSize, nYBlockSize;//nXBlocks |nYBlocks ：XY反向分块的数目；nXBlockSize, nYBlockSize：分的块在xy方向上的大小
	int iXBlock, iYBlock;
	GByte *pabyData;//根据分的块的大小为该缓存分配相应的空间

	//设置计算分块的数目；传入参数为m
	nXBlocks = InPyramidBand->GetXSize()/4000;	//计算x方向分的块数
	nYBlocks = InPyramidBand->GetYSize()/4000;	//xy方向分的快数一样多

	for ( int i = 1; i <= iBandCount; i++ )
	{
		InPyramidBand = InFile->GetRasterBand(i)->GetOverview(n);	//传入的参数n为读取那一层金字塔
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


////定义用于读取金字塔某一层的函数，以及读取时的图像块的设置；m:将图像分m次；n表示读取第几层金字塔
//void PyramidBase::sReadPyramidFile( int n,  char * filepath)
//{
//	GDALAllRegister();
//	//下面获取指定格式的驱动，用于创建指定格式的图像,InFormat为前面已经获取的输入图像的驱动名称
//	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("MEM");
//
//	/*
//	 *	@brief 设置输出文件的路径、投影信息、以及变换参数
//	 */
//	
//	sprintf(filepath,"%s%s%%s%d%s",FilePath,"//","1_",n,".jpg");
//	const char* OutFilepath = filepath;
//	//PyramidLayer_Path[n] = filepath;	//将金字塔第n层图像路径传递给金字塔类中的保存金字塔层图像路径的成员变量，
//	
//	InPyramidBand = InRasterBand->GetOverview(n);			//InPyramidBand中存放输入图像第一个波段第n层金字塔；需要进行初始化
//	GDALDataset* OutFile = poDriver->Create(OutFilepath, InPyramidBand->GetXSize(),InPyramidBand->GetYSize(),iBandCount, GDT_Byte, NULL ) ;
//	
//	//分波段，分块读取文件
//	int  nXBlockSize, nYBlockSize;//nXBlocks |nYBlocks ：XY反向分块的数目；nXBlockSize, nYBlockSize：分的块在xy方向上的大小
//	int iXBlock, iYBlock;
//	GByte *pabyData;//根据分的块的大小为该缓存分配相应的空间
//
//
//	for ( int i = 1; i <= iBandCount; i++ )
//	{
//		InPyramidBand = InFile->GetRasterBand(i)->GetOverview(n);	//传入的参数n为读取那一层金字塔
//		GDALRasterBand* OutBand = OutFile->GetRasterBand(i);
//		iOverViewCount = InFile->GetRasterBand(i)->GetOverviewCount();
//		CPLAssert( InPyramidBand->GetRasterDataType()  ==  GDT_Byte);
//		nYBlockSize = InPyramidBand->GetYSize() ;
//		nXBlockSize = InPyramidBand->GetXSize() ;
//		pabyData = (GByte *)CPLMalloc( nXBlockSize * nYBlockSize );
//
//				InPyramidBand->RasterIO(GF_Read,0,0,nXBlockSize,nYBlockSize,pabyData,nXBlockSize, nYBlockSize, GDT_Byte, 0,0);
//
//				OutBand->RasterIO(GF_Write,0,0,nXBlockSize,nYBlockSize,pabyData,nXBlockSize,nYBlockSize, GDT_Byte,0,0);
//	}		
//
//}
//



//保存单个波段的金字塔某一层的图像,； n:第n层金字塔； filepath:保存金字塔图像路径
void PyramidBase::SaveSingleBand(int n, char *filepath)//,GDALDataset *pInFile)
{

	GDALAllRegister();
	//下面获取指定格式的驱动，用于创建指定格式的图像,InFormat为前面已经获取的输入图像的驱动名称
	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(InFormat);

	/*
	 *	@brief 设置输出文件的路径、投影信息、以及变换参数
	 */
	//sprintf(filepath,"%s%d%s%d%s",Path,m,"_",n,".tif");
	sprintf(filepath,"%s%s%d%s",LayerPath,"/",n,".jpg");
	const char* OutFilepath = filepath;
	//PyramidLayer_Path[n] = filepath;	//将金字塔第n层图像路径传递给金字塔类中的保存金字塔层图像路径的成员变量，
	
	InPyramidBand = InRasterBand->GetOverview(n);//InPyramidBand->GetOverview(n);	//传入的参数n为读取那一层金字塔//InPyramidBand = InRasterBand->GetOverview(n);			//InPyramidBand中存放输入图像第一个波段第n层金字塔；需要进行初始化
	GDALDataset* OutFile = poDriver->Create(OutFilepath, InPyramidBand->GetXSize(),InPyramidBand->GetYSize(),1, GDT_Byte, NULL ) ;
	OutFile->SetProjection(InProjectionRef);
	OutFile->SetGeoTransform(InGeotransform);


	//分波段，分块读取文件
	int nXBlocks, nYBlocks, nXBlockSize, nYBlockSize;//nXBlocks |nYBlocks ：XY反向分块的数目；nXBlockSize, nYBlockSize：分的块在xy方向上的大小
	int iXBlock, iYBlock;
	GByte *pabyData;//根据分的块的大小为该缓存分配相应的空间

	InPyramidBand = InRasterBand->GetOverview(n);
	//设置计算分块的数目；传入参数为m
	nXBlocks = 2;//InPyramidBand->GetXSize()/4000;//如果图像尺寸大于4000,则按照最大是4000的块进行读写
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



//保存图像金字塔中某一层所有波段的图像，即原始图像对应金字塔层的原始图像，非单个波段图像
//n:第n层金字塔； filepath:保存金字塔图像路径
void PyramidBase::SaveAllBand(int n, char *filepath)
{
	GDALAllRegister();
	//下面获取指定格式的驱动，用于创建指定格式的图像,InFormat为前面已经获取的输入图像的驱动名称
	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(InFormat);

	/*
	 *	@brief 设置输出文件的路径、投影信息、以及变换参数
	 */
	//sprintf(filepath,"%s%d%s%d%s",Path,m,"_",n,".tif");
	sprintf(filepath,"%s%s%d%s",LayerPath,"/",n,".jpg");
	const char* OutFilepath = filepath;
	//PyramidLayer_Path[n] = filepath;	//将金字塔第n层图像路径传递给金字塔类中的保存金字塔层图像路径的成员变量，
	
	InPyramidBand = InRasterBand->GetOverview(n);//InPyramidBand->GetOverview(n);	//传入的参数n为读取那一层金字塔//InPyramidBand = InRasterBand->GetOverview(n);			//InPyramidBand中存放输入图像第一个波段第n层金字塔；需要进行初始化
	GDALDataset* OutFile = poDriver->Create(OutFilepath, InPyramidBand->GetXSize(),InPyramidBand->GetYSize(),iBandCount, GDT_Byte, NULL ) ;
	OutFile->SetProjection(InProjectionRef);
	OutFile->SetGeoTransform(InGeotransform);


	//分波段，分块读取文件
	int nXBlocks, nYBlocks, nXBlockSize, nYBlockSize;//nXBlocks |nYBlocks ：XY反向分块的数目；nXBlockSize, nYBlockSize：分的块在xy方向上的大小
	int iXBlock, iYBlock;
	GByte *pabyData;//根据分的块的大小为该缓存分配相应的空间  
	
	for (int i = 1;i <= iBandCount; ++i)
	{
		InPyramidBand = InFile->GetRasterBand(i)->GetOverview(n);
		//设置计算分块的数目；传入参数为m
		if (InPyramidBand->GetXSize() <= 4000)
		{
			nXBlocks = 1;
		}
		else
		{
			nXBlocks = InPyramidBand->GetXSize()/4000;//如果图像尺寸大于4000,则按照最大是4000的块进行读写
		}
		if (InPyramidBand->GetYSize() <= 4000)
		{
			nYBlocks = 1;
		}
		else
		{
			nYBlocks = InPyramidBand->GetYSize()/4000;//如果图像尺寸大于4000,则按照最大是4000的块进行读写

		}
		GDALRasterBand* OutBand = OutFile->GetRasterBand(i);
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
	
}

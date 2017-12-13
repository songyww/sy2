/************************************************************************/
/* 利用GDAL构建金字塔、读取金字塔某一层、保存某以波段等等与GDAL相关的函数 */
/************************************************************************/
#ifndef PYRAMID_WITH_GDAL_H
#define PYRAMID_WITH_GDAL_H

#include "ProcessBase.h"

class PyramidBase
{

public:
	//构造函数，对成员变量等进行初始化
	PyramidBase()
	{
		//为了支持中文路径，添加下面代码
		CPLSetConfigOption( "GDAL_FILENAME_IS_UTF8", "NO" );
		//注册栅格驱动
		GDALAllRegister();
	}

	//析构函数
	~PyramidBase()
	{
		GDALClose( InFile );
		GDALDestroyDriverManager();

	}

public:
	//下面几个变量是有关读入图像的信息
	
	/*文件路径*/
	const char *InFilePath;			//存储读入文件完整路径
	const char *FileName;			//存数读入文件的名字
	const char *FilePath;			//存储读入文件所在文件夹的路径
	const char* LayerPath ;				//存储金字塔层图像所在文件夹路径
	/*GDAL数据集*/
	GDALDataset *InFile;			//存储读入的文件数据
	int iBandCount,iOverViewCount;	//iBandCount存储波段数；iOverViewCount存储当前波段中金字塔层数
	int iWidth,iHeight;				//图像宽/高
	const char *InProjectionRef;	//图像投影信息
	double InGeotransform[6];		//图像变换参数

	GDALRasterBand *InRasterBand;	//图像第一个波段--用以获取金字塔层数及图像大小信息
	GDALRasterBand *InPyramidBand;	// 图像第一个波段的某一层金字塔图像；用以后面读取某一层时的参数设置；
	GDALDriverH hDriver ;			//获取图像驱动
	const char* InFormat;			//输入图像的格式
	
	//int iwidth;//保存图像width
	//int iheight;//保存图像height

	int iPixelNum, iTopNum, iCurNum;

	char * PyramidLayer_Path[20];///用于保存金字塔n层图像的路径，其中图像的名字最后一个数字代表第几层


	//该函数用于将文件数据读入数据集汇总，并检查文件是否存在金字塔
	bool CheckPyramid( );
	//对于小图像仍然要建立金字塔的情况，使用该函数
	bool sCheckPyramid();
	bool sCheckPyramid(int TopLayerSize);
	//该函数实现GDAL构建金字塔
	bool CreatePyramid();
	//对小图构建金字塔，相比上一函数只修改顶层金字塔大小
	bool sCreatePyramid();
	bool sCreatePyramid(int TopLayerSize);

	//定义用于读取金字塔某一层的函数，以及读取时的图像块的设置；m:将图像分m次；n表示读取第几层金字塔
	void ReadPyramidFile(int m, int n, char * filepath);
//	void sReadPyramidFile(int n, char * filepath);

	//保存单个波段的金字塔某一层的图像,m:第m个波段； n:第n层金字塔； filepath:保存金字塔图像路径
	void SaveSingleBand(int n, char *filepath);//, GDALDataset *pInFile );
	
	//保存图像金字塔中某一层所有波段的图像，
	void SaveAllBand(int n, char *filepath);

	//该函数实现a的b次方
	int p_pow(int a, int b);
//
};

















#endif

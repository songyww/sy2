/************************************************************************/
/* ����GDAL��������������ȡ������ĳһ�㡢����ĳ�Բ��εȵ���GDAL��صĺ��� */
/************************************************************************/
#ifndef PYRAMID_WITH_GDAL_H
#define PYRAMID_WITH_GDAL_H

#include "ProcessBase.h"

class PyramidBase
{

public:
	//���캯�����Գ�Ա�����Ƚ��г�ʼ��
	PyramidBase()
	{
		//Ϊ��֧������·��������������
		CPLSetConfigOption( "GDAL_FILENAME_IS_UTF8", "NO" );
		//ע��դ������
		GDALAllRegister();
	}

	//��������
	~PyramidBase()
	{
		GDALClose( InFile );
		GDALDestroyDriverManager();

	}

public:
	//���漸���������йض���ͼ�����Ϣ
	
	/*�ļ�·��*/
	const char *InFilePath;			//�洢�����ļ�����·��
	const char *FileName;			//���������ļ�������
	const char *FilePath;			//�洢�����ļ������ļ��е�·��
	const char* LayerPath ;				//�洢��������ͼ�������ļ���·��
	/*GDAL���ݼ�*/
	GDALDataset *InFile;			//�洢������ļ�����
	int iBandCount,iOverViewCount;	//iBandCount�洢��������iOverViewCount�洢��ǰ�����н���������
	int iWidth,iHeight;				//ͼ���/��
	const char *InProjectionRef;	//ͼ��ͶӰ��Ϣ
	double InGeotransform[6];		//ͼ��任����

	GDALRasterBand *InRasterBand;	//ͼ���һ������--���Ի�ȡ������������ͼ���С��Ϣ
	GDALRasterBand *InPyramidBand;	// ͼ���һ�����ε�ĳһ�������ͼ�����Ժ����ȡĳһ��ʱ�Ĳ������ã�
	GDALDriverH hDriver ;			//��ȡͼ������
	const char* InFormat;			//����ͼ��ĸ�ʽ
	
	//int iwidth;//����ͼ��width
	//int iheight;//����ͼ��height

	int iPixelNum, iTopNum, iCurNum;

	char * PyramidLayer_Path[20];///���ڱ��������n��ͼ���·��������ͼ����������һ�����ִ���ڼ���


	//�ú������ڽ��ļ����ݶ������ݼ����ܣ�������ļ��Ƿ���ڽ�����
	bool CheckPyramid( );


	//�ú���ʵ��GDAL����������
	bool CreatePyramid();


	//�������ڶ�ȡ������ĳһ��ĺ������Լ���ȡʱ��ͼ�������ã�m:��ͼ���m�Σ�n��ʾ��ȡ�ڼ��������
	void ReadPyramidFile(int m, int n, char * filepath);


	//���浥�����εĽ�����ĳһ���ͼ��,m:��m�����Σ� n:��n��������� filepath:���������ͼ��·��
	void SaveSingleBand(int n, char *filepath);//, GDALDataset *pInFile );


	//�ú���ʵ��a��b�η�
	int p_pow(int a, int b);
//
};

















#endif

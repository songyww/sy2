#include "sy_2.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	::CoInitialize(NULL);//初始化

	//绑定许可和初始化许可

#pragma region 绑定许可

	IArcGISVersionPtr ipVer(__uuidof(VersionManager));

	VARIANT_BOOL succeeded;

	if (FAILED(ipVer->LoadVersion(esriArcGISEngine , L"10.1",&succeeded)))

		return 0;

#pragma endregion

	//

#pragma region 初始化许可

	IAoInitializePtr ipInit(CLSID_AoInitialize);

	esriLicenseStatus status;

	ipInit->Initialize(esriLicenseProductCodeEngine, &status);

	if (status != esriLicenseCheckedOut)

	{

		AoExit(0);

		return 0;

	}



#pragma endregion

	esriLicenseExtensionCode ExtensionCode;
	esriLicenseStatus status1;
	ipInit->CheckOutExtension(esriLicenseExtensionCode3DAnalyst,&status1);


	///解决QT中中文乱码
	QTextCodec *codec = QTextCodec::codecForName("GB2312");
	QTextCodec::setCodecForLocale(codec);

	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);







	QApplication a(argc, argv);
	sy_2 w;
	w.show();
	return a.exec();
}

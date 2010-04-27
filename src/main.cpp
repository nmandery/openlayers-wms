#include "OpenlayersWMS.h"
#include <FastCgiQt/Manager.h>

// we need a GUI application to run webkit
#include <QApplication>

int main(int argc, char** argv)
{
	QApplication application(argc, argv);
	application.setApplicationName("OpenlayerWMS");

	FastCgiQt::Manager manager(&OpenlayersWMS::create);

	return application.exec();
}

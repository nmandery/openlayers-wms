#include "OpenlayersWMS.h"
#include <FastCgiQt/Manager.h>

// we need a GUI application to run webkit
#include <QApplication>

int main(int argc, char** argv)
{
	QApplication application(argc, argv);
	application.setApplicationName("OpenlayerWMS");

  QUrl url("http://localhost/");

	FastCgiQt::Manager manager;
  OpenlayersWMS responder(url);

  responder.connect(
      &manager,
      SIGNAL(newRequest(FastCgiQt::Request*)),
      SLOT(respond(FastCgiQt::Request*))
  );

  
	return application.exec();
}

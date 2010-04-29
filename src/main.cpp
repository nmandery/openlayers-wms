#include "Wms.h"
#include <FastCgiQt/Manager.h>

// we need a GUI application to run webkit
#include <QApplication>

void msgHandler(QtMsgType type, const char *msg)
{
   switch (type) {
   case QtDebugMsg:
       fprintf(stderr, "Debug: %s\n", msg);
       break;
   case QtWarningMsg:
       fprintf(stderr, "Warning: %s\n", msg);
       break;
   case QtCriticalMsg:
       fprintf(stderr, "Critical: %s\n", msg);
       break;
   case QtFatalMsg:
       fprintf(stderr, "Fatal: %s\n", msg);
       abort();
   }
}


int main(int argc, char** argv) {
  qInstallMsgHandler(msgHandler);
	QApplication application(argc, argv);
	application.setApplicationName("OpenlayerWMS");

  QUrl url("http://localhost/osm.html");

	FastCgiQt::Manager manager;
  Wms responder(url);

  responder.connect(
      &manager,
      SIGNAL(newRequest(FastCgiQt::Request*)),
      SLOT(respond(FastCgiQt::Request*))
  );

  
	return application.exec();
}



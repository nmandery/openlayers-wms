#include <FastCgiQt/Manager.h>
#include <QApplication> // we need a GUI application to run webkit

#include "Wms.h"

#define APP_NAME "OpenlayersWMS"

void msgHandler(QtMsgType type, const char *msg)
{
   switch (type) {
   case QtDebugMsg:
       fprintf(stderr, "%s Debug: %s\n", APP_NAME, msg);
       break;
   case QtWarningMsg:
       fprintf(stderr, "%s Warning: %s\n", APP_NAME, msg);
       break;
   case QtCriticalMsg:
       fprintf(stderr, "%s Critical: %s\n", APP_NAME, msg);
       break;
   case QtFatalMsg:
       fprintf(stderr, "%s Fatal: %s\n", APP_NAME, msg);
       abort();
   }
}


int main(int argc, char** argv) {
  qInstallMsgHandler(msgHandler);
	QApplication application(argc, argv);
	application.setApplicationName(APP_NAME);

  QString file("../html/unitr.html");

	FastCgiQt::Manager manager;
  Wms responder(file);

  responder.connect(
      &manager,
      SIGNAL(newRequest(FastCgiQt::Request*)),
      SLOT(respond(FastCgiQt::Request*))
  );

  
	return application.exec();
}



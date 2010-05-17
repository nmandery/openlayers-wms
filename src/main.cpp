#include <FastCgiQt/Manager.h>
#include <QApplication>

#include "Wms.h"

#define APP_NAME "OpenlayersWMS"
#define APP_VERSION "0.1"

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

  // this app has to be a QApplication (GUI-App)
  // to be able to run webkit
	QApplication application(argc, argv);

	application.setApplicationName(APP_NAME);
  application.setApplicationVersion(APP_VERSION);

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



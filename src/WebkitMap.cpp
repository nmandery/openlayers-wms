#include "WebkitMap.h"

WebkitMap::WebkitMap() : QWebPage() {

   // webkit settings
  settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
  settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
  settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
  
}


bool WebkitMap::resizeMap(const QSize &size) {
  // TODO: error handling
  
  mainFrame()->evaluateJavaScript(
    "mapapi.resize(" % 
      QString::number(size.width()) % ", " % 
      QString::number(size.height()) % ")");

  // TODO: wait till resize is finished
  return true;
}

bool WebkitMap::getProjection(QString &proj) {
  bool success = false;

  QVariant jsres = mainFrame()->evaluateJavaScript("mapapi.getProjectionCode();");
  if (!jsres.canConvert<QString>()) {
    qCritical() << "WebkitMap::getProjection() : cannot convert to string";  
  }
  else {
    proj = jsres.toString();
    success = true;
  }
  return success;
}


bool WebkitMap::getLayers(QList<Layer> &layers)  {

  QVariant jsres = mainFrame()->evaluateJavaScript("mapapi.getLayers();");
  qDebug() << jsres;

  return true;
}




void WebkitMap::javaScriptConsoleMessage(const QString &message, int lineNumber, const QString &sourceID) {
  QString fullMessage = 
    "JS Error in \"" % sourceID % 
    "\" (line " % QString::number(lineNumber) % "): " % message;

  //qCritical() << fullMessage; 

  emit errorMsg("jsError", fullMessage.toAscii().data());
}

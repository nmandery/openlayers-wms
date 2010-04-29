#include "WebkitMap.h"

WebkitMap::WebkitMap() : QWebPage() {

   // webkit settings
  settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
  settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
  settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
  
}

const QString WebkitMap::getProjection() {
  QString proj;

  QVariant jsres = mainFrame()->evaluateJavaScript("mapapi.getProjectionCode();");
  qDebug() << jsres;
  if (!jsres.canConvert<QString>()) {
    qCritical() << "WebkitMap::getProjection() : cannot convert to string";  
  }
  else {
    proj = jsres.toString();
  }
  return proj;
}


QList<Layer> WebkitMap::getLayers() {
  QList<Layer> layers;

  QVariant jsres = mainFrame()->evaluateJavaScript("mapapi.getLayers();");
  qDebug() << jsres;

  return layers;
}

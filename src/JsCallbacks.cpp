#include "JsCallbacks.h"

JsCallbacks::JsCallbacks(QObject* parent) 
    : QObject(parent) {
  
}

void JsCallbacks::done() {
  qDebug() << "JsCallbacks::done()";
  emit ready();  
}


const QString JsCallbacks::getVersion() {
  return QCoreApplication::applicationVersion();  
}

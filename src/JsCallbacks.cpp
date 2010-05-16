#include "JsCallbacks.h"

JsCallbacks::JsCallbacks(QObject* parent) 
    : QObject(parent) {
  
}

void JsCallbacks::done() {
  qDebug() << "JsCallbacks::done()";
  emit ready();  
}

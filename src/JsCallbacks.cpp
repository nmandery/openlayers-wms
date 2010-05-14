#include "JsCallbacks.h"

JsCallbacks::JsCallbacks(QObject* parent) 
    : QObject(parent) {
  
}

void JsCallbacks::done() {
  emit ready();  
}

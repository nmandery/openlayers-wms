#include "WebkitMap.h"

WebkitMap::WebkitMap() : QWebPage() {

   // webkit settings
  settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
  settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
  settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
  
}

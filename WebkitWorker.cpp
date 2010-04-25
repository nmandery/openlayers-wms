#include "WebkitWorker.h"

WebkitWorker::WebkitWorker() {

  // webkit settings
  page.settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
  page.settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
  page.settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);

  // connect signals
  // TODO
  
  // no scrollbars
  page.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  page.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

}


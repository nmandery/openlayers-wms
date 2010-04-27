#include "WebkitWorker.h"


WebkitWorker::WebkitWorker() {

  // connect signals
  connect(page.mainFrame(), SIGNAL(loadFinished(bool)), this, SLOT(setLoadingFinished(bool))); //qt4.6
  connect(page.mainFrame(), SIGNAL(loadStarted()), this, SLOT(setLoadingStarted())); // qt4.6
  
  // no scrollbars
  page.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  page.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

  timeout = 0;
}

const QString WebkitWorker::title() {
  return page.mainFrame()->title();  
}

void WebkitWorker::setLoadingFinished(bool success) {
  loading_finished = true;
  loading_success = success;  
}


void WebkitWorker::setLoadingStarted() {
  loading_finished = false;
}


void WebkitWorker::loadUrl(QUrl &url) {

  page.mainFrame()->load(url); 
  while (!loading_finished) {

    // TODO: handle timeouts  
    
    while (QApplication::instance()->hasPendingEvents()) {
      QApplication::instance()->processEvents();
    }
  }

  if (!loading_success) {
    throw(ERR_NO_LOAD_URL);   
  }

}
  
void WebkitWorker::render(QBuffer & target, const char * format) {

  page.setViewportSize(page.mainFrame()->contentsSize());

  QImage image(page.mainFrame()->contentsSize(), QImage::Format_ARGB32_Premultiplied);
  image.fill(Qt::transparent);

  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  page.mainFrame()->render(&painter);
  painter.end();

  image.save(&target, format);
}


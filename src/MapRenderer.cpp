#include "MapRenderer.h"


MapRenderer::MapRenderer() {

  // connect signals
  connect(map.mainFrame(), SIGNAL(loadFinished(bool)), this, SLOT(setLoadingFinished(bool))); //qt4.6
  connect(map.mainFrame(), SIGNAL(loadStarted()), this, SLOT(setLoadingStarted())); // qt4.6
  
  // no scrollbars
  map.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  map.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

  timeout = 0;
  loaded = false;  
}

const QString MapRenderer::title() {
  return map.mainFrame()->title();  
}

void MapRenderer::setLoadingFinished(bool success) {
  loading_finished = true;
  loaded = success;  
}


void MapRenderer::setLoadingStarted() {
  loading_finished = false;
}

bool MapRenderer::hasLoaded() {
  return loaded;
}

bool MapRenderer::loadUrl(QUrl &iurl) {
  url = iurl;
  return refresh();
}

bool MapRenderer::refresh() {
  map.mainFrame()->load(url); 
  while (!loading_finished) {

    // TODO: handle timeouts  
    
    QApplication::instance()->processEvents(QEventLoop::AllEvents, 10);
  }

  if (!loaded) {
    emit errorOccured("urlLoad", "Could not load the url ");
  }
  return loaded;
}
  
void MapRenderer::render(QBuffer & target, const char * format) {

  map.setViewportSize(map.mainFrame()->contentsSize());

  QImage image(map.mainFrame()->contentsSize(), QImage::Format_ARGB32_Premultiplied);
  image.fill(Qt::transparent);

  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  map.mainFrame()->render(&painter);
  painter.end();

  image.save(&target, format);
}



QList<QByteArray> MapRenderer::getImageFormats() {
  return QImageWriter::supportedImageFormats();
}

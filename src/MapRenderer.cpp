#include "MapRenderer.h"


MapRenderer::MapRenderer(QObject* parent) 
  : QObject(parent) {

  // no caching
  //QWebSettings::setMaximumPagesInCache(1);

  connect(map.mainFrame(), SIGNAL(loadFinished(bool)), 
      this, SLOT(setLoadingFinished(bool))); 
  connect(map.mainFrame(), SIGNAL(loadStarted()), 
      this, SLOT(setLoadingStarted())); 
  
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



bool MapRenderer::load(QString &ifile) {

  file = ifile;
  return refresh();

}



bool MapRenderer::refresh() {

  QFileInfo fi(file);
  if ( fi.exists() ) {
    map.mainFrame()->load(QUrl("file://" + fi.absoluteFilePath())); 
  }
  else {
    map.mainFrame()->load(QUrl(file)); 
  }

  while (!loading_finished) {

    // TODO: handle timeouts  
    
    QApplication::instance()->processEvents(QEventLoop::AllEvents, 10);
  }

  qDebug() << "finished loading page";

  if (!loaded) {
    emit errorMsg("urlLoad", "Could not load the url ");
  }
  return loaded;

}



bool MapRenderer::render(QBuffer & target, 
    const char *format, 
    const QSize &image_size) {

  qDebug() << "rendering in " << format << " and size " << image_size;

  if (image_size.isEmpty()) {
    emit errorMsg("renderMap", "Image size is empty");
    return false;  
  }

  map.setViewportSize(image_size);
  map.resizeMap(image_size);
  // TODO: wait till loaded

  QImage image(image_size, QImage::Format_ARGB32_Premultiplied);
  image.fill(Qt::transparent);

  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  map.mainFrame()->render(&painter);
  painter.end();

  image.save(&target, format);
  return true;

}



QList<QByteArray> MapRenderer::getImageFormats() {

  return QImageWriter::supportedImageFormats();

}

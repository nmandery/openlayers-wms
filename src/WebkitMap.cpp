#include "WebkitMap.h"

WebkitMap::WebkitMap(QObject* parent) : QWebPage(parent) {
  st_loading = false;
  st_jswaiting = false;

  jscallbacks = new JsCallbacks(this);

   // webkit settings
  settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
  settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
  settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
  

  connect(this, SIGNAL(loadFinished(bool)), 
      this, SLOT(loadingEnd(bool))); 
  connect(this, SIGNAL(loadStarted()), 
      this, SLOT(loadingStart())); 
  
  connect(mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
      this, SLOT(preparePage()));

  connect(jscallbacks, SIGNAL(ready()), 
      this, SLOT(jsEnd())); 
}


bool WebkitMap::resizeMap(const QSize &size) {
  // TODO: error handling
  
  mainFrame()->evaluateJavaScript(
    "map.doResize(" % 
      QString::number(size.width()) % ", " % 
      QString::number(size.height()) % ")");

  // TODO: wait till resize is finished
  return true;
}

void WebkitMap::loadProjection() {

  QVariant jsres = mainFrame()->evaluateJavaScript("map.getProjectionCode();");
  if (!jsres.canConvert<QString>()) {
    qCritical() << "WebkitMap::getProjection() : cannot convert to string";  
  }
  else {
    projection = jsres.toString();
  }
}

const QString WebkitMap::getProjection() { 
  return projection;  
}

void WebkitMap::loadLayerList() {

  QVariant jsres = mainFrame()->evaluateJavaScript("map.getLayerList();");
  qDebug() << jsres;
  if (!jsres.canConvert<QVariantMap>()) {

    qCritical() << "WebkitMap::getLayerList) : cannot convert to QVariantMap";  

  }
  else {
      QVariantMap lmap = jsres.toMap();
      
      for (QVariantMap::ConstIterator layer_variant = lmap.constBegin();
                layer_variant != lmap.constEnd();
                ++layer_variant) {
        Layer* layer = new Layer(this); 

        qDebug() << "Found layer named " << layer_variant.key();
        layer->name = layer_variant.key();

        if (layer_variant.value().canConvert<QVariantMap>()) {

          QVariantMap layer_props = layer_variant.value().toMap();

          // get title of the layer
          if (layer_props.contains(QString("title"))) {
            layer->title = layer_props.value(QString("title")).toString();
            qDebug() << layer_props.value(QString("title"));
          }

          // get bbox of the layer
          if (layer_props.contains(QString("extent"))) {
            QVariantList bbox_variants = layer_props.value(QString("extent")).toList();
            if (bbox_variants.size() == 4) {

              // TODO: check coordinates
              layer->bbox.left   = bbox_variants.at(0).toDouble();
              layer->bbox.top    = bbox_variants.at(1).toDouble();
              layer->bbox.right  = bbox_variants.at(2).toDouble();
              layer->bbox.bottom = bbox_variants.at(3).toDouble();

            }
            else {
              qWarning()  << "Can not convert the BBox of layer " 
                          << layer->name
                          << ". It contains " 
                          << bbox_variants.size() 
                          << " elements instead of 4.";
            }
          }
          else {
            qWarning() << "Layer " << layer->name 
                      << " has no BBox defined";  
          }
        }
        else {
          qWarning()  << "Can not read properties of layer " 
                      << layer->name 
                      << ". Ignoring this one";
        }

        // add to layerlist
        layers.append(layer);
      }
  }
 
}


LayerList WebkitMap::getLayerList( )  {
  return layers;
}


bool WebkitMap::hasLayer(const QString &layername) {

  int exists = false;

  Layer* layer;
  foreach(layer, layers) {
    if (layer->name == layername) {
      exists = true;
      break;
    } 
  }

  return exists;
}


void WebkitMap::setVisibleLayers(const QStringList &layernames) {

  mainFrame()->evaluateJavaScript(
    "map.setVisibleLayers([\"" % layernames.join("\",\"") 
        % "\"])");
 
}


void WebkitMap::javaScriptConsoleMessage(
      const QString &message, 
      int lineNumber, 
      const QString &sourceID) {

  QString fullMessage = 
    "JS Error in \"" % sourceID % 
    "\" (line " % QString::number(lineNumber) % "): " % message;

  //qCritical() << fullMessage; 

  emit errorMsg("jsError", fullMessage.toAscii().data());
}


bool WebkitMap::isReady() {

  return (!st_loading && !st_jswaiting);

}


void WebkitMap::loadingStart() {

  st_loading = true;

}

void WebkitMap::loadingEnd(bool ok) {

  st_loading = false;

  if (ok) {
    loadProjection();
    loadLayerList();
  } 

  if (isReady()) {
    emit ready();
  }
}


void WebkitMap::jsStart() {

  st_jswaiting = true;

}



void WebkitMap::jsEnd() {

  st_jswaiting = false;  

  if (isReady()) {
    emit ready();
  }

}


void WebkitMap::preparePage() {

  // register the callbacks in the page
  mainFrame()->addToJavaScriptWindowObject(
      QCoreApplication::applicationName(), 
      jscallbacks);    

}

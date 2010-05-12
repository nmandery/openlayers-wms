#include "Wms.h"

#include <QTextStream>



Wms::Wms(QString& file, QObject * parent)
  : QObject(parent) {
  
  qDebug("initializing OpenlayerWMS instance");


  // return error messages as service exceptions
  connect(&renderer.map, SIGNAL(errorMsg(const char *, const char *)), 
    this, SLOT(serviceException(const char *, const char *))); 
  connect(&renderer, SIGNAL(errorMsg(const char *, const char *)), 
    this, SLOT(serviceException(const char *, const char *))); 

  // log
  connect(&renderer.map, SIGNAL(errorMsg(const char *, const char *)), 
    this, SLOT(logMessage(const char *, const char *))); 
  connect(&renderer, SIGNAL(errorMsg(const char *, const char *)), 
    this, SLOT(logMessage(const char *, const char *))); 

  renderer.load(file);  
}


void Wms::respond(FastCgiQt::Request* request) {

  m_request = request;

  // try to reload the page if it failed previously
  if (!renderer.hasLoaded()) {
    renderer.refresh();
  }

  QString p_service;
  QString p_request;
  QString p_version;
  QString p_format;
  QString p_layers;
  QString p_srs;
  QString p_bbox;
  int p_width = 0;
  int p_height = 0;

  // get all variables from the query string
  QHash<QByteArray, QByteArray> p_getVars = request->rawValues(FastCgiQt::GetData);
  for ( QHash<QByteArray, QByteArray>::ConstIterator it = p_getVars.constBegin();
    it != p_getVars.constEnd();
    ++it) {

    QString key = QString(it.key().toUpper());
      
    if (key == "SERVICE") {
      p_service = it.value().toUpper();
    }
    else if (key == "REQUEST") {
      p_request = it.value().toUpper();
    }
    else if (key == "VERSION") {
      p_version = it.value();
    }
    else if (key == "FORMAT") {
      p_format = it.value().toLower();
    }
    else if (key == "LAYERS") {
      p_layers = it.value();
    }
    else if (key == "WIDTH") {
      bool conv_result;
      p_width = it.value().toInt(&conv_result);
      if (!conv_result) {
        serviceException(
         "paramError",
         "Illegal value for WIDTH"
         );  
        return; 
      }
    }
    else if (key == "HEIGHT") {
      bool conv_result;
      p_height = it.value().toInt(&conv_result);
      if (!conv_result) {
        serviceException(
         "paramError",
         "Illegal value for HEIGHT"
         );  
        return; 
      }
    }
    else if (key == "BBOX") {
    }
  }

  if (p_service != "WMS") {

    serviceException(
      "unknownService",
      "This is a WMS (more or less). try SERVICE=WMS"
      );  
    return;
  }

  // dispatch requests
  if (p_request == "GETMAP") {
    QSize image_size(p_width, p_height); 
    getMap(p_format, p_layers, image_size);
  }

  else if (p_request == "GETCAPABILITIES") {
    getCapabilities();
  }

  else {
    serviceException(
      "unknownRequest", 
      "Unknown/missing REQUEST parameter"
      );  
    return;
  }
}


void Wms::getCapabilities()
{
  QByteArray data;
  QTextStream out(&data);
  QString wmsurl = m_request->url(FastCgiQt::LocationUrl).toEncoded(); 

  QString proj;
  if (!renderer.map.getProjection(proj)) {
    return;  
  }
 
  LayerList layers = renderer.map.getLayerList();

  //out << XML_HEADER << endl;
  out << "<WMT_MS_Capabilities version=\"1.1.1\">";

  // service info

  out << "<Service>"
      << "<Name>OGC:WMS</Name>"
      << "<Title>" << renderer.title() << "</Title>"
      << "<Abstract/>"
      << "<OnlineResource " << XML_XLINK_NS << " xlink:href=\"" << wmsurl << "\"/>"
      << "<ContactInformation/>"
      << "<Fees/>"
      << "<AccessConstraints/>"
      << "<KeywordList>";
  
  out << "<Keyword>" << "</Keyword>"; // TODO

  out << "</KeywordList>"
      << "</Service>";

  out << "<Capability>" 
      << "<Request>" 
      << "<GetCapabilities>" 
      << "<Format>" << MIMETYPE_GC << "</Format>" 
      << "<DCPType><HTTP><Get>"
      << "<OnlineResource " << XML_XLINK_NS
      <<   " xlink:href=\"" << wmsurl << "?Service=WMS&amp;"
      <<   "\"/></Get></HTTP></DCPType>"
      << "</GetCapabilities>" 
      << "<GetMap>";

  // image formats
  QList<QByteArray> formats = renderer.getImageFormats();
  for (QList<QByteArray>::ConstIterator fit = formats.constBegin();
    fit != formats.constEnd();
    ++fit) {
    QByteArray fb = *fit;
    QString outf;

    // ony return a few formats we want to support
    if (fb == "png") {
      outf = MIMETYPE_PNG;
    }
    else if (fb == "jpg") {
      outf = MIMETYPE_JPG;
    }
    else if (fb == "tiff") {
      outf = MIMETYPE_TIF;
    }

    if (!outf.isEmpty()) {
      out << "<Format>" << QString(outf) << "</Format>";  
    }
  }
  
  out << "<DCPType><HTTP><Get>" 
      << "<OnlineResource " << XML_XLINK_NS
      <<  " xlink:href=\"" << wmsurl << "?SERVICE=WMS&amp;\"/>"
      << "</Get></HTTP></DCPType>"
      << "</GetMap>"
      << "</Request>"; 

  out << "<Exception><Format>" << MIMETYPE_SE << "</Format></Exception>";

  out << "<Layer>" 
      << "<Title>" << renderer.title() << "</Title>" 
      << "<SRS>" << proj << "</SRS>"; 
      //<LatLonBoundingBox minx="-179.992" miny="-90.008" maxx="180.008" maxy="89.992"/> 
      //
   // layers

/*  for (LayerList::ConstIterator layer_it = layers.constBegin();
              layer_it != layers.constEnd();
              ++layer_it) {
    Layer layer = **layer_it;
*/
    while (!layers.isEmpty()) {
      Layer *layer = layers.takeFirst();

    out << "<Layer queryable=\"0\">" 
        << "<Name>" << layer->name << "</Name>"
        << "<Title>" << layer->title << "</Title>"
        << "<SRS>" << proj << "</SRS>";

    
    out << "<LatLonBoundingBox minx=\"" << layer->bbox.left << "\" miny=\""
        << layer->bbox.bottom << "\" maxx=\"" << layer->bbox.right 
        << "\" maxy=\"" << layer->bbox.top << "\"/>" 
        << "<BoundingBox SRS=\"" << proj << "\" minx=\"" << layer->bbox.left 
        << "\" miny=\"" << layer->bbox.bottom << "\" maxx=\"" 
        << layer->bbox.right << "\" maxy=\"" << layer->bbox.top << "\"/>"; 

    out << "</Layer>";

    delete layer;
  }

  out << "</Layer>"
      << "</Capability>"
      << "</WMT_MS_Capabilities>";

  out.flush();

  m_request->setHeader(HTTP_CONTENT_TYPE, MIMETYPE_GC);
  QByteArray content_length = QByteArray::number(data.length());
  m_request->setHeader(HTTP_CONTENT_LEN, content_length );
  m_request->write(data);
}


void Wms::serviceException( const char *msgCode, const char *msgText, QtMsgType type)
{
  // only output a SE if there is an request
  if (!m_request) { return;} // TODO: this crashes - -check is not sufficent

  QByteArray data;
  QTextStream out(&data);

  //out << XML_HEADER << endl;
  out << "<ServiceExceptionReport version=\"1.1.1\">"
      << "<ServiceException code=\"" << msgCode << "\">\n"
      << msgText << "\n"
      << "</ServiceException>"
      << "</ServiceExceptionReport>"
      << endl;

  out.flush();

  m_request->setHeader(HTTP_CONTENT_TYPE, MIMETYPE_SE);
  QByteArray content_length = QByteArray::number(data.length());
  m_request->setHeader(HTTP_CONTENT_LEN, content_length );
  m_request->write(data);

}

void Wms::logMessage( const char *msgCode, const char *msgText, QtMsgType type) {
  switch(type) {
    case QtDebugMsg:
      qDebug() << msgText;
      break;
    case QtCriticalMsg:
      qCritical() << msgText;
      break;
    case QtFatalMsg:
      qCritical() << msgText;
      break;
    case QtWarningMsg:
      qWarning() << msgText;
      break;
  }
}

void Wms::getMap( const QString &image_format, const QString &layers, const QSize &image_size) {

  // check the validity of the input parameters
  
  // layers
  if (layers.isEmpty()) {
    serviceException(
      "noLayers",
      "no layers specified");
    return;
  }
  QStringList layer_names = layers.split(",");
  QStringList layer_names_cleaned;
  for (int i=0; i<layer_names.size(); ++i) {
    QString layer_name = layer_names.at(i).trimmed();
    
    if (!layer_name.isEmpty()) {
      if (renderer.map.hasLayer(layer_name)) {
        layer_names_cleaned.append(layer_name);  
      }
      else {
        serviceException(
          "layerDoesNotExist",
          "one of the layers does not exist");
        return;       
      }
    }
  }

  // format
  QByteArray renderformat;
  QList<QByteArray> supported_formats = renderer.getImageFormats();
  if (image_format == MIMETYPE_PNG) {
    renderformat = "png";  
  }
  else if (image_format == MIMETYPE_JPG) {
    renderformat = "jpeg";  
  }
  else if (image_format == MIMETYPE_TIF) {
    renderformat = "tiff";  
  };

  if (renderformat.isEmpty() || !supported_formats.contains(renderformat.data())) {
    serviceException(
      "formatNotsupported",
      "the imageformat is not supported");
    return;
  }


  // render
  QByteArray bytes;
  QBuffer buffer(&bytes); // write binary data
  buffer.open(QIODevice::WriteOnly);

  if (renderer.render( buffer, renderformat.toUpper(), image_size)) {

    m_request->setHeader(HTTP_CONTENT_TYPE, image_format.toUtf8());
    QByteArray content_length = QByteArray::number(bytes.length());
    m_request->setHeader(HTTP_CONTENT_LEN, content_length );
    m_request->write(bytes);
  }
  else {
    serviceException(
      "renderError",
      "could not render the image - this should realy not happen, sorry");
    return;
  }
}

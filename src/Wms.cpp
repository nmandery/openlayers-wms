#include "Wms.h"

#include <QTextStream>



Wms::Wms(QUrl& url) {
  
  qDebug("initializing OpenlayerWMS instance");
  renderer.loadUrl(url);  


  // return error messages as service exceptions
  //connect(renderer.map, SIGNAL(errorOccured(const char *, const char *)), 
  //  this, SLOT(serviceException(const char *, const char *))); 
}


void Wms::respond(FastCgiQt::Request* request) {

  m_request = request;

  QString p_service;
  QString p_request;
  QString p_version;
  QString p_format;
  QString p_srs;
  QString p_bbox;
  QString p_width;
  QString p_height;

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
      p_format = it.value();
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
    getMap();
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
  
  //out << XML_HEADER << endl;
  out << "<WMT_MS_Capabilities version=\"1.1.1\">" << endl;

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
      << "</Service>"
      << endl;
  out << renderer.map.getProjection() << endl;

  out << "<Capability>" 
      << "<Request>" 
      // GetCapabilities
      << "<GetCapabilities>" 
      << "<Format>" << MIMETYPE_GC << "</Format>" 
      << "<DCPType><HTTP><Get>"
      << "<OnlineResource " << XML_XLINK_NS
      <<   " xlink:href=\"" << wmsurl << "?Service=WMS"
      <<   "\"/></Get></HTTP></DCPType>"
      << "</GetCapabilities>" 
      // GetMap
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
      << "<SRS>" << renderer.map.getProjection() << "</SRS>"; 
      //<LatLonBoundingBox minx="-179.992" miny="-90.008" maxx="180.008" maxy="89.992"/> 
   // layers
   /*
      <Layer queryable="1"> 
        <Name>ne2:10m_admin0_boundary_lines_land</Name> 
        <Title>NE2 Admin 0 Boundary Lines Land (1:10000000)</Title> 
        <SRS>EPSG:4326</SRS> 
        <LatLonBoundingBox minx="-141.006" miny="-55.121" maxx="140.978" maxy="70.075"/> 
        <BoundingBox SRS="EPSG:4326" minx="-141.006" miny="-55.121" maxx="140.978" maxy="70.075"/> 
      </Layer> 
  */
  out << "</Layer>"
      << "</Capability>"
      << "</WMT_MS_Capabilities>" << endl;

  out.flush();

  m_request->setHeader(HTTP_CONTENT_TYPE, MIMETYPE_GC);
  QByteArray content_length = QByteArray::number(data.length());
  m_request->setHeader(HTTP_CONTENT_LEN, content_length );
  m_request->write(data);
}


void Wms::serviceException( const char *msgCode, const char *msgText)
{
  
  QByteArray data;
  QTextStream out(&data);

  out << XML_HEADER << endl;
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


void Wms::getMap() {
  try {

    QByteArray bytes;
    QBuffer buffer(&bytes); // write binary data
    buffer.open(QIODevice::WriteOnly);

    renderer.render( buffer, "PNG");

    m_request->setHeader(HTTP_CONTENT_TYPE, MIMETYPE_PNG);
    QByteArray content_length = QByteArray::number(bytes.length());
    m_request->setHeader(HTTP_CONTENT_LEN, content_length );
    m_request->write(bytes);

  }
  catch(int e) {

    switch (e) {
      case ERR_NO_LOAD_URL:
        serviceException(
          "noLoad", 
          "Could not load the HTML file."
        );  
        return;        
        break;
      default:
        serviceException(
          "unknownError", 
          "A unknown Error occured - great error message, isn't it?"
        );  
        return;           
      }
  }

}

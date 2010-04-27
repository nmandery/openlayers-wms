#include "OpenlayersWMS.h"


void OpenlayersWMS::initialize() {
  if (initialized != INIT_DONE) {

    qDebug("initializing OpenlayerWMS instance");

    QUrl url = QUrl("http://localhost/");
    worker.loadUrl(url);  

    initialized = INIT_DONE;
  }  
}


void OpenlayersWMS::respond() {

  // initialize on first request
  initialize();

  if (request.isValid()) {
    QString p_service;
    QString p_request;
    QString p_version;

    // get all variables from the query string
    QHash<QString, QString> p_getVars = request.getData();
    for ( QHash<QString, QString>::ConstIterator it = p_getVars.constBegin();
      it != p_getVars.constEnd();
      ++it) {

      QString key = it.key().toUpper();

      if (key == "SERVICE") {
        p_service = it.value().toUpper();
      }
      else if (key == "REQUEST") {
        p_request = it.value().toUpper();
      }
      else if (key == "VERSION") {
        p_version = it.value();
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
}


void OpenlayersWMS::getCapabilities()
{
  QString onlineResource = "http://example.com/df";

  FastCgiQt::ClientIOInterface::setHeader("Content-Type", MIMETYPE_XML);
  
  out << XML_HEADER;

  out << "<WMT_MS_Capabilities version=\"1.1.1\">";

  out << "<Service>"
      << "<Name>OGC:WMS</Name>"
      << "<Title>XXXX</Title>"
      << "<OnlineResource xlink:href=\"cccccs\"/>"
      << "</Service>";

  out << "</WMT_MS_Capabilities>";
}


void OpenlayersWMS::serviceException( const char *msgCode, const char *msgText)
{
  
  FastCgiQt::ClientIOInterface::setHeader("Content-Type", MIMETYPE_SE);

  out << XML_HEADER ;
  out << "<ServiceExceptionReport version=\"1.1.1\">"
      << "<ServiceException code=\"" << msgCode << "\">\n"
      << msgText << "\n"
      << "</ServiceException>"
      << "</ServiceExceptionReport>";
}


void OpenlayersWMS::getMap() {
  try {
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);

    worker.render( buffer, "PNG");

    FastCgiQt::ClientIOInterface::setHeader("Content-Type", MIMETYPE_PNG);
    // write binary data
    out.device()->write(bytes);
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

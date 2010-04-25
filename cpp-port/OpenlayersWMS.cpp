#include "OpenlayersWMS.h"

void OpenlayersWMS::respond()
{
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
      QString msgCode = QString("unknownService");
      QString msgText = QString("This is a WMS (more or less). try SERVICE=WMS");
      serviceException(msgCode, msgText);  
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
      QString msgCode = QString("unknownRequest");
      QString msgText = QString("Unknown/missing REQUEST parameter");
      serviceException(msgCode, msgText);  
      return;
    }
  }
}


void OpenlayersWMS::getCapabilities()
{
  QString onlineResource = "http://example.com/df";

  FastCgiQt::ClientIOInterface::setHeader("Content-Type","application/xml");
  
  out << XML_HEADER;

  out << "<WMT_MS_Capabilities version=\"1.1.1\">";

  out << "<Service>"
      << "<Name>OGC:WMS</Name>"
      << "<Title>XXXX</Title>"
      << "<OnlineResource xlink:href=\"cccccs\"/>"
      << "</Service>";

  out << "</WMT_MS_Capabilities>";
}

void OpenlayersWMS::serviceException(QString &msgCode, QString &msgText)
{
  
  FastCgiQt::ClientIOInterface::setHeader("Content-Type","application/vnd.ogc.se+xml");

  out << XML_HEADER ;
  out << "<ServiceExceptionReport version=\"1.1.1\">"
      << "<ServiceException code=\"" << msgCode << "\">\n"
      << msgText << "\n"
      << "</ServiceException>"
      << "</ServiceExceptionReport>";
}

void OpenlayersWMS::getMap() {
  out << "getmap";

}

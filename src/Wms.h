#ifndef CLASS_WMS_H
#define CLASS_WMS_H


#include <FastCgiQt/Request.h>
#include "MapRenderer.h"
#include "Layer.h"
#include "BoundingBox.h"


#define XML_HEADER "<?xml version='1.0' encoding=\"UTF-8\" standalone=\"no\" ?>"
#define XML_XLINK_NS "xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\"" 

#define INIT_DONE 12

// mimetypes
#define MIMETYPE_XML "application/xml"
#define MIMETYPE_SE  "application/vnd.ogc.se+xml"
#define MIMETYPE_GC  "application/vnd.ogc.wms_xml"
#define MIMETYPE_PNG "image/png"
#define MIMETYPE_JPG "image/jpeg"
#define MIMETYPE_TIF "image/tiff"

// http headers
#define HTTP_CONTENT_TYPE "Content-Type"
#define HTTP_CONTENT_LEN "Content-Length"


class Wms : public QObject {
	Q_OBJECT;

  public:
    Wms(QString&, QObject* parent = 0);

  public slots:
    /**
     * respond to an incoming http request
     *
     * this method does the request dispatching and forwards
     * requests to the corresponding methods
     *   - getMap
     *   - getCapabilites
     *   - serviceException
     */
    void respond(FastCgiQt::Request*);

  protected slots:
    /// return a service exception to the client
		void serviceException(const char* msgCode, 
              const char* msgText, 
              QtMsgType type = QtCriticalMsg);

    /// log errors
		void logMessage(const char* msgCode, 
              const char* msgText, 
              QtMsgType type = QtCriticalMsg);

  private:
    /**
     * return a GetCapabilities response 
     * gets called from respond 
     */
		void getCapabilities();

    /// render a map
		void getMap(const QString &image_format, 
              const QString &layer,
              const QSize &image_size,
              const BoundingBox &bbox);

    MapRenderer renderer;

    /// the request object of the current request
    FastCgiQt::Request* m_request;

};

#endif //CLASS_WMS_H

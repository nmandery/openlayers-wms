#include <FastCgiQt/Request.h>
#include "MapRenderer.h"
#include "Layer.h"


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
    Wms(QString&);

  public slots:
    void respond(FastCgiQt::Request*);

  protected slots:
		void serviceException(const char* msgCode, 
              const char* msgText, 
              QtMsgType type = QtCriticalMsg);
		void logMessage(const char* msgCode, 
              const char* msgText, 
              QtMsgType type = QtCriticalMsg);

  private:
		void getCapabilities();
		void getMap(const QString &image_format, 
              const QString &layer,
              const QSize &image_size);
    MapRenderer renderer;
    FastCgiQt::Request* m_request;

};

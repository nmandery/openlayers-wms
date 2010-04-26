#include <FastCgiQt/Responder.h>
#include "Common.h"
#include "WebkitWorker.h"


#define XML_HEADER "<?xml version='1.0' encoding=\"UTF-8\" standalone=\"no\" ?>\n"

#define INIT_DONE 12

// mimetypes
#define MIMETYPE_XML "application/xml"
#define MIMETYPE_SE  "application/vnd.ogc.se+xml"
#define MIMETYPE_PNG "image/png"
#define MIMETYPE_JPG "image/jpeg"


class OpenlayersWMS : public FastCgiQt::Responder {
	Q_OBJECT;
	FASTCGIQT_RESPONDER_API_V1;
	RESPONDER(OpenlayersWMS);
  private:
		void getCapabilities();
		void getMap();
		void serviceException(const char* msgCode, const char* msgText);
    WebkitWorker worker;
    int initialized;
    void initialize();
	public:
		void respond();

};

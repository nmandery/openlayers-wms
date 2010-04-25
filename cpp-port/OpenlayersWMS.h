#include <FastCgiQt/Responder.h>

// common constants
#define XML_HEADER "<?xml version='1.0' encoding=\"UTF-8\" standalone=\"no\" ?>\n"

class OpenlayersWMS : public FastCgiQt::Responder
{
	Q_OBJECT;
	FASTCGIQT_RESPONDER_API_V1;
	RESPONDER(OpenlayersWMS);
  private:
		void getCapabilities();
		void getMap();
		void serviceException(QString & msgCode, QString & msgText);
	public:
		void respond();

};

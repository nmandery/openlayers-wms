#include <FastCgiQt/Responder.h>
#include "Common.h"


class OpenlayersWMS : public FastCgiQt::Responder {
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

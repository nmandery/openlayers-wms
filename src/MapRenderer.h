#include <QtGui/QtGui>

#include "WebkitMap.h"


#define ERR_NO_LOAD_URL 1

class MapRenderer : public QObject {
  Q_OBJECT;
  
  public:
    //MapRenderer(QObject *parent = 0) { /* TODO*/ };
    MapRenderer();
    WebkitMap map;
    void loadUrl(QUrl & url);
    void setTimeout(int seconds); 
    int getTimeout();
    void render(QBuffer & target, const char * format);
    const QString title();
    QList<QByteArray> getImageFormats();

  public slots:
    void setLoadingFinished(bool);
    void setLoadingStarted();

  private:
    int timeout;
    bool loading_finished;
    bool loading_success;
};


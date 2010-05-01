#include <QtGui/QtGui>

#include "WebkitMap.h"


#define ERR_NO_LOAD_URL 1

class MapRenderer : public QObject {
  Q_OBJECT;
  
  public:
    //MapRenderer(QObject *parent = 0) { /* TODO*/ };
    MapRenderer();
    WebkitMap map;
    bool loadUrl(QUrl & url);
    bool refresh();
    void setTimeout(int seconds); 
    int getTimeout();
    void render(QBuffer & target, const char * format);
    const QString title();
    QList<QByteArray> getImageFormats();
    bool hasLoaded();

  signals:
    void errorOccured(const char *, const char *);

  public slots:
    void setLoadingFinished(bool);
    void setLoadingStarted();

  private:
    int timeout;
    bool loading_finished;
    bool loaded;
    QUrl url;
};


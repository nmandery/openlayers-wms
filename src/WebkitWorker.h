#include <QtGui/QtGui>

#include "WebkitMap.h"


#define ERR_NO_LOAD_URL 1

class WebkitWorker : public QObject {
  Q_OBJECT;
  
  public:
    //WebkitWorker(QObject *parent = 0) { /* TODO*/ };
    WebkitWorker();
    void loadUrl(QUrl & url);
    void setTimeout(int seconds); 
    int getTimeout();
    void render(QBuffer & target, const char * format);
    const QString title();

  public slots:
    void setLoadingFinished(bool);
    void setLoadingStarted();

  private:
    WebkitMap page;
    int timeout;
    bool loading_finished;
    bool loading_success;
};


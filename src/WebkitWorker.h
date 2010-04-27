#include <QtGui/QtGui>
#include <QtWebKit/QtWebKit>

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

  public slots:
    void setLoadingFinished(bool);
    void setLoadingStarted();

  private:
    QWebPage page;
    //QWebView * view;
    //QMainWindow * window;
    int timeout;
    bool loading_finished;
    bool loading_success;

};


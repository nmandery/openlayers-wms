#include <QtGui/QtGui>
#include <QtWebKit/QtWebKit>

class WebkitWorker : public QObject {
  Q_OBJECT;
  
  public:
    //WebkitWorker(QObject *parent = 0) { /* TODO*/ };
    WebkitWorker();

  private:
    QWebPage page;
    //QMainWindow window;

};


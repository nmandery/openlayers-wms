#include <QtWebKit/QtWebKit>
#include <QStringBuilder>
#include "Layer.h"


class WebkitMap : public QWebPage {
  Q_OBJECT;  

  public:
    WebkitMap();
    bool resizeMap(const QSize &size);
    bool getProjection(QString &proj);    
    bool hasLayer(const QString &layername);

    bool getLayerList(QList<Layer> &layers);
    /*
    // info from metatags
    const QString getDescription();
    const QString getAuthor();
    const QSet<QString> getKeywords();
    */

  signals:
    void errorMsg(const char * msgCode ,const char * msgText);

  protected:
    void javaScriptConsoleMessage(const QString &message,
                int lineNumber, 
                const QString &sourceID);
};

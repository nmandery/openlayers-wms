#include <QtWebKit/QtWebKit>
#include "Layer.h"


class WebkitMap : public QWebPage {
  Q_OBJECT;  

  public:
    WebkitMap();
    const QString getProjection();    

    QList<Layer> getLayers();
    /*
    // info from metatags
    const QString getDescription();
    const QString getAuthor();
    const QSet<QString> getKeywords();
    */

    /*
    // info collected by javascript calls
    const QSet<Layer> getLayers();
    */

  protected:
    void javaScriptConsoleMessage(const QString &message, int lineNumber, const QString &sourceID);
};

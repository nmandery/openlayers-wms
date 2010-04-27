#include <QtWebKit/QtWebKit>


class WebkitMap : public QWebPage {
  Q_OBJECT;  

  public:
    WebkitMap();

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
};

#ifndef CLASS_WEBKITMAP_H
#define CLASS_WEBKITMAP_H


#include <QtWebKit/QtWebKit>
#include <QStringBuilder>
#include "Layer.h"

class WebkitMap : public QWebPage {
  Q_OBJECT;  

  public:
    WebkitMap();

    /// resize to map and the Javascript map to the given size
    bool resizeMap(const QSize &size);

    /// get the Projection of the map in epsg format. (i.e. "EPSG:4326")
    bool getProjection(QString &proj);    

    /// check if the layer with the given name exists 
    bool hasLayer(const QString &layername);

    /// get a list of all layers in the map
    LayerList getLayerList();

    /*
    // info from metatags
    const QString getDescription();
    const QString getAuthor();
    const QSet<QString> getKeywords();
    */

  signals:
    /**
     * this signal get called when something goes wrong.
     * i.e. when getProjection returns false, this signal
     * emits the error message
     */
    void errorMsg(const char * msgCode ,const char * msgText);

  protected slots:
    /**
     * load the list of layers from the javascript map
     * 
     * if ok is not true, nothing will be done.
     */
    void loadLayerList(bool ok = true);

  protected:
    LayerList layers;

    /// catch JS error from the QWebPage class
    void javaScriptConsoleMessage(const QString &message,
                int lineNumber, 
                const QString &sourceID);
};

#endif

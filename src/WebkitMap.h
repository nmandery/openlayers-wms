#ifndef CLASS_WEBKITMAP_H
#define CLASS_WEBKITMAP_H


#include <QtWebKit/QtWebKit>
#include <QStringBuilder>
#include "Layer.h"
#include "JsCallbacks.h"

class WebkitMap : public QWebPage {
  Q_OBJECT;  

  public:
    WebkitMap(QObject* parent = 0);

    /// resize to map and the Javascript map to the given size
    bool resizeMap(const QSize &size);

    /// get the Projection of the map in epsg format.
    //(i.e. "EPSG:4326")
    const QString getProjection();    

    /// check if the layer with the given name exists 
    bool hasLayer(const QString &layername);

    /// get a list of all layers in the map
    LayerList getLayerList();

    ///
    void setVisibleLayers(const QStringList &layernames);

    bool isReady();

  signals:
    /**
     * this signal get called when something goes wrong.
     * i.e. when getProjection returns false, this signal
     * emits the error message
     */
    void errorMsg(const char * msgCode ,const char * msgText);
    
    void ready();


  protected slots:
    void loadingStart();
    void loadingEnd(bool ok = true);
    void preparePage();
    void jsEnd();

  protected:
    bool st_loading;
    bool st_jswaiting;
    LayerList layers;
    QString projection;
    JsCallbacks* jscallbacks;

    /**
     * load the list of layers from the javascript map
     * 
     * if ok is not true, nothing will be done.
     */
    void loadLayerList();

    void loadProjection();

    void jsStart();


    /// catch JS error from the QWebPage class
    void javaScriptConsoleMessage(const QString &message,
                int lineNumber, 
                const QString &sourceID);


    /// ignore js alerts
    void javaScriptAlert( QWebFrame * frame, const QString & msg) {};

    /// ignore js dialogs
    /// confirm everything with "yes"
    bool javaScriptConfirm(QWebFrame * frame, const QString & msg) { return true; };
};

#endif

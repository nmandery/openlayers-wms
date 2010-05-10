#ifndef CLASS_MAPRENDERER_H
#define CLASS_MAPRENDERER_H

#include <QtGui/QtGui>
#include "WebkitMap.h"


class MapRenderer : public QObject {
  Q_OBJECT;
  
  public:
    //MapRenderer(QObject *parent = 0) { /* TODO*/ };
    MapRenderer();

    /// the WebkitMap instance
    WebkitMap map;

    bool load(QString &file);
    bool refresh();
    void setTimeout(int seconds); 
    int getTimeout();

    bool render(QBuffer & target,
                const char *format, 
                const QSize &image_size);

    /// get the title of the map
    const QString title();

    /// the list of supported image formats
    QList<QByteArray> getImageFormats();

    bool hasLoaded();

  signals:
    void errorMsg(const char *, const char *);

  public slots:
    void setLoadingFinished(bool);
    void setLoadingStarted();

  private:
    int timeout;
    bool loading_finished;
    bool loaded;
    QString file;
};

#endif

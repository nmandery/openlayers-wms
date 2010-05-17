#ifndef CLASS_JSCALLBACKS_H
#define CLASS_JSCALLBACKS_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>

class JsCallbacks : public QObject {

  Q_OBJECT;

  public:
    JsCallbacks(QObject* parent = 0);

  signals:
    /**
     * emitted when tha javascript api has 
     * finished whatever it was doing
     */
    void ready();

  public slots:
    /**
     * to be called from Javascript
     * when it has finished perforing a task
     */
    Q_INVOKABLE void done();

    Q_INVOKABLE const QString getVersion();

};

#endif //CLASS_JSCALLBACKS_H

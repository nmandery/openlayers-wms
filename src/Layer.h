#ifndef CLASS_LAYER_H
#define CLASS_LAYER_H

#include <QObject>
#include "BoundingBox.h"
#include <QDebug>

class Layer : public QObject {
  Q_OBJECT

  public:
    Layer(QObject* parent = 0);
    QString name;
    QString title;
    BoundingBox bbox;

};

QDataStream &operator<<(QDataStream &, const Layer &);
QDebug operator<<(QDebug dbg, const Layer&);

typedef QList<Layer *> LayerList;

#endif

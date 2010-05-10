#ifndef STRUCT_LAYER_H
#define STRUCT_LAYER_H

#include <QObject>
#include "BoundingBox.h"

class Layer : public QObject {
  Q_OBJECT

  public:
    Layer(QObject* parent = 0);
    QString name;
    QString title;
    BoundingBox bbox;

};

typedef QList<Layer *> LayerList;

#endif

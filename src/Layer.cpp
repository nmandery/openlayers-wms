#include "Layer.h"


Layer::Layer(QObject* parent) 
  : QObject(parent) {

}


QDataStream& operator<<( QDataStream& out, const Layer& layer) {

    out << "Layer( name=" << layer.name 
        << ", title=" << layer.title 
        << ", bbox=" << layer.bbox << ")";

    return out;
}

QDebug operator<<(QDebug dbg, const Layer &layer) {

  dbg.nospace() << "Layer( name=" << layer.name
                << ", title=" << layer.title 
                << ", bbox=" << layer.bbox << ")";

  return dbg.space();
}

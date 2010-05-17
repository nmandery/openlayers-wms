#ifndef CLASS_BOUNDINGBOX_H
#define CLASS_BOUNDINGBOX_H

#include <QObject>
#include <QStringList>
#include <QDebug>

class BoundingBox : public QObject {
  Q_OBJECT;

  public:
    BoundingBox(QObject* parent = 0);
    double left;
    double top;
    double right;
    double bottom;
    
    /**
     * extract the values from a WMS BBOX param
     * in the form of 
     * left + "," + bottom + "," + right + "," + top
     */
    bool fromString(const QString &bboxstr);
};



QDataStream &operator<<(QDataStream &, const BoundingBox &);
QDebug operator<<(QDebug dbg, const BoundingBox &);

#endif //CLASS_BOUNDINGBOX_H

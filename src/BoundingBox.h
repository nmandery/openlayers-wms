#ifndef CLASS_BOUNDINGBOX_H_
#define CLASS_BOUNDINGBOX_H_

#include <QObject>

class BoundingBox : public QObject {
  Q_OBJECT;

  public:
    BoundingBox(QObject* parent = 0);
    double left;
    double top;
    double right;
    double bottom;
    //void fromString(const QString &bboxstr) {};
};


#endif

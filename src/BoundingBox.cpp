#include "BoundingBox.h"


BoundingBox::BoundingBox(QObject* parent) 
  : QObject(parent) 
{
}

bool BoundingBox::fromString(const QString &bboxstr) {

  bool allok = true;
  
  QStringList nums = bboxstr.split(",");
  if (nums.count() == 4) {
    double coords[4];
    bool ok;

    for(int i=0; i<4; ++i) {
      coords[i] = nums.at(i).trimmed().toDouble(&ok);
      if (!ok) {
        allok = false;
      }
    }

    if (allok) {
      left = coords[0];
      bottom = coords[1];
      right = coords[2];
      top = coords[3];
    }
  }
  else {
    allok = false;
  }

  return allok;
}

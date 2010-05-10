#ifndef STRUCT_LAYER_H
#define STRUCT_LAYER_H

#include "BBox.h"

typedef struct {
  QString name;
  QString title;
  BBox bbox; 
} Layer;

#endif

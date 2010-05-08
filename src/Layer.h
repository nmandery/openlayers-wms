
#ifndef STRUCT_BBOX_H
#define STRUCT_BBOX_H

typedef struct {
  double left;
  double top;
  double right;
  double bottom;
} BBox;

#endif



#ifndef STRUCT_LAYER_H
#define STRUCT_LAYER_H

typedef struct {
  QString name;
  QString title;
  BBox bbox; 
} Layer;

#endif

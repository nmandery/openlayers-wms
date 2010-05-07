#ifndef strct_bbox
#define strct_bbox
typedef struct {
  double left;
  double top;
  double right;
  double bottom;
} BBox;
#endif

#ifndef strct_layer
#define strct_layer
typedef struct {
  QString name;
  QString title;
  BBox bbox; 
} Layer;
#endif

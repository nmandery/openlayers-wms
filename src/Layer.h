
typedef struct {
  double left;
  double top;
  double right;
  double bottom;
} BBox;


typedef struct {
  QString name;
  QString title;
  BBox bbox; 
} Layer;

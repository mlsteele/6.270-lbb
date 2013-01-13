// 2d v2dtor manipulation

#ifndef _VECTORTOOLS_H_

typedef struct {
  float x;
  float y;
} v2d;

v2d v2c_add(v2d* a, v2d* b) {
  v2d v = {a->x + b->x, a->y + b->y};
  return v;
}

v2d v2d_sub(v2d* a, v2d* b) {
  v2d v = {a->x - b->x, a->y - b->y};
  return v;
}


#endif
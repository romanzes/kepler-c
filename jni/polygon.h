#ifndef POLYGON_H
#define POLYGON_H

#include "vector2.h"

int polyContainsV(Vector2 *poly, int offset, int count, Vector2 *vec);
int polygonsIntersect(Vector2 *polygon1, int offset1, int count1, Vector2 *polygon2, int offset2, int count2);

#endif /* POLYGON_H */

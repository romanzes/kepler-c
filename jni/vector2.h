#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct
{
    float x, y;
} Vector2;

void vecRotate(Vector2 *vec, float angle);
void vecRotateAround(Vector2 *vec1, Vector2 *vec2, float angle);
float vecLenV(Vector2 *vec);
float vecLenF(float x, float y);
float vecDistanceV(Vector2 *vec1, Vector2 *vec2);
float vecDistanceF(float x1, float y1, float x2, float y2);
void vecMulAddV(Vector2 *vec1, Vector2 *vec2, float scale);

#endif /* VECTOR2_H */

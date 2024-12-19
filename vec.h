#ifndef __VEC__H
#define __VEC__H

#include <stdlib.h>

typedef struct {
    size_t x, y;
} Vec2;

Vec2 add(Vec2 a, Vec2 b);
Vec2 sub(Vec2 a, Vec2 b);

#endif
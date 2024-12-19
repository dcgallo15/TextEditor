#include "vec.h"

Vec2 add(Vec2 a, Vec2 b) {
    Vec2 ret = {
        .x = a.x + b.x,
        .y = a.y + b.y
    };
    return ret;
}

Vec2 sub(Vec2 a, Vec2 b) {
    Vec2 ret = {
        .x = a.x - b.x,
        .y = a.y - b.y
    };
    return ret;
}
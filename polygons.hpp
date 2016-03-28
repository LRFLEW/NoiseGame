#ifndef __Noise__polygons__
#define __Noise__polygons__

#include "utilities.hpp"

struct Point {
    GLfloat x, y;
};

struct Rect {
    GLfloat x0, y0, x1, y1;
};

struct Triangle {
    GLfloat x0, y0, x1, y1, x2, y2;
};

union RectTriangles {
    struct {Triangle t1, t2;};
    struct {GLfloat x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;};
};

constexpr RectTriangles RectToTriangles(const Rect rect) {
    return {
        rect.x0, rect.y0, rect.x0, rect.y1, rect.x1, rect.y1,
        rect.x1, rect.y1, rect.x1, rect.y0, rect.x0, rect.y0
    };
}

constexpr bool RectContainsPoint(const SDL_Point point, const SDL_Rect rect) {
    return point.x >= rect.x && point.x <= rect.x + rect.w && point.y >= rect.y && point.y <= rect.y + rect.h;
}

constexpr bool RectContainsPoint(const Point point, const Rect rect) {
    return point.x >= rect.x0 && point.x <= rect.x1 && point.y >= rect.y0 && point.y <= rect.y1;
}

constexpr Point PointFromSDLPoint(const SDL_Point point, const SDL_Rect size) {
    return {
        (GLfloat) (point.x - size.x) / size.w * 2.0f - 1.0f,
        (GLfloat) (point.y - size.y) / size.h * -2.0f + 1.0f
    };
}

constexpr Rect RectFromSDLRect(const SDL_Rect rect, const SDL_Rect size) {
    return {
        (GLfloat) (rect.x - size.x) / size.w * 2.0f - 1.0f,
        (GLfloat) (size.h - rect.y + size.y) / size.h * 2.0f - 1.0f,
        (GLfloat) (rect.x + rect.w - size.x) / size.w * 2.0f - 1.0f,
        (GLfloat) (size.h - rect.y - rect.h + size.y) / size.h * 2.0f - 1.0f
    };
}

constexpr RectTriangles RTrisFromSDLRect(const SDL_Rect rect, const SDL_Rect size) {
    return RectToTriangles(RectFromSDLRect(rect, size));
}

#endif /* defined(__Noise__polygons__) */

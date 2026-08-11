#include "class.h"
#include <math.h>
#include <stdio.h>
#include <sys/select.h>

class(Vec2, {
    int x;
    int y;
});

constructor(Vec2, int x, int y) { return (Vec2){x, y}; }

Vec2 method(Vec2, add, Vec2 *other) {
    return Vec2_create(self->x + other->x, self->y + other->y);
}

Vec2 method(Vec2, sub, Vec2 *other) {
    return Vec2_create(self->x - other->x, self->y - other->y);
}

Vec2 method(Vec2, scale, int n) {
    return Vec2_create(self->x * n, self->y * n);
}

double method0(Vec2, distance) {
    return sqrt((double)(self->x * self->x + self->y * self->y));
}

class(Vec3, {
    Vec2 super;
    int z;
});

constructor(Vec3, int x, int y, int z) {
    return (Vec3){
        Vec2_create(x, y),
        z,
    };
}

Vec3 Vec3_from_vec2(Vec2 super, int z) { return (Vec3){super, z}; }

Vec3 method(Vec3, add, Vec3 *other) {
    return Vec3_from_vec2(Vec2_add(downcast(self), downcast(other)),
                          self->z + other->z);
}

double method0(Vec3, distance) {
    return sqrt((double)(self->super.x * self->super.x +
                         self->super.y * self->super.y + self->z * self->z));
}

interface(Distance, { double virtual0(distance); });

double method0(Distance, distance) { return vcall0(self, distance); }

impl(Vec2, Distance, {vcast0(Vec2_distance, double)});

impl(Vec3, Distance, {vcast0(Vec3_distance, double)});

int main() {
    Vec2 vec2 = Vec2_create(5, 10);
    printf("Vec2 distance: %f\n", Vec2_distance(&vec2));
    Vec3 vec3 = Vec3_from_vec2(vec2, 15);
    printf("Vec3 distance: %f\n", Vec3_distance(&vec3));

    Distance distances[2] = {
        dyn(Distance, Vec2, &vec2),
        dyn(Distance, Vec3, &vec3),
    };

    for (int i = 0; i < 2; i++) {
        printf("Distance: %f\n", Distance_distance(&distances[i]));
    }
}

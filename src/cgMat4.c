#include "cgMat4.h"

#define _USER_MATH_DEFINES
#include <math.h>
#include <string.h>

#define DEGREES(radians) ((radians) * 180.0 / M_PI)
#define RADIANS(degrees) ((degrees) * M_PI / 180.0)

cgMat4 *cgMat4_identity(cgMat4 *self) {
    memset(self->m, 0, sizeof(self->m));
    self->m00 = self->m11 = self->m22 = self->m33 = 1.0;
    return self;
}

cgMat4 *cgMat4_perspective(cgMat4 *self, cgScalar fov, cgScalar aspect,
                           cgScalar znear, cgScalar zfar) {
    cgScalar r = RADIANS(fov / 2);
    cgScalar dz = zfar - znear;
    cgScalar s = sin(r);
    cgScalar cotangent = 0;

    if (dz == 0 || s == 0 || aspect == 0) {
        return NULL;
    }


    cotangent = cos(r) / s;

    cgMat4_identity(self);
    self->m00 = cotangent / aspect;
    self->m11 = cotangent;
    self->m22 = -(zfar + znear) / dz;
    /*self->m23 = 2 * zfar * znear / dz;*/
    self->m23 = -2 * zfar * znear / dz;
    self->m32 = -1;
    self->m33 = 0;

    return self;
}

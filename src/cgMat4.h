#ifndef CG_MAT4_H
#define CG_MAT4_H

#include "cgMath.h"

typedef struct cgMat4 {
    union {
        cgScalar m[16];

#if CG_MATH_GL_MATRIX_TRANSPOSE
        struct {
            cgScalar m00, m01, m02, m03;
            cgScalar m10, m11, m12, m13;
            cgScalar m20, m21, m22, m23;
            cgScalar m30, m31, m32, m33;
        };
#else
        struct {
            cgScalar m00, m10, m20, m30;
            cgScalar m01, m11, m21, m31;
            cgScalar m02, m12, m22, m32;
            cgScalar m03, m13, m23, m33;
        };
#endif
    };
} cgMat4;


cgMat4 *cgMat4_identity(cgMat4 *self);

cgMat4 *cgMat4_perspective(cgMat4 *self, cgScalar fov, cgScalar aspect,
                           cgScalar znear, cgScalar zfar);

#endif // CG_MAT4_H

#include "lh_math.h"

#include <windows.h>
#include <stdio.h>
#include <limits.h>

// F32 FUNCTIONS

f32 lerp(f32 a, f32 b, f32 t) {
    f32 result = a + (b - a) * t;
    return result;
}

f32 Lerp(f32 a, f32 b, f32 t) {
    return (1.0f - t) * a + b * t;
}
f32 InvLerp(f32 a, f32 b, f32 v) {
    return (v - a) / (b - a);
}
f32 Remap(f32 iMin, f32 iMax, f32 oMin, f32 oMax, f32 v) {
    f32 t = InvLerp(iMin, iMax, v);
    return Lerp(oMin, oMax, t);
}


// VEC2 FUNCTIONS
vec2 operator+(vec2 l, vec2 r) {
    vec2 result = { l.x + r.x, l.y + r.y };
    return result;
}

vec2 operator-(vec2 l, vec2 r) {
    vec2 result = { l.x - r.x, l.y - r.y };
    return result;
}

vec2 operator*(vec2 l, vec2 r) {
    vec2 result = { l.x * r.x, l.y * r.y };
    return result;
}

vec2 operator*(vec2 v, f32 f) {
    vec2 result = { v.x * f, v.y * f };
    return result;
}

vec2 operator/(vec2 l, vec2 r) {
    vec2 result = { l.x / r.x, l.y / r.y };
    return result;
}

f32 dot(vec2 l, vec2 r) {
    return l.x * r.x + l.y * r.y;
}

f32 lenSq(vec2 v) {
    f32 lenSq = v.x * v.x + v.y * v.y;
    if(lenSq < EPSILON) {
        return 0.0f;
    }
    return lenSq;
}

f32 len(vec2 v) {
    f32 lenSq = v.x * v.x + v.y * v.y;
    if(lenSq < EPSILON) {
        return 0.0f;
    }
    return sqrtf(lenSq);
}

void normalize(vec2 *v) {
    f32 lenSq = v->x * v->x + v->y * v->y;
    if(lenSq < EPSILON) {
        return;
    }
    f32 invLen = 1.0f / sqrtf(lenSq);
    v->x *= invLen;
    v->y *= invLen;
}

vec2 normalized(vec2 v) {
    f32 lenSq = v.x * v.x + v.y * v.y;
    if(lenSq < EPSILON) {
        return v;
    }
    f32 invLen = 1.0f / sqrtf(lenSq);
    vec2 result = { v.x * invLen, v.y * invLen };
    return result;
}

f32 angle(vec2 l, vec2 r) {
    f32 lenSqL = l.x * l.x + l.y * l.y;
    f32 lenSqR = r.x * r.x + r.y * r.y;
    if(lenSqL < EPSILON || lenSqR < EPSILON) {
        return 0;
    }
    f32 dot = l.x * r.x + l.y * r.y;
    f32 len = sqrtf(lenSqL) * sqrtf(lenSqR);
    return acosf(dot / len);
}

vec2 project(vec2 a, vec2 b) {
    f32 magB = len(b);
    if(magB < EPSILON) {
        vec2 zero = {};
        return zero;
    }
    f32 scale = dot(a, b) / magB;
    return b * scale;
}

vec2 reject(vec2 a, vec2 b) {
    vec2 projection = project(a, b);
    return a - projection;
}

vec2 reflect(vec2 a, vec2 b) {
    f32 magB = len(b);
    if(magB < EPSILON) {
        vec2 zero = {};
        return zero;
    }
    
    f32 scale = dot(a, b) / magB;
    vec2 proj2 = b * (scale * 2.0f);
    return a - proj2;
}

vec2 lerp(vec2 a, vec2 b, f32 t) {
    vec2 result = {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
    return result;
}

vec2 nlerp(vec2 a, vec2 b, f32 t) {
    vec2 result = {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
    return normalized(result);
}

vec2 slerp(vec2 s, vec2 e, f32 t) {
    if(t < 0.01f) {
        return lerp(s, e, t);
    }
    vec2 from = normalized(s);
    vec2 to = normalized(e);
    f32 theta = angle(from, to);
    f32 sin_theta = sinf(theta);
    f32 a = sinf((1.0f - t) * theta) / sin_theta;
    f32 b = sinf(t * theta) / sin_theta;
    return from * a + to * b;
}

// VEC3 FUNCTIONS
vec3 operator+(vec3 l, vec3 r) {
    vec3 result = { l.x + r.x, l.y + r.y, l.z + r.z };
    return result;
}

vec3 operator-(vec3 l, vec3 r) {
    vec3 result = { l.x - r.x, l.y - r.y, l.z - r.z };
    return result;
}

vec3 operator*(vec3 l, vec3 r) {
    vec3 result = { l.x * r.x, l.y * r.y, l.z * r.z };
    return result;
}

vec3 operator*(vec3 v, f32 f) {
    vec3 result = { v.x * f, v.y * f, v.z * f };
    return result;
}

vec3 operator/(vec3 l, vec3 r) {
    vec3 result = { l.x / r.x, l.y / r.y, l.z / r.z };
    return result;
}

vec3 operator/(vec3 l, f32 f) {
    vec3 result = { l.x / f, l.y / f, l.z / f };
    return result;
}

f32 dot(vec3 l, vec3 r) {
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

f32 lenSq(vec3 v) {
    f32 lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
    if(lenSq < EPSILON) {
        return 0.0f;
    }
    return lenSq;
}

f32 len(vec3 v) {
    f32 lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
    if(lenSq < EPSILON) {
        return 0.0f;
    }
    return sqrtf(lenSq);
}

void normalize(vec3 *v) {
    f32 lenSq = v->x * v->x + v->y * v->y + v->z * v->z;
    if(lenSq < EPSILON) {
        return;
    }
    f32 invLen = 1.0f / sqrtf(lenSq);
    v->x *= invLen;
    v->y *= invLen;
    v->z *= invLen;
}

vec3 normalized(vec3 v) {
    f32 lenSq = v.x * v.x + v.y * v.y + v.z * v.z;;
    if(lenSq < EPSILON) {
        return v;
    }
    f32 invLen = 1.0f / sqrtf(lenSq);
    vec3 result = { v.x * invLen, v.y * invLen, v.z * invLen };
    return result;
}

f32 angle(vec3 l, vec3 r) {
    f32 lenSqL = l.x * l.x + l.y * l.y + l.z * l.z;
    f32 lenSqR = r.x * r.x + r.y * r.y + r.z * r.z;
    if(lenSqL < EPSILON || lenSqR < EPSILON) {
        return 0;
    }
    f32 dot = l.x * r.x + l.y * r.y + l.z * r.z;
    f32 len = sqrtf(lenSqL) * sqrtf(lenSqR);
    f32 result = acosf(dot / len);
    if(isnan(result)) return 0.0f;
    return result;
}

vec3 project(vec3 a, vec3 b) {
    f32 magB = len(b);
    if(magB < EPSILON) {
        vec3 zero = {};
        return zero;
    }
    f32 scale = dot(a, b) / magB;
    return b * scale;
}

vec3 reject(vec3 a, vec3 b) {
    vec3 projection = project(a, b);
    return a - projection;
}

vec3 reflect(vec3 a, vec3 b) {
    f32 magB = len(b);
    if(magB < EPSILON) {
        vec3 zero = {};
        return zero;
    }
    
    f32 scale = dot(a, b) / magB;
    vec3 proj2 = b * (scale * 2.0f);
    return a - proj2;
}

vec3 cross(vec3 l, vec3 r) {
    vec3 result = {
        l.y * r.z - l.z * r.y,      
        l.z * r.x - l.x * r.z,      
        l.x * r.y - l.y * r.x
    };
    return result;
}

vec3 lerp(vec3 a, vec3 b, f32 t) {
    vec3 result = {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
    return result;
}

vec3 nlerp(vec3 a, vec3 b, f32 t) {
    vec3 result = {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
    return normalized(result);
}

vec3 slerp(vec3 s, vec3 e, f32 t) {
    if(t < 0.01f) {
        return lerp(s, e, t);
    }
    vec3 from = normalized(s);
    vec3 to = normalized(e);
    f32 theta = angle(from, to);
    f32 sin_theta = sinf(theta);
    f32 a = sinf((1.0f - t) * theta) / sin_theta;
    f32 b = sinf(t * theta) / sin_theta;
    return from * a + to * b;
}

void normalize(vec4 *v) {
    f32 lenSq = v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
    if(lenSq < EPSILON) {
        return;
    }
    f32 invLen = 1.0f / sqrtf(lenSq);
    v->x *= invLen;
    v->y *= invLen;
    v->z *= invLen;
    v->w *= invLen;
}

#if 0
bool operator==(vec2 a, vec2 b) {
    if(a.x == b.x &&
       a.y == b.y) {
        return true;
    }
    return false;
}

bool operator==(vec3 a, vec3 b) {
    if(a.x == b.x &&
       a.y == b.y &&
       a.z == b.z) {
        return true;
    }
    return false;
}
#else
bool operator==(vec2 a, vec2 b) {
    if(fabsf(a.x - b.x) <= FLT_EPSILON &&
       fabsf(a.y - b.y) <= FLT_EPSILON) {
        return true;
    }
    return false;
}

bool operator==(vec3 a, vec3 b) {
    if(fabsf(a.x - b.x) <= FLT_EPSILON &&
       fabsf(a.y - b.y) <= FLT_EPSILON &&
       fabsf(a.z - b.z) <= FLT_EPSILON) {
        return true;
    }
    return false;
}
#endif

// MAT2 FUNCTIONS
mat2 Mat2Identity() {
    mat2 result = {
        1, 0,
        0, 1
    };
    return result;
}

void Mat2Print(mat2 m) {
    char buffer[256];
    sprintf(buffer, "|%.1f  %.1f|\n", m.m00, m.m01);
    OutputDebugString(buffer);
    sprintf(buffer, "|%.1f  %.1f|\n", m.m10, m.m11);
    OutputDebugString(buffer);
    OutputDebugString("##########################################\n");
}

mat2 Mat2Rotate(f32 angle) {
    mat2 result = {
        cosf(angle), sinf(angle),
        -sinf(angle), cosf(angle),
    };
    return result;
}

mat2 operator+(mat2 a, mat2 b) {
    mat2 result = {
        a.m00 + b.m00, a.m01 + b.m01,
        a.m10 + b.m10, a.m11 + b.m11
    };
    return result;
}

mat2 operator*(mat2 m, f32 f) {
    mat2 result = {
        m.m00 * f, m.m01 * f,
        m.m10 * f, m.m11 * f
    };
    return result;
}

vec2 operator*(mat2 m, vec2 v) {
    vec2 result = {
        m.m00 * v.x + m.m01 * v.y,
        m.m10 * v.x + m.m11 * v.y
    };
    return  result;
}

mat2 operator*(mat2 a, mat2 b) {
    mat2 result = {
        a.m00 * b.m00 + a.m01 * b.m10,
        a.m00 * b.m01 + a.m01 * b.m11,
        a.m10 * b.m00 + a.m10 * b.m10,
        a.m10 * b.m01 + a.m11 * b.m11
    };
    return result;
}

// MAT3 FUNCTIONS
mat3 Mat3Identity() {
    mat3 result = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    return result;
}

void Mat3Print(mat3 m) {
    char buffer[256];
    sprintf(buffer, "|%.1f  %.1f  %.1f|\n", m.m00, m.m01, m.m02);
    OutputDebugString(buffer);
    sprintf(buffer, "|%.1f  %.1f  %.1f|\n", m.m10, m.m11, m.m12);
    OutputDebugString(buffer);
    sprintf(buffer, "|%.1f  %.1f  %.1f|\n", m.m20, m.m21, m.m22);
    OutputDebugString(buffer);
    OutputDebugString("##########################################\n");
}

mat3 Mat3Scale(f32 x, f32 y, f32 z) {
    mat3 result = {
        x, 0, 0,
        0, y, 0,
        0, 0, z
    };
    return result;
}

mat3 Mat3RotateX(f32 angle) {
    mat3 result = {
        1, 0, 0,
        0, cosf(angle), sinf(angle),
        0, -sinf(angle), cosf(angle), 
    };
    return result;
}

mat3 Mat3RotateY(f32 angle) {
    mat3 result = {
        cosf(angle), 0, sinf(angle),
        0, 1, 0,
        -sinf(angle), 0, cosf(angle),
    };
    return result;
}

mat3 Mat3RotateZ(f32 angle) {
    mat3 result = {
        cosf(angle), sinf(angle), 0,
        -sinf(angle), cosf(angle), 0,
        0, 0, 1,
    };
    return result;
}

mat3 Mat3Rotate(vec3 axis, f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);
    f32 t = 1.0f - cosf(angle);
    normalize(&axis);
    f32 x = axis.x;
    f32 y = axis.y;
    f32 z = axis.z;
    return {
        t * (x * x) + c,t * x * y + s * z,t * x * z - s * y,
        t * x * y - s * z,t * (y * y) + c,t * y * z + s * x,
        t * x * z + s * y,t * y * z - s * x,t * (z * z) + c
    };
}


mat3 operator+(mat3 a, mat3 b) {
    mat3 result = {
        a.m00 + b.m00, a.m01 + b.m01, a.m02 + b.m02, 
        a.m10 + b.m10, a.m11 + b.m11, a.m12 + b.m12, 
        a.m20 + b.m20, a.m21 + b.m21, a.m22 + b.m22
    };
    return result;
}

mat3 operator*(mat3 m, f32 f) {
    mat3 result = {
        m.m00 * f, m.m01 * f, m.m02 * f, 
        m.m10 * f, m.m11 * f, m.m12 * f, 
        m.m20 * f, m.m21 * f, m.m22 * f
    };
    return result;
}

vec3 operator*(mat3 m, vec3 v) {
    vec3 result = {
        m.m00 * v.x + m.m01 * v.y + m.m02 * v.z, 
        m.m10 * v.x + m.m11 * v.y + m.m12 * v.z, 
        m.m20 * v.x + m.m21 * v.y + m.m22 * v.z
    };
    return result;
}

mat3 operator*(mat3 a, mat3 b) {
    mat3 result = {
        a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20,
        a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21, 
        a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22, 
        a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20,
        a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21, 
        a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22, 
        a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20,
        a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21, 
        a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22
    };
    return result;
}

// MAT4 FUNCTIONS
mat4 Mat4Identity() {
    mat4 result = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    return result;
}

void Mat4Print(mat4 m) {
    char buffer[256];
    sprintf(buffer, "|%.1f  %.1f  %.1f  %.1f|\n", m.m00, m.m01, m.m02, m.m03);
    OutputDebugString(buffer);
    sprintf(buffer, "|%.1f  %.1f  %.1f  %.1f|\n", m.m10, m.m11, m.m12, m.m13);
    OutputDebugString(buffer);
    sprintf(buffer, "|%.1f  %.1f  %.1f  %.1f|\n", m.m20, m.m21, m.m22, m.m23);
    OutputDebugString(buffer);
    sprintf(buffer, "|%.1f  %.1f  %.1f  %.1f|\n", m.m30, m.m31, m.m32, m.m33);
    OutputDebugString(buffer);
    OutputDebugString("##########################################\n");
}

mat4 Mat4Translate(f32 x, f32 y, f32 z) {
    mat4 result = {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };
    return result;
}

mat4 Mat4Scale(f32 x, f32 y, f32 z) {
    mat4 result = {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
    return result;
}

mat4 Mat4RotateX(f32 angle) {
    mat4 result = {
        1, 0, 0, 0,
        0, cosf(angle), sinf(angle), 0,
        0, -sinf(angle), cosf(angle), 0,
        0, 0, 0, 1 
    };
    return result;
}

mat4 Mat4RotateY(f32 angle) {
    mat4 result = {
        cosf(angle), 0, sinf(angle), 0,
        0, 1, 0, 0,
        -sinf(angle), 0, cosf(angle), 0,
        0, 0, 0, 1 
    };
    return result;
}

mat4 Mat4RotateZ(f32 angle) {
    mat4 result = {
        cosf(angle), sinf(angle), 0, 0,
        -sinf(angle), cosf(angle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    return result;
}




mat4 Mat4Transpose(mat4 m) {
    mat4 result = {
        m.m00, m.m10, m.m20, m.m30, 
        m.m01, m.m11, m.m21, m.m31, 
        m.m02, m.m12, m.m22, m.m32, 
        m.m03, m.m13, m.m23, m.m33 
    };
    return result;
}

mat4 operator+(mat4 a, mat4 b) {
    mat4 result = {
        a.m00 + b.m00, a.m01 + b.m01, a.m02 + b.m02, a.m03 + b.m03,
        a.m10 + b.m10, a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13,
        a.m20 + b.m20, a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23,
        a.m30 + b.m30, a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33
    };
    return result;
}

mat4 operator*(mat4 m, f32 f) {
    mat4 result = {
        m.m00 * f, m.m01 * f, m.m02 * f, m.m03 * f,
        m.m10 * f, m.m11 * f, m.m12 * f, m.m13 * f,
        m.m20 * f, m.m21 * f, m.m22 * f, m.m23 * f,
        m.m30 * f, m.m31 * f, m.m32 * f, m.m33 * f
    };
    return result;
}

internal
__m128 Vec4Mat4MulSSE(__m128 v, mat4 m) {
    // first transpose v
    __m128 vX = _mm_shuffle_ps(v, v, 0x00); // (vx,vx,vx,vx)
    __m128 vY = _mm_shuffle_ps(v, v, 0x55); // (vy,vy,vy,vy)
    __m128 vZ = _mm_shuffle_ps(v, v, 0xAA); // (vz,vz,vz,vz)
    __m128 vW = _mm_shuffle_ps(v, v, 0xFF); // (vw,vw,vw,vw)
    _MM_TRANSPOSE4_PS(m.row[0], m.row[1], m.row[2], m.row[3]);
    __m128 result = _mm_mul_ps(vX, m.row[0]);
    result = _mm_add_ps(result, _mm_mul_ps(vY, m.row[1]));
    result = _mm_add_ps(result, _mm_mul_ps(vZ, m.row[2]));
    result = _mm_add_ps(result, _mm_mul_ps(vW, m.row[3]));
    return result;
}

mat4 Mat4MulSSE(mat4 a, mat4 b) {
    mat4 result;
    result.row[0] = Vec4Mat4MulSSE(a.row[0], b);
    result.row[1] = Vec4Mat4MulSSE(a.row[1], b);
    result.row[2] = Vec4Mat4MulSSE(a.row[2], b);
    result.row[3] = Vec4Mat4MulSSE(a.row[3], b);
    return result;
}

#if 0

vec4 operator*(mat4 m, vec4 v) {
    vec4 result = {
        m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w, 
        m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w, 
        m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w, 
        m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w 
    };
    return result;
}

mat4 operator*(mat4 a, mat4 b) {
    mat4 result = {
        a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30, 
        a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31, 
        a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32,
        a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33,
        a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30, 
        a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31, 
        a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32,
        a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33,
        a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30, 
        a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31, 
        a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32,
        a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33,
        a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30, 
        a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31, 
        a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32,
        a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33
    };
    return result;
}
#else

vec4 operator*(mat4 m, vec4 v) {
    vec4 result;
    __m128 vX = _mm_shuffle_ps(v.wide, v.wide, 0x00); // (vx,vx,vx,vx)
    __m128 vY = _mm_shuffle_ps(v.wide, v.wide, 0x55); // (vy,vy,vy,vy)
    __m128 vZ = _mm_shuffle_ps(v.wide, v.wide, 0xAA); // (vz,vz,vz,vz)
    __m128 vW = _mm_shuffle_ps(v.wide, v.wide, 0xFF); // (vw,vw,vw,vw)
    _MM_TRANSPOSE4_PS(m.row[0], m.row[1], m.row[2], m.row[3]);
    result.wide = _mm_mul_ps(vX, m.row[0]);
    result.wide = _mm_add_ps(result.wide, _mm_mul_ps(vY, m.row[1]));
    result.wide = _mm_add_ps(result.wide, _mm_mul_ps(vZ, m.row[2]));
    result.wide = _mm_add_ps(result.wide, _mm_mul_ps(vW, m.row[3]));
    return result;
}



mat4 operator*(mat4 a, mat4 b) {
    mat4 result;
    result.row[0] = Vec4Mat4MulSSE(a.row[0], b);
    result.row[1] = Vec4Mat4MulSSE(a.row[1], b);
    result.row[2] = Vec4Mat4MulSSE(a.row[2], b);
    result.row[3] = Vec4Mat4MulSSE(a.row[3], b);
    return result;
}

#endif

mat4 Mat4Frustum(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {	
    mat4 result = {
        (2.0f * n) / (r - l), 0, -(r + l) / (r - l), 0,
        0, (2.0f * n) / (t - b), -(t + b) / (t - b), 0,
        0, 0, f / (f - n), -(f * n) / (f - n),
        0, 0, 1, 0
    };
    return result;
}

mat4 Mat4Perspective(f32 fov, f32 aspect, f32 znear, f32 zfar) {
	f32 ymax = znear * tanf(fov * 3.14159265359f / 360.0f);
	f32 xmax = ymax * aspect;
	return Mat4Frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

mat4 Mat4Ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    mat4 result = {
        2.0f / (r - l), 0, 0, -(r + l) / (r - l),
        0, 2.0f / (t - b), 0, -(t + b) / (t - b),
        0, 0, 1.0f / (f - n), -n / (f - n),
        0, 0, 0, 1
    };
    return result;
}

mat4 Mat4LookAt(vec3 position, vec3 target, vec3 up) {
    vec3 zaxis = normalized(target - position);
    vec3 xaxis = normalized(cross(up, zaxis));
    vec3 yaxis = cross(zaxis, xaxis);
    mat4 result = {
        xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, position), 
        yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, position), 
        zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, position),
        0,       0,       0,       1 
    };
    return result;
}

#define M4SWAP(x, y) \
    {f32 t = x; x = y; y = t; }

void transpose(mat4& m) {
	M4SWAP(m.yx, m.xy);
	M4SWAP(m.zx, m.xz);
	M4SWAP(m.tx, m.xw);
	M4SWAP(m.zy, m.yz);
	M4SWAP(m.ty, m.yw);
	M4SWAP(m.tz, m.zw);
}

mat4 transposed(mat4 m) {
	return {
		m.xx, m.yx, m.zx, m.tx,
		m.xy, m.yy, m.zy, m.ty,
		m.xz, m.yz, m.zz, m.tz,
		m.xw, m.yw, m.zw, m.tw
    };
}

#define M4_3X3MINOR(c0, c1, c2, r0, r1, r2) \
    (m.m[c0 * 4 + r0] * (m.m[c1 * 4 + r1] * m.m[c2 * 4 + r2] - m.m[c1 * 4 + r2] * m.m[c2 * 4 + r1]) - \
     m.m[c1 * 4 + r0] * (m.m[c0 * 4 + r1] * m.m[c2 * 4 + r2] - m.m[c0 * 4 + r2] * m.m[c2 * 4 + r1]) + \
     m.m[c2 * 4 + r0] * (m.m[c0 * 4 + r1] * m.m[c1 * 4 + r2] - m.m[c0 * 4 + r2] * m.m[c1 * 4 + r1]))

float determinant(mat4 m) {
	return  m.m[0]  * M4_3X3MINOR(1, 2, 3, 1, 2, 3)
		  - m.m[4]  * M4_3X3MINOR(0, 2, 3, 1, 2, 3)
		  + m.m[8]  * M4_3X3MINOR(0, 1, 3, 1, 2, 3)
		  - m.m[12] * M4_3X3MINOR(0, 1, 2, 1, 2, 3);
}

mat4 adjugate(mat4 m) {
	// Cofactor(M[i, j]) = Minor(M[i, j]] * pow(-1, i + j)
	mat4 cofactor;

	cofactor.m[0] = M4_3X3MINOR(1, 2, 3, 1, 2, 3);
	cofactor.m[1] = -M4_3X3MINOR(1, 2, 3, 0, 2, 3);
	cofactor.m[2] = M4_3X3MINOR(1, 2, 3, 0, 1, 3);
	cofactor.m[3] = -M4_3X3MINOR(1, 2, 3, 0, 1, 2);

	cofactor.m[4] = -M4_3X3MINOR(0, 2, 3, 1, 2, 3);
	cofactor.m[5] = M4_3X3MINOR(0, 2, 3, 0, 2, 3);
	cofactor.m[6] = -M4_3X3MINOR(0, 2, 3, 0, 1, 3);
	cofactor.m[7] = M4_3X3MINOR(0, 2, 3, 0, 1, 2);

	cofactor.m[8] = M4_3X3MINOR(0, 1, 3, 1, 2, 3);
	cofactor.m[9] = -M4_3X3MINOR(0, 1, 3, 0, 2, 3);
	cofactor.m[10] = M4_3X3MINOR(0, 1, 3, 0, 1, 3);
	cofactor.m[11] = -M4_3X3MINOR(0, 1, 3, 0, 1, 2);

	cofactor.m[12] = -M4_3X3MINOR(0, 1, 2, 1, 2, 3);
	cofactor.m[13] = M4_3X3MINOR(0, 1, 2, 0, 2, 3);
	cofactor.m[14] = -M4_3X3MINOR(0, 1, 2, 0, 1, 3);
	cofactor.m[15] = M4_3X3MINOR(0, 1, 2, 0, 1, 2);

	return transposed(cofactor);
}

mat4 inverse(mat4 m) {
	f32 det = determinant(m);

	if (det == 0.0f) { // Epsilon check would need to be REALLY small
		OutputDebugString("WARNING: Trying to invert a matrix with a zero determinant\n");
		return {};
	}
	mat4 adj = adjugate(m);

	return adj * (1.0f / det);
}




vec3 Vec4ToVec3(vec4 v) {
    vec3 result = { v.x, v.y, v.z };
    return result;
}

vec4 Vec3ToVec4(vec3 v, f32 w) {
    vec4 result = {v.x, v.y, v.z, w};
    return result;
}

rectangle2i RectangleInvertedInfinity() {
    rectangle2i result;
    result.minX = result.minY = INT_MAX;
    result.maxX = result.maxY = -INT_MAX;
    return result;
}

rectangle2i RectangleIntersect(rectangle2i a, rectangle2i b) {
    rectangle2i result;
    result.minX = (a.minX > b.minX) ? a.minX : b.minX;
    result.minY = (a.minY > b.minY) ? a.minY : b.minY;
    result.maxX = (a.maxX < b.maxX) ? a.maxX : b.maxX;
    result.maxY = (a.maxY < b.maxY) ? a.maxY : b.maxY;
    return result;
}

rectangle2i RectangleUnion(rectangle2i a, rectangle2i b) {
    rectangle2i result;
    result.minX = (a.minX < b.minX) ? a.minX : b.minX;
    result.minY = (a.minY < b.minY) ? a.minY : b.minY;
    result.maxX = (a.maxX > b.maxX) ? a.maxX : b.maxX;
    result.maxY = (a.maxY > b.maxY) ? a.maxY : b.maxY;
    return result;
}

i32 RectangleGetClampArea(rectangle2i a) {
    i32 width = (a.maxX - a.minX);
    i32 height = (a.maxY - a.minY);
    i32 result = 0;
    if((width > 0) && (height) > 0) {
        result = width * height;
    }
    return result;
}

bool RectangleHasArea(rectangle2i a) {
    bool result = ((a.minX < a.maxX) && (a.minY < a.maxY));
    return result;
}

mat4 TransformToMat4(vec3 position, quat rotation, vec3 scale) {
    mat4 translationMat = Mat4Translate(position.x, position.y, position.z);
    mat4 scaleMat = Mat4Scale(scale.x, scale.y, scale.z);
    mat4 rotationMat = quatToMat4(rotation);
    mat4 world = translationMat * rotationMat * scaleMat;
    return world;
}

quat angleAxis(float angle, const vec3 &axis) {
    vec3 norm = normalized(axis);
    float s = sinf(angle * 0.5f);
    return  quat(norm.x * s,
                 norm.y * s,
                 norm.z * s,
                 cosf(angle * 0.5f));
}

quat fromTo(const vec3 &from, const vec3 &to) {
    vec3 f = normalized(from);
    vec3 t = normalized(to);
    // make sure they are not the same vector
    if(f == t) {
        return quat();
    }
    // check whether the two vectors are oposites of each other.
    // if they are the most orthogonal axis of the from can be used
    // to create a pure quaternion
    else if(f == t * -1.0f) {
        vec3 ortho = Vec3(1.0f, 0.0f, 0.0f);
        if(fabsf(f.y) < fabsf(f.x)) {
            ortho = Vec3(0.0f, 1.0f, 0.0f);
        }
        if(fabsf(f.z) < fabsf(f.y) && fabsf(f.z) < fabsf(f.x)) {
            ortho = Vec3(0.0f, 0.0f, 1.0f); 
        }
        vec3 axis = normalized(cross(f, ortho));
        return quat(axis.x, axis.y, axis.z, 0.0f);
    }
    // create a half vector between the from and to vectors
    // use the cross product of the half vector and the starting
    // vector to calculate the axis of rotation and the dot product
    // of the two to find the angle of rotation
    vec3 half = normalized(f + t);
    vec3 axis = cross(f, half);
    return quat(axis.x, axis.y, axis.z, dot(f, half));
}

vec3 getAxis(const quat &q) {
    return normalized(Vec3(q.x, q.y, q.z));
}

float getAngle(const quat &q) {
    return 2.0f * acosf(q.w);
}

quat operator+(const quat &a, const quat &b) {
    return quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

quat operator-(const quat &a, const quat &b) {
    return quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

quat operator*(const quat &a, float b) {
    return quat(a.x * b, a.y * b, a.z * b, a.w * b);
}

quat operator-(const quat &q) {
    return quat(-q.x, -q.y, -q.z, -q.w);
}

bool operator==(const quat &left, const quat &right) {
    return (fabsf(left.x - right.x) <= QUAT_EPSILON &&
            fabsf(left.y - right.y) <= QUAT_EPSILON &&
            fabsf(left.z - right.z) <= QUAT_EPSILON &&
            fabsf(left.w - right.w) <= QUAT_EPSILON);
}

bool operator!=(const quat &left, const quat &right) {
    return !(left == right);
}

bool sameOrientation(const quat& left, const quat& right) {
	return (fabsf(left.x - right.x) <= QUAT_EPSILON &&
            fabsf(left.y - right.y) <= QUAT_EPSILON &&
            fabsf(left.z - right.z) <= QUAT_EPSILON &&
            fabsf(left.w - left.w) <= QUAT_EPSILON) ||
           (fabsf(left.x + right.x) <= QUAT_EPSILON &&
            fabsf(left.y + right.y) <= QUAT_EPSILON &&
            fabsf(left.z + right.z) <= QUAT_EPSILON && 
            fabsf(left.w + left.w) <= QUAT_EPSILON);
}

float dot(const quat &a, const quat &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float lenSq(const quat &q) {
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

float len(const quat &q) {
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if(lenSq < QUAT_EPSILON) {
        return 0.0f;
    }
    return sqrtf(lenSq);
}

void normalize(quat &q) {
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if(lenSq < QUAT_EPSILON) {
        return;
    }
    float i_len = 1.0f / sqrtf(lenSq);
    q.x *= i_len;
    q.y *= i_len;
    q.z *= i_len;
    q.w *= i_len;
}

quat normalized(const quat &q) {
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if(lenSq < QUAT_EPSILON) {
        return quat();
    }
    float il = 1.0f / sqrtf(lenSq);
    return quat(q.x * il, q.y * il, q.z * il, q.w * il);
}

quat conjugate(const quat &q) {
    return quat(-q.x, -q.y, -q.z, q.w);
}

quat inverse(const quat &q) {
    float lenSq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
    if (lenSq < QUAT_EPSILON) {
        return quat();
    }
    float recip = 1.0f / lenSq;
    return quat(-q.x * recip, -q.y * recip, -q.z * recip, q.w * recip);
}

quat operator*(const quat& q1, const quat& q2) {
	return quat(
		q2.x * q1.w + q2.y * q1.z - q2.z * q1.y + q2.w * q1.x,
		-q2.x * q1.z + q2.y * q1.w + q2.z * q1.x + q2.w * q1.y,
		q2.x * q1.y - q2.y * q1.x + q2.z * q1.w + q2.w * q1.z,
		-q2.x * q1.x - q2.y * q1.y - q2.z * q1.z + q2.w * q1.w
	);
}

vec3 operator*(const quat &q, const vec3 &v) {
    return q.vector * 2.0f * dot(q.vector, v) +
           v * (q.scalar * q.scalar - dot(q.vector, q.vector)) +
           cross(q.vector, v) * 2.0f * q.scalar;
}

quat mix(const quat &from, const quat &to, float t) {
    return from * (1.0f - t) + to * t;
}

quat nlerp(const quat &from, const quat &to, float t) {
    return normalized(from * (1.0f - t) + to * t);
}

quat operator^(const quat &q, float f) {
    float angle = 2.0f * acosf(q.scalar);
    vec3 axis = normalized(q.vector);
    float halfCos = cosf(f * angle * 0.5f);
    float halfSin = sinf(f * angle * 0.5f);
    return quat(axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos);
}

quat slerp(const quat &start, const quat &end, float t) {
    if(fabsf(dot(start, end)) > 1.0f - QUAT_EPSILON) {
        return nlerp(start, end, t);
    }
    quat delta = inverse(start) * end;
    return  normalized((delta ^ t) * start);
}

quat lookRotation(const vec3 &direcion, const vec3 &up) {
	// Find orthonormal basis vectors
	vec3 f = normalized(direcion);
	vec3 u = normalized(up);
	vec3 r = cross(u, f);
	u = cross(f, r);

	// From world forward to object forward
	quat f2d = fromTo(Vec3(0, 0, 1), f);

	// what direction is the new object up?
	vec3 objectUp = f2d * Vec3(0, 1, 0);
	// From object up to desired up
	quat u2u = fromTo(objectUp, u);

	// Rotate to forward direction first, then twist to correct up
	quat result = f2d * u2u;
	// Don’t forget to normalize the result
	return normalized(result);
}

mat4 quatToMat4(const quat &q) {
    vec3 r = q * Vec3(1.0f, 0.0f, 0.0f);
    vec3 u = q * Vec3(0.0f, 1.0f, 0.0f);
    vec3 f = q * Vec3(0.0f, 0.0f, 1.0f);
    return     {r.x, r.y, r.z, 0,
                u.x, u.y, u.z, 0,
                f.x, f.y, f.z, 0,
                0,   0,   0,   1};
}

quat mat4ToQuat(const mat4 &m) {
    vec3 up = normalized(Vec3(m.up.x, m.up.y, m.up.z));
    vec3 forward = normalized(Vec3(m.forward.x, m.forward.y, m.forward.z));
    vec3 right = cross(up, forward);
    up = cross(forward, right);
    return lookRotation(forward, up);
}

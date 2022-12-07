#include "lh_transform_entity.h"
#include "lh_static_entity.h"
#include "lh_input.h"
#include "lh_platform.h"

/*
struct TranslateState {
    vec3 startPosition;
    vec3 offset;
    bool setOffset;
    bool moving;
};
*/

struct Camera {
    vec3 position;
    vec3 target;
    vec3 front;
    vec3 right;
    vec3 up;
    f32 pitch;
    f32 yaw;
    mat4 view;
    mat4 proj;
    f32 fov;
    i32 mouseDefaultScreenX;
    i32 mouseDefaultScreenY;
    f32 distance;
};

internal
Ray GetMouseRay(Camera *camera, f32 x, f32 y) {
    mat4 invView = inverse(camera->view);
    mat4 invProj = inverse(camera->proj);
    vec4 rayClip;
    rayClip.x = 2.0f * x / (f32)WindowGetWidth() - 1.0f;
    rayClip.y = 1.0f - (2.0f * y) / (f32)WindowGetHeight();
    rayClip.z = 1.0f;
    rayClip.w = 1.0f;
    vec4 rayEye = invProj * rayClip;
    rayEye.z = 1.0f;
    rayEye.w =  0.0f;
    vec4 rayWorld = invView * rayEye;
    normalize(&rayWorld);

    Ray ray;
    ray.o = camera->position;
    ray.d = {rayWorld.x, rayWorld.y, rayWorld.z};
    normalize(&ray.d);
    return ray;
}

internal
void UpdateEntity(StaticEntity *entity) {
    entity->obb = CreateOBB(entity->transform.position,
                            entity->transform.rotation,
                            entity->transform.scale);
    entity->world = TransformToMat4(entity->transform.position,
                                    entity->transform.rotation,
                                    entity->transform.scale);
}

global_variable TranslateState gTranslateState = {};
global_variable RotateState gRotateState = {};
global_variable ScaleState gScaleState = {};

void TranslateInitialize() {
    gTranslateState.moving = true;
    gTranslateState.setOffset = true;
}

void TranslateEntity(StaticEntity *entity, Camera *camera) {
    if(gTranslateState.moving) {
        Plane plane;
        plane.p = camera->position - entity->transform.position;
        plane.n = camera->front * -1.0f;
        Ray ray = GetMouseRay(camera, MouseGetCursorX(), MouseGetCursorY());
        f32 t = 0.0f;
        if(RaycastPlane(&plane, &ray, &t)) {
            vec3 hitPosition = ray.o + ray.d * t;
            
            if(gTranslateState.setOffset) {
                gTranslateState.startPosition = entity->transform.position;
                gTranslateState.offset = hitPosition - entity->transform.position;
                gTranslateState.setOffset = false;
            }

            vec3 oldP = gTranslateState.startPosition;
            vec3 newP = hitPosition - gTranslateState.offset;
            entity->transform.position = gTranslateState.startPosition + (newP - oldP);
            entity->transform.position.x = roundf(entity->transform.position.x);
            entity->transform.position.y = roundf(entity->transform.position.y);
            entity->transform.position.z = roundf(entity->transform.position.z);
            UpdateEntity(entity);
        }
    }
}

void TranslateEntityAxis(StaticEntity *entity, Camera *camera, i32 axis) {
    if(gTranslateState.moving) {
        Plane plane;
        plane.p = camera->position - entity->transform.position;
        plane.n = camera->front * -1.0f;
        Ray ray = GetMouseRay(camera, MouseGetCursorX(), MouseGetCursorY());
        f32 t = 0.0f;
        if(RaycastPlane(&plane, &ray, &t)) {
            vec3 hitPosition = ray.o + ray.d * t;
            
            if(gTranslateState.setOffset) {
                gTranslateState.startPosition = entity->transform.position;
                gTranslateState.offset = hitPosition - entity->transform.position;
                gTranslateState.setOffset = false;
            }
           
            vec3 oldP = gTranslateState.startPosition;
            vec3 newP = hitPosition;
            vec3 finalP = gTranslateState.startPosition + (newP - oldP);
            entity->transform.position = oldP;
            entity->transform.position.v[axis] = finalP.v[axis];
            entity->transform.position.v[axis] = roundf(entity->transform.position.v[axis]);
            UpdateEntity(entity);
        }
    }
}

void TranslateAccept(StaticEntity *entity) {
    if(gTranslateState.moving) {
        gTranslateState.moving = false;
        gTranslateState.setOffset = false;
    } 
}

void TranslateReject(StaticEntity *entity) {
    if(gTranslateState.moving) {
        entity->transform.position = gTranslateState.startPosition;
        UpdateEntity(entity);
        gTranslateState.moving = false;
        gTranslateState.setOffset = false;
    } 
}

bool TranslateGetState() {
    return gTranslateState.moving;
}

void RotateInitialize() {
    gRotateState.moving = true;
    gRotateState.setMouseToObject = true;
}

void RotateEntity(StaticEntity *entity, Camera *camera) {
    if(gRotateState.moving) {
        Plane plane;
        plane.p = camera->position - entity->transform.position;
        plane.n = camera->front * -1.0f;
        Ray ray = GetMouseRay(camera, MouseGetCursorX(), MouseGetCursorY());
        f32 t = 0.0f;
        if(RaycastPlane(&plane, &ray, &t)) {
            vec3 hitPosition = ray.o + ray.d * t;
            if(gRotateState.setMouseToObject) {
                gRotateState.startRotation = entity->transform.rotation;
                gRotateState.mouseToObject = normalized(hitPosition - entity->transform.position);
                gRotateState.setMouseToObject = false;
            }
            vec3 currentMouseToObject = normalized(hitPosition - entity->transform.position);
            entity->transform.rotation = gRotateState.startRotation * fromTo(gRotateState.mouseToObject, currentMouseToObject); 
            UpdateEntity(entity);
        }
    }
}

void RotateEntityAxis(StaticEntity *entity, Camera *camera, i32 axis) {
    if(gRotateState.moving) {
        Plane plane;
        plane.p = camera->position - entity->transform.position;
        plane.n = camera->front * -1.0f;
        Ray ray = GetMouseRay(camera, MouseGetCursorX(), MouseGetCursorY());
        f32 t = 0.0f;
        if(RaycastPlane(&plane, &ray, &t)) {
            vec3 hitPosition = ray.o + ray.d * t;
            
            if(gRotateState.setMouseToObject) {
                gRotateState.startRotation = entity->transform.rotation;
                gRotateState.mouseToObject = normalized(hitPosition - entity->transform.position);
                gRotateState.setMouseToObject = false;
            }
            vec3 currentMouseToObject = normalized(hitPosition - entity->transform.position);
            f32 angleSide = dot(normalized(cross(plane.n, gRotateState.mouseToObject)), currentMouseToObject);
            f32 angleBet = angle(gRotateState.mouseToObject, currentMouseToObject);
            if(angleSide < 0.0f) {
                angleBet = RAD(360.0f) - angleBet;
            }
            vec3 axisArray[3] = {
                {1, 0, 0},
                {0, 1, 0},
                {0, 0, 1}
            };
            if(dot(normalized(plane.p), axisArray[axis]) < 0.0f) {
                angleBet = -angleBet;
            }
            entity->transform.rotation = gRotateState.startRotation * angleAxis(angleBet, axisArray[axis]);
            UpdateEntity(entity);
        }
    }
}

void RotateAccept(StaticEntity *entity) {
    if(gRotateState.moving) {
        gRotateState.moving = false;
        gRotateState.setMouseToObject = false;
    } 
}

void RotateReject(StaticEntity *entity) {
    if(gRotateState.moving) {
        entity->transform.rotation = gRotateState.startRotation;
        UpdateEntity(entity);
        gRotateState.moving = false;
        gRotateState.setMouseToObject = false;
    }
}

bool RotateGetState() {
    return gRotateState.moving;
}

void ScaleInitialize() {
    gScaleState.moving = true;
    gScaleState.set = true;
}

void ScaleEntity(StaticEntity *entity, Camera *camera) {
    if(gScaleState.moving) {
        Plane plane;
        plane.p = camera->position - entity->transform.position;
        plane.n = camera->front * -1.0f;
        Ray ray = GetMouseRay(camera, MouseGetCursorX(), MouseGetCursorY());
        f32 t = 0.0f;
        if(RaycastPlane(&plane, &ray, &t)) {
            vec3 hitPosition = ray.o + ray.d * t;
            if(gScaleState.set) {
                gScaleState.startScale = entity->transform.scale;
                gScaleState.scaleVector = hitPosition - entity->transform.position;
                gScaleState.set = false;
            }
            vec3 currentMouseToObject = hitPosition - entity->transform.position;
            f32 ratio = (lenSq(currentMouseToObject) / lenSq(gScaleState.scaleVector));            
            if(ratio > 0.0f) {
                entity->transform.scale = gScaleState.startScale  * ratio;
                UpdateEntity(entity);
            }
        }
    }
}

void ScaleEntityAxis(StaticEntity *entity, Camera *camera, i32 axis) {
    if(gScaleState.moving) {
        Plane plane;
        plane.p = camera->position - entity->transform.position;
        plane.n = camera->front * -1.0f;
        Ray ray = GetMouseRay(camera, MouseGetCursorX(), MouseGetCursorY());
        f32 t = 0.0f;
        if(RaycastPlane(&plane, &ray, &t)) {
            vec3 hitPosition = ray.o + ray.d * t;
            if(gScaleState.set) {
                gScaleState.startScale = entity->transform.scale;
                gScaleState.scaleVector = hitPosition - entity->transform.position;
                gScaleState.set = false;
            }
            vec3 currentMouseToObject = hitPosition - entity->transform.position;
            f32 ratio = lenSq(currentMouseToObject) / lenSq(gScaleState.scaleVector);
            if(ratio > 0.0f) {
                entity->transform.scale = gScaleState.startScale;
                entity->transform.scale.v[axis] = gScaleState.startScale.v[axis] * ratio; 
            }
            UpdateEntity(entity);
        }
    }
}

void ScaleAccept(StaticEntity *entity) {
    if(gScaleState.moving) {
        gScaleState.moving = false;
        gScaleState.set = false;
    } 
}

void ScaleReject(StaticEntity *entity) {
    if(gScaleState.moving) {
        entity->transform.scale = gScaleState.startScale;
        UpdateEntity(entity);
        gScaleState.moving = false;
        gScaleState.set = false;
    }
}

bool ScaleGetState() {
    return gScaleState.moving;
}

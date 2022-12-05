#ifndef _LH_TRANSFORM_ENTITY_H_
#define _LH_TRANSFORM_ENTITY_H_

#include "lh_defines.h"
#include "lh_math.h"

struct StaticEntity;
struct Camera;

struct TranslateState {
    vec3 startPosition;
    vec3 offset;
    bool setOffset;
    bool moving;
};

struct RotateState {
    quat startRotation;
    vec3 mouseToObject;
    bool setMouseToObject;
    bool moving;
};

void TranslateInitialize();
void TranslateEntity(StaticEntity *entity, Camera *camera);
void TranslateAccept(StaticEntity *entity);
void TranslateReject(StaticEntity *entity);
bool TranslateGetState();

void RotateInitialize();
void RotateEntity(StaticEntity *entity, Camera *camera);
void RotateAccept(StaticEntity *entity);
void RotateReject(StaticEntity *entity);
bool RotateGetState();

void ScaleInitialize();
void ScaleEntity(StaticEntity *entity, Camera *camera);
void ScaleAccept(StaticEntity *entity);
void ScaleReject(StaticEntity *entity);
bool ScaleGetState();

#endif

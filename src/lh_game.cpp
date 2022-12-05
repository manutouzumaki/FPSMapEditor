#include "lh_game.h"
#include "lh_platform.h"
#include "lh_renderer.h"
#include "lh_input.h"
#include "lh_static_entity.h"
#include "lh_file.h"
#include "lh_transform_entity.h"

// TODO (Manuto):
/////////////////////////////////////////////////////////////
// - add a mesh loader  (OBJ parser)
/////////////////////////////////////////////////////////////

bool StringCompare(char *a, char *b, i32 size) {
    for(i32 i = 0; i < size; ++i) {
        if(a[i] != b[i]) return false;
    }
    return true;
}

#include <stdio.h>

internal
Mesh *LoadOBJFile(char *path, Arena *arena)
{
    ReadFileResult file = ReadFile(path);
    
    i32 verticesCount = 0; 
    i32 textureCoordsCount = 0;
    i32 normalsCount = 0;
    i32 indexCount = 0;

    // first we loop through the entire file and get the size of the mesh
    char *line = (char *)file.data;
    while(*line != '\0') { 
        if(StringCompare(line, "v ", 2)) {
            ++verticesCount;
        }
        if(StringCompare(line, "vt ", 3)) {
            ++textureCoordsCount; 
        }
        if(StringCompare(line, "vn ", 3)) {
            ++normalsCount;
        }
        if(StringCompare(line, "f ", 2)) {
            ++indexCount;
        }    
        
        i32 counter = 0;
        char *letter = line;
        while(*letter != '\n') {
            ++counter;
            ++letter;
        }
        line += counter + 1;
    }

    // get the memory
    f32 *tempVertices = (f32 *)malloc((verticesCount*3)*sizeof(f32));
    f32 *tempTextureCoords = (f32 *)malloc((textureCoordsCount*2)*sizeof(f32));
    f32 *tempNormals = (f32 *)malloc((normalsCount*3)*sizeof(f32));
    i32 *tempVIndex =  (i32 *)malloc((indexCount*3)*sizeof(i32));
    i32 *tempTIndex =  (i32 *)malloc((indexCount*3)*sizeof(i32));
    i32 *tempNIndex =  (i32 *)malloc((indexCount*3)*sizeof(i32));
    
    // second loop through the file to ge the values 
    f32 *verticesPtr = tempVertices;
    f32 *textureCoordsPtr = tempTextureCoords;
    f32 *normalsPtr = tempNormals;
    i32 *vIndexPtr = tempVIndex;
    i32 *tIndexPtr = tempTIndex;
    i32 *nIndexPtr = tempNIndex;
    line = (char *)file.data;
    while(*line != '\0') {   
        if(StringCompare(line, "v ", 2)) {
            f32 X, Y, Z;
            sscanf(line, "v %f %f %f", &X, &Y, &Z);
            *verticesPtr++ = X;
            *verticesPtr++ = Y;
            *verticesPtr++ = Z;
        }
        if(StringCompare(line, "vt ", 3)) {
            f32 U, V;
            sscanf(line, "vt %f %f", &U, &V);
            *textureCoordsPtr++ = U;
            *textureCoordsPtr++ = V;
        }
        if(StringCompare(line, "vn ", 3)) {
            f32 X, Y, Z;
            sscanf(line, "vn %f %f %f", &X, &Y, &Z);
            *normalsPtr++ = X;
            *normalsPtr++ = Y;
            *normalsPtr++ = Z;
        }
        if(StringCompare(line, "f ", 2)) {
            i32 vIndex[3];
            i32 tIndex[3];
            i32 nIndex[3];
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                   &vIndex[0], &tIndex[0], &nIndex[0],
                   &vIndex[1], &tIndex[1], &nIndex[1],
                   &vIndex[2], &tIndex[2], &nIndex[2]);
            *vIndexPtr++ = vIndex[0] - 1;
            *vIndexPtr++ = vIndex[1] - 1;
            *vIndexPtr++ = vIndex[2] - 1;
            *tIndexPtr++ = tIndex[0] - 1;
            *tIndexPtr++ = tIndex[1] - 1;
            *tIndexPtr++ = tIndex[2] - 1;
            *nIndexPtr++ = nIndex[0] - 1;
            *nIndexPtr++ = nIndex[1] - 1;
            *nIndexPtr++ = nIndex[2] - 1;
        }  

        i32 counter = 0;
        char *letter = line;
        while(*letter != '\n') {
            ++counter;
            ++letter;
        }
        line += counter + 1;
    }

    // this is store in the permanent storage of the program  
    f32 *verticesArray = (f32 *)malloc((3*indexCount)*(sizeof(f32)*8));
    f32 *vertexBufferPtr = verticesArray;
    vIndexPtr = tempVIndex;
    tIndexPtr = tempTIndex;
    nIndexPtr = tempNIndex;
    for(i32 index = 0;
        index < 3*indexCount;
        ++index)
    {
        i32 vIndex = *vIndexPtr * 3;
        *vertexBufferPtr++ = tempVertices[vIndex + 0];
        *vertexBufferPtr++ = tempVertices[vIndex + 1];
        *vertexBufferPtr++ = tempVertices[vIndex + 2];
        ++vIndexPtr;
        i32 tIndex = *tIndexPtr * 2;
        *vertexBufferPtr++ = tempTextureCoords[tIndex + 0];
        *vertexBufferPtr++ = tempTextureCoords[tIndex + 1];
        ++tIndexPtr;
        i32 nIndex = *nIndexPtr * 3;
        *vertexBufferPtr++ = tempNormals[nIndex + 0];
        *vertexBufferPtr++ = tempNormals[nIndex + 1];
        *vertexBufferPtr++ = tempNormals[nIndex + 2];
        ++nIndexPtr;
    }
 
    Vertex *vertices = (Vertex *)malloc(3*indexCount * sizeof(Vertex));
    u32 *indices = (u32 *)ArenaPushArray(arena, indexCount*3, u32);
    u32 indicesCount = indexCount * 3;
    
    Vertex *vertexArray = (Vertex *)verticesArray;
    i32 vertexAddedCount = 0;
    for(i32 i = 0; i < indexCount * 3; ++i) {
        Vertex vertex = vertexArray[i];
        if(vertexAddedCount > 0) {
            bool flag = true;
            for(i32 j = 0; j < vertexAddedCount; ++j) {
                if(vertex.position == vertices[j].position &&
                   vertex.uv == vertices[j].uv &&
                   vertex.normal == vertices[j].normal) {
                    indices[i] = j;
                    flag = false; 
                    break;
                }
            }
            if(flag) {
                vertices[vertexAddedCount] = vertex;
                indices[i] = vertexAddedCount;
                vertexAddedCount++;
            } 
        }
        else {
            vertices[vertexAddedCount] = vertex;
            indices[i] = vertexAddedCount;
            vertexAddedCount++;
        }
    }
    verticesCount = vertexAddedCount;
    Vertex *uniqueVretices = (Vertex *)ArenaPushArray(arena, verticesCount, Vertex);
    memcpy(uniqueVretices, vertices, sizeof(Vertex)*verticesCount);

    free(vertices);
    free(verticesArray);
    free(tempVertices);
    free(tempTextureCoords);
    free(tempNormals);
    free(tempVIndex);
    free(tempTIndex);
    free(tempNIndex);
    free(file.data);

    Mesh *result = RendererCreateMesh(uniqueVretices, verticesCount, indices, indicesCount, arena); 
    return result;
}


global_variable Vertex verticesCube[] = {
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,

         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,

         1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
        
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,-1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,-1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,-1.0f,  0.0f,  0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,-1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,-1.0f,  0.0f,  0.0f,

         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,

         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,

         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,

         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
};

global_variable Vertex verticesCube2[] = {
    -1.0f,  1.0f, -1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     1.0f,  1.0f, -1.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     1.0f, -1.0f, -1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,

    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    -1.0f, -1.0f, -1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    -1.0f,  1.0f, -1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,

     1.0f, -1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
     1.0f, -1.0f, -1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
     1.0f,  1.0f, -1.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
     1.0f,  1.0f,  1.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,

    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     1.0f, -1.0f, -1.0f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     1.0f,  1.0f, -1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,

    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f
};

global_variable u32 indicesCube2[] =
{
    3,1,0,2,1,3,
    6,4,5,7,4,6,
    11,9,8, 10,9, 11,
    14, 12, 13, 15, 12, 14,
    19, 17, 16, 18, 17, 19,
    22, 20, 21, 23, 20, 22
};


global_variable Vertex vertices[] = {
    // position           // uv        // normal
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f
};

global_variable u32 indices[] = {
    0, 1, 3,
    3, 1, 2
};

internal 
StaticEntity *AddStaticEntityToArray(vec3 position, Mesh *mesh, GameState *gameState, Arena *arena) {
    StaticEntity *entity = ArenaPushStruct(arena, StaticEntity);
    gameState->entitiesCount++;
    entity->mesh = mesh;
    entity->bitmap = gameState->bitmaps[0];
    entity->transform.position = position;
    entity->transform.rotation = quat();
    entity->transform.scale = {1, 1, 1};
    entity->obb = CreateOBB(entity->transform.position,
                            entity->transform.rotation,
                            entity->transform.scale);
    entity->world = TransformToMat4(entity->transform.position,
                                    entity->transform.rotation,
                                    entity->transform.scale);
    gameState->entities = entity;
    return entity;
}

internal
void DrawStaticEntityArray(StaticEntity *entities, i32 count,
                           vec3 *lightsToRendere, i32 lightsToRenderCount,
                           GameState *gameState) {
    for(i32 i = 0; i < count; ++i) {
        StaticEntity *staticEntity = entities + i;
        Mesh *mesh = staticEntity->mesh;
        f32 repeatV = 1.0f;//staticEntity->transform.scale.y;
        f32 repeatU = 1.0f;//staticEntity->transform.scale.x > staticEntity->transform.scale.z ? staticEntity->transform.scale.x : staticEntity->transform.scale.z;
        RendererDrawMesh(mesh, staticEntity->world, staticEntity->bitmap, lightsToRendere, lightsToRenderCount,
                         gameState->camera.position, true, repeatU, repeatV,
                         &gameState->constBuffer, gameState->shader);
    }
}

internal
void DrawSkybox(vec3 cameraPosition, GameState *gameState) {
    mat4 world;
    Mesh *mesh = gameState->meshes[1];
    // FRONT
    world = Mat4Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z + 1.0f);
    RendererDrawMesh(mesh, world, gameState->skybox[2], NULL, 0, cameraPosition, false, 1, 1,
                     &gameState->constBuffer, gameState->skyboxShader);
// BACK
    world = Mat4Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z - 1.0f) * Mat4RotateY(RAD(180.0f));
    RendererDrawMesh(mesh, world, gameState->skybox[3], NULL, 0, cameraPosition, false, 1, 1,
                     &gameState->constBuffer, gameState->skyboxShader);


// LEFT
    world = Mat4Translate(cameraPosition.x - 1, cameraPosition.y, cameraPosition.z) * Mat4RotateY(RAD(90.0f));
    RendererDrawMesh(mesh, world, gameState->skybox[5], NULL, 0, cameraPosition, false, 1, 1,
                     &gameState->constBuffer, gameState->skyboxShader);

// RIGHT
    world = Mat4Translate(cameraPosition.x + 1, cameraPosition.y, cameraPosition.z) * Mat4RotateY(RAD(-90.0f));
    RendererDrawMesh(mesh, world, gameState->skybox[4], NULL, 0, cameraPosition, false, 1, 1,
                     &gameState->constBuffer, gameState->skyboxShader);

// UP
    world = Mat4Translate(cameraPosition.x, cameraPosition.y + 1, cameraPosition.z) * Mat4RotateX(RAD(-90.0f)) * Mat4RotateZ(RAD(-90.0f));
    RendererDrawMesh(mesh, world, gameState->skybox[0], NULL, 0, cameraPosition, false, 1, 1,
                     &gameState->constBuffer, gameState->skyboxShader);
}

internal
void UpdateCamera(Camera *camera) {
    if(MouseGetButtonJustDown(MOUSE_BUTTON_RIGHT)) {
        MouseShowCursor(false);
        camera->mouseDefaultScreenX = MouseGetScreenX();
        camera->mouseDefaultScreenY = MouseGetScreenY();
    }
    if(MouseGetButtonJustUp(MOUSE_BUTTON_RIGHT)) {
        MouseShowCursor(true);
    }
    if(MouseGetButtonDown(MOUSE_BUTTON_RIGHT)) {
        f32 deltaMouseX = (f32)(MouseGetScreenX() - camera->mouseDefaultScreenX);
        f32 deltaMouseY = (f32)(MouseGetScreenY() - camera->mouseDefaultScreenY);
        MouseSetCursor(camera->mouseDefaultScreenX, camera->mouseDefaultScreenY);

        if(KeyboardGetKeyDown(KEYBOARD_KEY_SHIFT)) {
            camera->target = camera->target - camera->right * (deltaMouseX * 0.01); 
            camera->target = camera->target + camera->up    * (deltaMouseY * 0.01); 
        }
        else {
            camera->yaw   -= (deltaMouseX * 0.001f);
            camera->pitch += (deltaMouseY * 0.001f); 
        }
    }
    f32 maxPitch = RAD(89.0f);
    if(camera->pitch > maxPitch) {
       camera->pitch = maxPitch;
    }
    else if(camera->pitch < -maxPitch) {
            camera->pitch = -maxPitch;
    }

    vec3 relPosition = camera->target - camera->position;
    f32 lenght = len(relPosition);
    relPosition.x = cosf(camera->yaw) * cosf(camera->pitch);
    relPosition.y = sinf(camera->pitch);
    relPosition.z = sinf(camera->yaw) * cosf(camera->pitch);
    relPosition = normalized(relPosition) * lenght;
    camera->position = camera->target + relPosition;
    camera->front = normalized(camera->target - camera->position);
    camera->right = normalized(cross({0, 1, 0}, camera->front));
    camera->up =  normalized(cross(camera->front, camera->right));

    camera->distance -= (f32)MouseGetWheel();
    if(camera->distance <= 2.0f) {
        camera->distance = 2.0f;
    }
    if(camera->distance >= 20.0f) {
        camera->distance = 20.0f;
    }
    camera->position = camera->target - camera->front * camera->distance;

    camera->view = Mat4LookAt(camera->position,
                              camera->position + camera->front,
                              {0, 1, 0});

}

internal
Ray GetMouseRay(GameState *gameState, f32 x, f32 y) {
    mat4 invView = inverse(gameState->camera.view);
    mat4 invProj = inverse(gameState->camera.proj);
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
    ray.o = gameState->camera.position;
    ray.d = {rayWorld.x, rayWorld.y, rayWorld.z};
    normalize(&ray.d);
    return ray;
}


// TODO: implement this well
enum EditorState {
    NONE_STATE,
    TRANSLATE_STATE,
    ROTATE_STATE,
    SCALE_STATE
};

enum AxisState {
    AXIS_X,
    AXIS_Y,
    AXIS_Z,
    AXIS_NONE
};

void AddEntityCallback(void *dataA, void *dataB) {
    GameState *gameState = (GameState *)dataA;
    AddStaticEntityToArray({0, 0, -3}, gameState->meshes[0], gameState, &gameState->staticEntitiesArena);
    gameState->entities = gameState->entities - (gameState->entitiesCount - 1);
}

void EntityChangeTexture(void *dataA, void *dataB) {
    GameState *gameState = (GameState *)dataA;
    Texture *texture = (Texture *)dataB;
    gameState->currentTexture = texture;
}

void EntityChangeMesh(void *dataA, void *dataB) {
    GameState *gameState = (GameState *)dataA;
    Mesh *mesh = (Mesh *)dataB;
    gameState->currentMesh = mesh;
}

global_variable EditorState editorState = NONE_STATE;
global_variable AxisState axisState = AXIS_NONE;

internal
void UpdateEntities(GameState *gameState) {
    if(MouseGetButtonJustDown(MOUSE_BUTTON_LEFT) &&
       !TranslateGetState() &&
       !RotateGetState() &&
       !ScaleGetState()) {
        f32 tMin = FLT_MAX;
        for(i32 i = 0; i < gameState->entitiesCount; ++i) { 
            StaticEntity *entity = gameState->entities + i;
            Ray ray = GetMouseRay(gameState, MouseGetCursorX(), MouseGetCursorY());
            f32 t = 0.0f;
            if(RaycastOBB(&entity->obb, &ray, &t)) {
                if(t < tMin) {
                    tMin = t;
                    Plane plane;
                    plane.p = gameState->camera.position - entity->transform.position;
                    plane.n = gameState->camera.front * -1.0f;
                    if(RaycastPlane(&plane, &ray, &t)) {
                        gameState->currentEntity = entity;
                    }
                }
            }
        }
        if(gameState->currentEntity) {
            if(gameState->currentTexture) {
                gameState->currentEntity->bitmap = gameState->currentTexture;
                gameState->currentTexture = NULL;
            }
            if(gameState->currentMesh) {
                gameState->currentEntity->mesh = gameState->currentMesh;
                gameState->currentMesh = NULL;
            }
        }
    }

    if(gameState->currentEntity) {
        StaticEntity *entity = gameState->currentEntity;
        if(editorState == TRANSLATE_STATE) {
            TranslateEntity(gameState->currentEntity, &gameState->camera);
            if(MouseGetButtonJustDown(MOUSE_BUTTON_LEFT)) {
                TranslateAccept(gameState->currentEntity);
            }
            if(MouseGetButtonJustDown(MOUSE_BUTTON_RIGHT)) {
                TranslateReject(gameState->currentEntity);    
            }
        }
        if(editorState == ROTATE_STATE) {
            RotateEntity(gameState->currentEntity, &gameState->camera);
            if(MouseGetButtonJustDown(MOUSE_BUTTON_LEFT)) {
                RotateAccept(gameState->currentEntity);
            }
            if(MouseGetButtonJustDown(MOUSE_BUTTON_RIGHT)) {
                RotateReject(gameState->currentEntity);    
            }
        }
        if(editorState == SCALE_STATE) {
            ScaleEntity(gameState->currentEntity, &gameState->camera);
            if(MouseGetButtonJustDown(MOUSE_BUTTON_LEFT)) {
                ScaleAccept(gameState->currentEntity);
            }
            if(MouseGetButtonJustDown(MOUSE_BUTTON_RIGHT)) {
                ScaleReject(gameState->currentEntity);    
            }
        }
        if(editorState == NONE_STATE) {
            TranslateReject(gameState->currentEntity);
            RotateReject(gameState->currentEntity);
            ScaleReject(gameState->currentEntity);

        }

#if 0
        if(MouseGetButtonDown(MOUSE_BUTTON_LEFT) &&
          !MouseGetButtonDown(MOUSE_BUTTON_RIGHT)) {
            StaticEntity *entity = gameState->currentEntity;
            // TODO: IMPORTANT improve this
            if(editorState == TRANSLATE_STATE) {
                Plane plane;
                plane.p = gameState->camera.position - entity->transform.position;
                plane.n = gameState->camera.front * -1.0f;
                Ray ray = GetMouseRay(gameState, MouseGetCursorX(), MouseGetCursorY());
                f32 t = 0.0f;
                if(RaycastPlane(&plane, &ray, &t)) {
                    vec3 hitPositiom = ray.o + ray.d * t;
                    vec3 offset = hitPositiom - gameState->clickMousePosition;
                    gameState->clickMousePosition = hitPositiom;
                    if(axisState == AXIS_NONE) {
                        entity->transform.position = entity->transform.position + offset;
                    }
                    else {
                        entity->transform.position.v[axisState] += offset.v[axisState];
                    }
                    entity->obb = CreateOBB(entity->transform.position,
                                            entity->transform.rotation,
                                            entity->transform.scale);
                    entity->world = TransformToMat4(entity->transform.position,
                                                    entity->transform.rotation,
                                                    entity->transform.scale);
                }
            }
            else if(editorState == ROTATE_STATE) { 
                Plane plane;
                plane.p = gameState->camera.position - entity->transform.position;
                plane.n = gameState->camera.front * -1.0f;
                Ray ray = GetMouseRay(gameState, MouseGetCursorX(), MouseGetCursorY());
                f32 t = 0.0f;
                if(RaycastPlane(&plane, &ray, &t)) {
                    vec3 hitPositiom = ray.o + ray.d * t;
                    vec3 offset = hitPositiom - gameState->clickMousePosition;
                    gameState->clickMousePosition = hitPositiom;

                    entity->transform.rotation.v[axisState] += offset.v[axisState] * 10.0f;
                    
                    entity->obb = CreateOBB(entity->transform.position,
                                            entity->transform.rotation,
                                            entity->transform.scale);
                    entity->world = TransformToMat4(entity->transform.position,
                                                    entity->transform.rotation,
                                                    entity->transform.scale);
                }

            }
            else if(editorState == SCALE_STATE) {
                Plane plane;
                plane.p = gameState->camera.position - entity->transform.position;
                plane.n = gameState->camera.front * -1.0f;
                Ray ray = GetMouseRay(gameState, MouseGetCursorX(), MouseGetCursorY());
                f32 t = 0.0f;
                if(RaycastPlane(&plane, &ray, &t)) {
                    vec3 hitPositiom = ray.o + ray.d * t;
                    vec3 offset = hitPositiom - gameState->clickMousePosition;
                    gameState->clickMousePosition = hitPositiom;

        
                    entity->transform.scale.v[axisState] += offset.v[axisState];
                    
                    entity->obb = CreateOBB(entity->transform.position,
                                            entity->transform.rotation,
                                            entity->transform.scale);
                    entity->world = TransformToMat4(entity->transform.position,
                                                    entity->transform.rotation,
                                                    entity->transform.scale);
                }
            }
        }
#endif
    }
}

void GameInit(Memory *memory) {
    // The GameState has to be the first element on the memory
    ASSERT(memory->used + sizeof(GameState) <= memory->size);
    GameState *gameState = (GameState *)memory->data;
    memory->used += sizeof(GameState);

    WindowSystemInitialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Editor");
    RendererSystemInitialize();
    
    gameState->dataArena = ArenaCreate(memory, Megabytes(500));
    gameState->frameArena = ArenaCreate(memory, Megabytes(10));
    gameState->textureArena = ArenaCreate(memory, Megabytes(1));
    gameState->soundArena = ArenaCreate(memory, Megabytes(1));
    gameState->staticEntitiesArena = ArenaCreate(memory, Megabytes(1));

    // Load Assets
    gameState->bitmaps[0] = RendererCreateTexture("../assets/test.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->bitmaps[1] = RendererCreateTexture("../assets/grass.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->bitmaps[2] = RendererCreateTexture("../assets/hand.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->bitmaps[3] = RendererCreateTexture("../assets/crosshair.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->bitmaps[4] = RendererCreateTexture("../assets/shootSpritesheet.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->bitmaps[5] = RendererCreateTexture("../assets/enemy.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->bitmaps[6] = RendererCreateTexture("../assets/enemyHit.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->bitmaps[7] = RendererCreateTexture("../assets/add_button.bmp", &gameState->textureArena, &gameState->dataArena);
    
    gameState->skybox[0] = RendererCreateTexture("../assets/skyUp.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->skybox[1] = RendererCreateTexture("../assets/skyDown.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->skybox[2] = RendererCreateTexture("../assets/skyFront.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->skybox[3] = RendererCreateTexture("../assets/skyBack.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->skybox[4] = RendererCreateTexture("../assets/skyLeft.bmp", &gameState->textureArena, &gameState->dataArena);
    gameState->skybox[5] = RendererCreateTexture("../assets/skyRight.bmp", &gameState->textureArena, &gameState->dataArena);
    
    gameState->shader = RendererCreateShader("../src/shaders/vertex.hlsl", 
                                             "../src/shaders/pixel.hlsl",
                                             &gameState->dataArena);
    gameState->skyboxShader = RendererCreateShader("../src/shaders/vertex.hlsl", 
                                                   "../src/shaders/skybox.hlsl",
                                                   &gameState->dataArena);

    gameState->meshes[0] = LoadOBJFile("../assets/cube1.obj", &gameState->dataArena);
    gameState->meshes[1] = RendererCreateMesh(vertices, ARRAY_LENGTH(vertices), indices, ARRAY_LENGTH(indices), &gameState->dataArena);
    gameState->meshes[2] = LoadOBJFile("../assets/mon.obj", &gameState->dataArena);
    gameState->meshes[3] = LoadOBJFile("../assets/quad.obj", &gameState->dataArena);
    gameState->meshesCount = 4;
    //gameState->[1] = RendererCreateMesh(vertices, ARRAY_LENGTH(vertices), indices, ARRAY_LENGTH(indices), &gameState->dataArena); 
    //gameState->cubeMesh = RendererCreateMesh(verticesCube2, ARRAY_LENGTH(verticesCube2), indicesCube2, ARRAY_LENGTH(indicesCube2), &gameState->dataArena);



    gameState->camera.position = {0, 0, 1.0f};
    gameState->camera.target = {0, 0, 0};
    gameState->camera.front = {0, 0, 1};
    gameState->camera.up    = {0, 1, 0};
    gameState->camera.right = {1, 0, 0};
    gameState->camera.pitch = 0.0f;
    gameState->camera.yaw = RAD(90.0f);
    gameState->camera.fov = 60.0f;
    gameState->camera.proj = Mat4Perspective(gameState->camera.fov, (f32)WINDOW_WIDTH/(f32)WINDOW_HEIGHT, 0.01f, 100.0f);
    gameState->camera.view = Mat4LookAt(gameState->camera.position,
                                        gameState->camera.position + gameState->camera.front,
                                        gameState->camera.up);
    RendererSetProj(gameState->camera.proj);
    RendererSetView(gameState->camera.view);
    gameState->constBuffer.proj = gameState->camera.proj;
    gameState->constBuffer.view = gameState->camera.view;
    RendererUpdateShaderData(gameState->shader, &gameState->constBuffer);    

    // TODO initialize buttons
    gameState->buttons[0] = {
        0, 0, 100, 100, 
        false, AddEntityCallback,
        gameState->bitmaps[7]
    };
    gameState->buttonCount++;

    for(i32 i = 0; i < 7; ++i) {
        gameState->buttons[i + 1] = {
            (i * 50) + 100, 0, 50, 50,
            false, EntityChangeTexture,
            gameState->bitmaps[i],
            gameState->bitmaps[i]
        };
        gameState->buttonCount++;
    }

    for(i32 i = 0; i < 4; ++i) {
        gameState->buttons[8 + i] = {
            (i * 50) + 100, 50, 50, 50,
            false, EntityChangeMesh,
            gameState->bitmaps[0],
            gameState->meshes[i]
        };
        gameState->buttonCount++;
    }


    gameState->currentEntity = NULL;
    gameState->currentTexture = NULL;
    gameState->currentMesh = NULL;
}

void GameUpdate(Memory *memory, f32 dt) {
    GameState *gameState = (GameState *)memory->data;
    if(KeyboardGetKeyJustDown(KEYBOARD_KEY_1) || JoysickGetButtonJustDown(JOYSTICK_BUTTON_B)) {
        RendererType type = GetRendererType();
        if(type == RENDERER_DIRECTX) {
            RendererSetMode(RENDERER_CPU);
        }
        if(type == RENDERER_CPU) {
            RendererSetMode(RENDERER_DIRECTX);
        }
    }
    
    // this is for the editor mouse controls
    if(KeyboardGetKeyJustDown(KEYBOARD_KEY_T)) {
        TranslateInitialize();
        editorState = TRANSLATE_STATE;
    }
    if(KeyboardGetKeyJustDown(KEYBOARD_KEY_R)) {
        RotateInitialize();
        editorState = ROTATE_STATE;
    }
    if(KeyboardGetKeyJustDown(KEYBOARD_KEY_S)) {
        ScaleInitialize();
        editorState = SCALE_STATE;
    }
    if(KeyboardGetKeyJustDown(KEYBOARD_KEY_ESCAPE)) { 
        editorState = NONE_STATE;
        axisState = AXIS_NONE;
    }

    if(KeyboardGetKeyJustDown(KEYBOARD_KEY_X)) {
        axisState = AXIS_X; 
    }
    if(KeyboardGetKeyJustDown(KEYBOARD_KEY_Y)) {
        axisState = AXIS_Y; 
    }
    if(KeyboardGetKeyJustDown(KEYBOARD_KEY_Z)) {
        axisState = AXIS_Z; 
    }

    // update buttons
    bool flag = true;
    for(i32 i = 0; i < gameState->buttonCount; ++i) {
        Button *button =  gameState->buttons + i;
        f32 ratioX = WindowGetWidth() / (f32)WINDOW_WIDTH;
        f32 ratioY = WindowGetHeight() / (f32)WINDOW_HEIGHT;
        i32 minX = button->x * ratioX;
        i32 maxX = (button->x + button->w) * ratioX;  
        i32 minY = button->y * ratioY;
        i32 maxY = (button->y + button->h) * ratioY;  
        i32 mouseX = MouseGetCursorX();
        i32 mouseY = WindowGetHeight() - MouseGetCursorY();
        if(mouseX >= minX && mouseX <= maxX &&
           mouseY >= minY && mouseY <= maxY) {
            button->hot = true;
            flag = false;
        }
        else {
            button->hot = false;
        }

        if(MouseGetButtonJustDown(MOUSE_BUTTON_LEFT) && button->hot) {
            if(button->callback) {
                button->callback((void *)gameState, button->data);
            }
        }
    }

    UpdateCamera(&gameState->camera);

    if(flag ) UpdateEntities(gameState);
}

void GameRender(Memory *memory) {
    GameState *gameState = (GameState *)memory->data;
    
    RendererClearBuffers(0xFFFFFFEE, 0.0f);
    
    RendererSetView(gameState->camera.view);
    gameState->constBuffer.view = gameState->camera.view;
    RendererUpdateShaderData(gameState->shader, &gameState->constBuffer);
    RendererUpdateShaderData(gameState->skyboxShader, &gameState->constBuffer);

    // SKYBOX
    DrawSkybox(gameState->camera.position, gameState);
    RendererFlushWorkQueue();
   
    DrawStaticEntityArray(gameState->entities, gameState->entitiesCount, NULL, 1, gameState); 
    RendererFlushWorkQueue();
    
    for(i32 i = 0; i < gameState->buttonCount; ++i) {
        Button *button =  gameState->buttons + i;
        RendererDrawRect(button->x, button->y, button->w, button->h, button->texture);
    }
    RendererPresent();
    ArenaReset(&gameState->frameArena);
}

void GameShutdown(Memory * memory) {
    GameState *gameState = (GameState *)memory->data;
    RendererSystemShutdown();
    WindowSystemShutdown();
}

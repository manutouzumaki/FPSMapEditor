#ifndef _LH_GAME_H_
#define _LH_GAME_H_

#include "lh_defines.h"
#include "lh_memory.h"
#include "lh_math.h"
#include "lh_renderer.h"

struct Texture;
struct StaticEntity;
struct OctreeNode;

struct Mesh;
struct Shader;
struct Button;

typedef void ButtonCallBack(void *dataA, void *dataB);
struct Button {
    i32 x, y, w, h;
    bool hot;
    ButtonCallBack *callback;
    Texture *texture;
    void *data;
};

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

struct GameState {
    Arena dataArena;
    Arena frameArena;
    Arena textureArena; 
    Arena soundArena;
    Arena staticEntitiesArena;
    
    Texture *bitmaps[8];
    Texture *skybox[6];
    
    Camera camera;

    StaticEntity *entities;
    i32 entitiesCount;

    StaticEntity *currentEntity;
    Texture *currentTexture;
    Mesh *currentMesh;
    vec3 clickMousePosition;

    // TODO: test directx renderer
    //Mesh *cubeMesh;
    //Mesh *quadMesh;
    Mesh *meshes[4];
    i32 meshesCount;
    
    
    Shader *shader;
    Shader *skyboxShader;
    ConstantBuffer constBuffer;


    Button buttons[20];
    i32 buttonCount;
};

void GameInit(Memory *memory);
void GameUpdate(Memory *memory, f32 dt);
void GameRender(Memory *memory);
void GameShutdown(Memory *memory);

#endif

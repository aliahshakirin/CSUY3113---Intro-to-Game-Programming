#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
#include <SDL_mixer.h>


enum EntityType {NONE, PLAYER, PLATFORM, ENEMY, ENTRANCE1, ENTRANCE2, ENTRANCE3, ENTRANCE4, EXIT,
    KEY, BOSS_KEY, BOSS_DOOR, FISHING_ROD, POND, RING};
enum AIType {WALKERY, WALKERX, WAITANDGO};
enum State { IDLE, WALKING, ATTACKING};

class Entity {
public:
    EntityType entityType;
    
    EntityType lastCollision;
    AIType aiType;
    State state;
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    glm::vec3 scale = glm::vec3(1,1,0);
    glm::vec3 ring_pos;
    float speed;
    int *level;
    bool key = 0;
    bool boss_key = 0;
    bool mystery = 0;
    bool missing_object = 0;
    bool missing_object_appear = 0;
    Mix_Chunk *player_die = Mix_LoadWAV("player_die.wav");
    Mix_Chunk *monster_die = Mix_LoadWAV("monster_die.wav");
    Mix_Chunk *collect = Mix_LoadWAV("collect.wav");
    
    float width = 1;
    float height = 1;
    
    bool jump = false;
    float jumpPower = 0;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUpL = NULL;
    int *animUpR = NULL;
    
    int *animAttackR = NULL;
    int *animAttackL = NULL;
    int *animAttackUR = NULL;
    int *animAttackUL = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    
    bool isActive = true;
    
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    Entity();

    bool CheckCollision(Entity *other);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsX(Map *map);
    void CheckCollisionsY(Map *map);
    
    void Update(float deltaTime, Entity *player, Entity *enemies, int enemyCount, Entity *objects, int objectCount, Map *map);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    
    void AI(Entity *player);
    void AIWalkerY();
    void AIWalkerX();
    void AIWaitAndGo(Entity *player);

};

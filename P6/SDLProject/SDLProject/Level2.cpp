#include "Level2.h"

#define LEVEL2_WIDTH 20
#define LEVEL2_HEIGHT 16

#define LEVEL2_ENEMY_COUNT 6

#define LEVEL2_OBJECT_COUNT 3

// 42 empty brick
// pond 0,12,10, 27, 27, 29, 40, 44, 45

unsigned int level2_data[] = {
    78,    78,    78,    78,    78,    78,    78,    78,    20,     1,    2,    3,    4,    1,    2,    3,    4,    3,    4,    5,
    78,    78,    78,    78,    78,    78,    78,    78,    30,     7,    7,    7,    7,    7,    7,    7,    7,    7,    7,    25,
    10,    1,     2,     3,     5,    78,    78,    78,    20,     7,    7,    7,    7,    7,    7,    7,     7,    7,    7,    35,
    20,    7,     7,     7,    15,    78,    78,    78,    10,     7,    7,    7,    54,   41,   42,   43,    44,    41,    42,    45,
    10,    7,     7,     7,     1,     3,    4,     3,     1,     7,     7,    7,    25,   78,   78,   78,    78,    78,    78,    78,
    30,    7,     7,     7,     7,     7,    7,     7,     7,     7,     7,    7,    35,   78,   78,   78,    78,    78,    78,    78,
    20,    7,     7,     7,     7,     7,    7,     7,     7,      7,     7,   7,    15,   78,   78,   78,    78,    78,    78,    78,
    10,     7,     7,     7,    50,    42,   43,    44,     53,     7,     7,   7,    25,   78,   78,   78,   78,     78,     78,   78,
    40,    41,    42,     44,    45,    78,   78,    78,     30,     7,     7,   7,   35,   78,   78,   78,   78,     78,     78,    78,
    78,    78,    78,    78,     10,     1,    2,     3,     4,     7,     7,   7,    1,    2,    3,    4,    15,    78,    78,    78,
    78,    78,    78,    78,     10,     7,    7,     7,     7,     7,     7,   7,    7,    7,    7,    7,    15,    78,    78,    78,
    78,    78,    78,    78,     20,     7,    7,     7,     7,     7,     7,   7,    7,    7,    7,    7,    25,    78,    78,    78,
    78,    78,    78,    78,     30,     7,    7,    7,     7,      7,     7,   7,    7,    7,    7,    7,    35,    78,    78,    78,
    78,    78,    78,    78,     40,     41,  42,    43,    44,     53,     7,   54,   41,   42,   43,    44,  45,    78,    78,    78,
    78,    78,    78,    78,     78,     78,  78,    78,    78,     20,     7,   25,    78,   78,   78,   78,   78,    78,    78,    78,
    78,    78,    78,    78,     78,     78,  78,    78,    78,     40,     42,   45,    78,   78,   78,   78,   78,    78,    78,    78,
    
};

Level2::Level2(int *lives) {
    l2_level = lives;
}

void Level2::Initialize() {
    
    state.nextScene = -1;
    state.mode = GAME_LEVEL;
    
    GLuint mapTextureID = Util::LoadTexture("Dungeon_Tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 10, 10, DUNGEON);
    
    // Initialize Game Objects
    
    // 0-1 is exit
    state.objects = new Entity[LEVEL2_OBJECT_COUNT];
    
    state.objects[0].textureID = Util::LoadTexture("sideexit1.png");
    state.objects[0].entityType = EXIT;
    state.objects[0].width = 0.5;
    state.objects[0].height = 0.5;
    state.objects[1].textureID = Util::LoadTexture("sideexit2.png");
    state.objects[1].width = 0.5;
    state.objects[1].height = 0.5;
    state.objects[1].entityType = EXIT;
    
    state.objects[0].position = glm::vec3(18.5, -0.8, 0);
    state.objects[1].position = glm::vec3(18.5, -1.8, 0);
    
    state.objects[2].textureID = Util::LoadTexture("key.png");
    state.objects[2].entityType = KEY;
    state.objects[2].scale = glm::vec3(0.5,0.5,0);
    state.objects[2].width = 0.5;
    state.objects[2].height = 0.5;
    state.objects[2].position = glm::vec3(1, -3, 0);

    
    // Initialize Player
    // can put this somewhere else as u will make this a lot of time every level
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->level = l2_level;
    
    state.player->position = glm::vec3(10, -14, 0);
    state.player->movement = glm::vec3(0);
    state.player->scale = glm::vec3(2.0,2.0,0);
  
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("player.png");
    
    state.player->animRight = new int[3] {0, 0, 0};
    state.player->animLeft = new int[3] {4, 4, 4};
    state.player->animUpL = new int[3] {12, 12, 12};
    state.player->animUpR = new int[3] {8, 8, 8};
    
    state.player->animAttackR = new int[3] {1, 2, 3};
    state.player->animAttackL = new int[3] {5, 6, 7};
    state.player->animAttackUR = new int[3] {9, 10, 11};
    state.player->animAttackUL = new int[3] {13, 14, 15};

    
    state.player->state = IDLE;
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 3;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player->height = 0.4f;
    state.player->width = 0.4f;
    
    
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("skeleton.png");
    GLuint enemy2TextureID = Util::LoadTexture("maskguyL.png");
    GLuint enemy3TextureID = Util::LoadTexture("maskguyR.png");
    
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].speed = 1.5;
        state.enemies[i].width = 0.5f;
        state.enemies[i].height = 0.5f;
        state.enemies[i].scale = glm::vec3(0.75,0.75,0);
    }
    
    for (int i = 0; i < 3; i++) {
        state.enemies[i].textureID = enemyTextureID;
    }
    
    state.enemies[3].textureID = enemy3TextureID;
    state.enemies[4].textureID = enemy2TextureID;
    
    state.enemies[0].position = glm::vec3(5, -12, 0);
    state.enemies[0].aiType = WALKERX;
    state.enemies[1].position = glm::vec3(14, -10, 0);
    state.enemies[1].aiType = WALKERX;

    
    state.enemies[2].position = glm::vec3(10, -11, 0);
    state.enemies[2].aiType = WALKERX;
    state.enemies[3].position = glm::vec3(5, -5.5, 0);
    state.enemies[3].aiType = WAITANDGO;
    state.enemies[3].state = IDLE;
    state.enemies[4].position = glm::vec3(17, -1.5, 0);
    state.enemies[4].aiType = WAITANDGO;
    state.enemies[4].state = IDLE;
    
    state.enemies[5].textureID = enemyTextureID;
    state.enemies[5].aiType =  WALKERX;
    state.enemies[5].position = glm::vec3(2, -4, 0);
    state.enemies[5].speed = 2.5f;
    
    
    
}

bool Level2::checkAI() { //return 1 if enemy is all cleared
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive == true) {
            return 0;
        }
    }
    return 1;
}

void Level2::Update(float deltaTime) {
    
    for (int i = 0; i < LEVEL2_OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.objects, LEVEL2_OBJECT_COUNT, state.map);
    }
    
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.objects, LEVEL2_OBJECT_COUNT, state.map);
    }
    
    
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.objects, LEVEL2_OBJECT_COUNT, state.map);
    
    
    if (checkAI()) {
        state.clear = 1;
    }
    
    if (state.clear) {
        *state.player->level = 3;
    }

    
}

void Level2::Render(ShaderProgram *program) {
    
    state.map->Render(program);
    
    for (int i = 0; i < LEVEL2_OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }
    
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
    state.player->Render(program);

     
}

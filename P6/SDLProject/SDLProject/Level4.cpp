#include "Level4.h"

#define LEVEL4_WIDTH 20
#define LEVEL4_HEIGHT 16

#define LEVEL4_ENEMY_COUNT 6

#define LEVEL4_OBJECT_COUNT 6

// 42 empty brick
// pond 0,12,10, 27, 27, 29, 40, 44, 45

unsigned int level4_data[] = {
    78,    78,    78,    78,    10,     1,    4,    3,     2,      1,    2,     3,    4,     1,    2,    4,    5,   78,    78,    78,
    78,    78,    78,    78,    20,     7,    7,    7,     7,     7,    7,     7,    7,     7,    7,    7,    15,    78,    78,    78,
    78,    78,    78,    78,    30,     7,    7,    7,     7,     7,     7,     7,    7,    7,    7,    7,     35,    78,    78,    78,
    78,    78,    78,    78,    10,     7,    7,    7,     7,     7,     7,     7,     7,    7,    7,    7,    15,    78,   78,    78,
    78,    78,    78,    78,    20,     7,    7,    7,     7,      7,     7,     7,     7,    7,    7,    7,    25,    78,   78,   78,
    78,    78,    78,    78,    30,     7,    7,     7,     7,      7,     7,     7,     7,    7,    7,     7,    35,    78,   78,   78,
    78,    78,    78,    78,    40,     41,   53,     7,     7,     7,      7,     7,    7,     7,    54,   44,    45,    78,   78,   78,
    78,    78,    78,    78,    78,     78,    20,     7,     7,      7,     7,     7,   7,     7,    25,   78,   78,     78,   78,   78,
    30,     1,     2,     4,     5,     78,   30,     7,      7,     7,     7,    7,     7,    7,    35,   78,    78,     78,   78,    78,
    20,     7,     7,     7,     15,    78,   40,    42,    41,     53,     7,   54,     41,    44,   45,   78,    78,     78,   78,    78,
    30,     7,     7,     7,     25,    78,   78,    78,    78,     10,     7,   35,     78,    78,   78,   78,    78,     78,   78,    78,
    20,    7,     7,     7,      2,     1,    2,     3,      4,     1,     7,    1,     2,     3,    4,     1,     2,     3,    4,    5,
    10,    7,     7,     7,      7,     7,    7,    7,      7,     7,      7,    7,     7,     7,     7,    7,     7,     7,    7,    35,
    20,    7,     7,     7,     54,     41,   42,    43,    44,     53,     7,   54,     41,    42,    43,   44,    41,    42,   43,   45,
    30,    7,     7,     7,     35,     78,   78,    78,    78,     30,     7,    35,     78,    78,   78,  78,    78,    78,    78,    78,
    40,    41,    41,    42,     45,     78,   78,    78,    78,     40,     41,   45,    78,    78,    78,    78,   78,    78,    78,    78,
    
};

Level4::Level4(int *lives) {
    l4_level = lives;
}

void Level4::Initialize() {
    
    state.nextScene = -1;
    state.mode = GAME_LEVEL;
    
    GLuint mapTextureID = Util::LoadTexture("Dungeon_Tileset.png");
    state.map = new Map(LEVEL4_WIDTH, LEVEL4_HEIGHT, level4_data, mapTextureID, 1.0f, 10, 10, DUNGEON);
    
    // Initialize Game Objects
    
    // 0-1 is exit
    state.objects = new Entity[LEVEL4_OBJECT_COUNT];
    
    state.objects[0].textureID = Util::LoadTexture("sideexit1.png");
    state.objects[0].entityType = EXIT;
    state.objects[0].width = 0.5;
    state.objects[0].height = 0.5;
    state.objects[1].textureID = Util::LoadTexture("sideexit2.png");
    state.objects[1].width = 0.5;
    state.objects[1].height = 0.5;
    state.objects[1].entityType = EXIT;
    
    state.objects[0].position = glm::vec3(18.5, -11, 0);
    state.objects[1].position = glm::vec3(18.5, -12, 0);
    
    state.objects[2].textureID = Util::LoadTexture("key.png");
    state.objects[2].entityType = KEY;
    state.objects[2].scale = glm::vec3(0.5,0.5,0);
    state.objects[2].width = 0.5;
    state.objects[2].height = 0.5;
    state.objects[2].position = glm::vec3(10, -3, 0);
    state.objects[2].isActive = false;
    
    state.objects[3].textureID = Util::LoadTexture("key_silver.png");
    state.objects[3].entityType = BOSS_KEY;
    state.objects[3].scale = glm::vec3(0.5,0.5,0);
    state.objects[3].width = 0.5;
    state.objects[3].height = 0.5;
    state.objects[3].position = glm::vec3(1, -9, 0);
    
    state.objects[4].textureID = Util::LoadTexture("bossDoor1.png");
    state.objects[4].entityType = BOSS_DOOR;
    state.objects[4].width = 0.5;
    state.objects[4].height = 0.5;
    
    state.objects[4].position = glm::vec3(10, -9, 0);
    
    state.objects[5].textureID = Util::LoadTexture("Fish-rod.png");
    state.objects[5].entityType = FISHING_ROD;
    state.objects[5].scale = glm::vec3(0.5,0.5,0);
    state.objects[5].width = 0.5;
    state.objects[5].height = 0.5;
    state.objects[5].position = glm::vec3(11, -3, 0);
    state.objects[5].isActive = false;
     

    // Initialize Player
    // can put this somewhere else as u will make this a lot of time every level
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->level = l4_level;
    
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
    
    
    state.enemies = new Entity[LEVEL4_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("skeleton.png");
    
    for (int i = 0; i < LEVEL4_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].speed = 2.7f;
        state.enemies[i].width = 0.5f;
        state.enemies[i].height = 0.5f;
        state.enemies[i].scale = glm::vec3(0.75,0.75,0);
    }
    
    
    for (int i = 0; i < LEVEL4_ENEMY_COUNT; i++) {
        state.enemies[i].textureID = enemyTextureID;
    }
    
    state.enemies[0].position = glm::vec3(2, -10, 0);
    state.enemies[0].aiType = WALKERY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(1, -10, 0);
    state.enemies[1].aiType = WALKERY;
    
    
    state.enemies[2].position = glm::vec3(10, -3, 0);
    state.enemies[2].aiType = WALKERX;
    state.enemies[2].textureID = enemyTextureID;
    
    state.enemies[3].position = glm::vec3(5, -2, 0);
    state.enemies[3].aiType = WALKERX;
    state.enemies[4].position = glm::vec3(14, -1, 0);
    state.enemies[4].aiType = WALKERX;
    state.enemies[5].position = glm::vec3(8, -5, 0);
    state.enemies[5].aiType = WALKERX;
    
  
    
    
}

bool Level4::checkAI() { //return 1 if enemy is all cleared
    for (int i = 0; i < LEVEL4_ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive == true) {
            return 0;
        }
    }
    return 1;
}

void Level4::Update(float deltaTime) {
    
    for (int i = 0; i < LEVEL4_OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.enemies, LEVEL4_ENEMY_COUNT, state.objects, LEVEL4_OBJECT_COUNT, state.map);
    }
    
    for (int i = 0; i < LEVEL4_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL4_ENEMY_COUNT, state.objects, LEVEL4_OBJECT_COUNT, state.map);
    }
    
    
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL4_ENEMY_COUNT, state.objects, LEVEL4_OBJECT_COUNT, state.map);
    
    
    if (checkAI()) {
        state.clear = 1;
    }
    
    if (state.clear) {
        *state.player->level = 5;
        if (state.player->key == 0) {
            state.objects[2].isActive = true;
        }
        if (state.player->mystery == 0) {
            state.objects[5].isActive = true;
        }

    }

    
}

void Level4::Render(ShaderProgram *program) {
    
    state.map->Render(program);
    
    for (int i = 0; i < LEVEL4_OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }
    
    for (int i = 0; i < LEVEL4_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
    state.player->Render(program);

     
}

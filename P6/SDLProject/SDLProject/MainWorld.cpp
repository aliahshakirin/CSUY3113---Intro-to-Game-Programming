#include "MainWorld.h"

#define MAINWORLD_WIDTH 20
#define MAINWORLD_HEIGHT 16

#define MAINWORLD_ENEMY_COUNT 0

#define MAINWORLD_OBJECT_COUNT 17

// 42 empty brick
// pond 11,12,13, 27, 27, 29, 43, 44, 45

unsigned int mainWorld_data[] = {
    3,  3,  3,  3,  37,  35,  3,  3,  3,  3,  3,  3,  3,  3,  37,  35,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3,  3,  3,  3,  53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3,  3,  3,  3,  53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3, 11, 12,  13,  3,  3,  3,  3,  53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3, 27, 28,  29,  3,  3,  3,  3,  53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3, 43, 44,  45,  3,  3,  3,  3,  53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3,  3,  3,  3,  53,  51,  3,  3,  3,  3,
    55, 55, 55, 55, 57,  54, 55,  55, 55, 55,  55, 55,  55, 55, 57, 54,  55,  55, 55,  55,
    23, 23, 23, 23, 25,  22, 23,  23, 23, 23,  23, 23,  23, 23, 25, 22,  23,  23, 23,  23,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3,  3,  3,  3, 53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3,  3,  3,  3, 53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3,  3,  3,  3, 53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3, 11, 12,  13, 53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3, 27, 28,  29, 53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3, 43, 44,  45, 53,  51,  3,  3,  3,  3,
    3,  3,  3,  3,  53,  51,  3,  3,  3,  3,  3,  3,  3,  3, 53,  51,  3,  3,  3,  3,
    
};

MainWorld::MainWorld(int *level) {
    mw_level = level;
}

void MainWorld::Initialize() {
    
    state.nextScene = -1;
    state.mode = GAME_LEVEL;
    
    GLuint mapTextureID = Util::LoadTexture("terrain.png");
    state.map = new Map(MAINWORLD_WIDTH, MAINWORLD_HEIGHT, mainWorld_data, mapTextureID, 1.0f, 16, 16, TERRAIN);
    
    // Initialize Game Objects
    
    state.objects = new Entity[MAINWORLD_OBJECT_COUNT];
    // 0 - 3 is entrance
    for (int i = 0; i < 4; i++) {
        state.objects[i].textureID = Util::LoadTexture("trap_door.png");
    }
    
    state.objects[0].position = glm::vec3(3, -15, 0);
    state.objects[0].entityType = ENTRANCE1;
    state.objects[1].position = glm::vec3(3, -1, 0);
    state.objects[1].entityType = ENTRANCE2;
    state.objects[2].position = glm::vec3(13, -15, 0);
    state.objects[2].entityType = ENTRANCE3;
    state.objects[3].position = glm::vec3(13, -1, 0);
    state.objects[3].entityType = ENTRANCE4;
    
    state.objects[4].textureID = Util::LoadTexture("ring.png");
    state.objects[4].scale = glm::vec3(0.5,0.5,0);
    state.objects[4].entityType = RING;
    state.objects[4].isActive = false;
    
    for (int i = 5; i < MAINWORLD_OBJECT_COUNT; i++) {
        state.objects[i].textureID = Util::LoadTexture("tree.png");
    }
    state.objects[5].position = glm::vec3(6, -6.3, 0);
    state.objects[6].position = glm::vec3(7, -6.3, 0);
    state.objects[7].position = glm::vec3(13, -8.4, 0);
    state.objects[8].position = glm::vec3(13, -9.4, 0);
    state.objects[9].position = glm::vec3(3, -13, 0);
    state.objects[10].position = glm::vec3(2, -13, 0);
    state.objects[11].position = glm::vec3(3, -12, 0);
    state.objects[12].position = glm::vec3(10, -13, 0);
    state.objects[13].position = glm::vec3(10, -14, 0);
    state.objects[14].position = glm::vec3(19, -6.2, 0);
    state.objects[15].position = glm::vec3(18, -6.2, 0);
    state.objects[16].position = glm::vec3(6, -1, 0);
    
    // Initialize Player
    // can put this somewhere else as u will make this a lot of time every level
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->level = mw_level;
    
    if (*(state.player->level) == 1) {
        state.player->position = glm::vec3(9.5, -7.5, 0);
    } else if (*(state.player->level) == 2) {
        state.player->position = glm::vec3(4, -15, 0);
    }
    else if (*(state.player->level) == 3) {
        state.player->position = glm::vec3(4, -1, 0);
    }
    else if (*(state.player->level) == 4) {
        state.player->position = glm::vec3(14, -15, 0);
    }
    else if (*(state.player->level) == 5) {
        state.player->position = glm::vec3(14, -1, 0);
    }
    
     // 4, -15
    state.player->movement = glm::vec3(0);
    state.player->scale = glm::vec3(2.0,2.0,0);
    //state.player->acceleration = glm::vec3(0,-9.81f,0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("player.png");
    
    state.player->animRight = new int[3] {0, 0, 0};
    state.player->animLeft = new int[3] {4, 4, 4};
    state.player->animUpL = new int[3] {8, 8, 8};
    state.player->animUpR = new int[3] {12, 12, 12};
    
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
    
    state.player->height = 0.3f;
    state.player->width = 0.3f;
    
    state.player->jumpPower = 6.5f;
    //state.player->isActive = false;
    
    
}
void MainWorld::Update(float deltaTime) {
    
    for (int i = 0; i < MAINWORLD_OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.enemies, MAINWORLD_ENEMY_COUNT, state.objects, MAINWORLD_OBJECT_COUNT, state.map);
    }
    
    state.player->Update(deltaTime, state.player, state.enemies, MAINWORLD_ENEMY_COUNT, state.objects, MAINWORLD_OBJECT_COUNT, state.map);
    
    state.objects[4].position = state.player->ring_pos; // position based on player collision flag
    
    // 3, -14
    
}

void MainWorld::Render(ShaderProgram *program) {
    
    GLuint wordTextureID = Util::LoadTexture("pixel_font.png");
    
    state.map->Render(program);
    
    Util::DrawText(program, wordTextureID, "Dungeon 1", 0.3, 0.0f, glm::vec3(1, -14.3 ,0));
    Util::DrawText(program, wordTextureID, "Dungeon 2", 0.3, 0.0f, glm::vec3(1.2, -0.3 ,0));
    Util::DrawText(program, wordTextureID, "Dungeon 3", 0.3, 0.0f, glm::vec3(10, -15 ,0));
    Util::DrawText(program, wordTextureID, "Dungeon 4", 0.3, 0.0f, glm::vec3(11, -0.3 ,0));
    
    
    for (int i = 0; i < MAINWORLD_OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }
   
    state.player->Render(program);
    
     
}


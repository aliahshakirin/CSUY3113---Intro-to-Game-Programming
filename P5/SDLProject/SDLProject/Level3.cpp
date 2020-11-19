#include "Level3.h"

#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 1

GLuint lose3TextureID;

unsigned int level3_data[] = {
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 3,
    3, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3,
    3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3,
    3, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3,
    3, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3
};

Level3::Level3(int *lives) {
    l3_lives = lives;
}

void Level3::Initialize() {
    
    state.nextScene = -1;
    state.mode = GAME_LEVEL;
    
    state.background = new Background();
    state.background->textureID = Util::LoadTexture("bg_star.png");
    state.background->position = glm::vec3(7, -1, 0);

    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    
    lose3TextureID = Util::LoadTexture("pixel_font.png");
    
    // Initialize Game Objects
    
    // Initialize Player
    // can put this somewhere else as u will make this a lot of time every level
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->lives = l3_lives;
    state.player->position = glm::vec3(2, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-9.81f,0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player->height = 0.8f;
    state.player->width = 0.5f;
    
    state.player->jumpPower = 6.5f;
    
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
       
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(9, -1, 0);
    state.enemies[0].acceleration = glm::vec3(0,-9.81f,0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    
    
}
void Level3::Update(float deltaTime) {
    state.background->Update(deltaTime);
    
    int prevLives = *l3_lives;
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
           state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
       }
    
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
    if ((*l3_lives) < prevLives && (*l3_lives) != 0 && state.enemies[0].isActive == true) {
        state.nextScene = 3;
    }
    
   
    
}

void Level3::Render(ShaderProgram *program) {
    state.background->Render(program);
    state.map->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
    state.player->Render(program);
    
    if (*l3_lives == 0 && state.enemies[0].isActive == true) {
        if (state.player->position.x > 7) {
            Util::DrawText(program, lose3TextureID, "You Lose", 0.9, 0.0f, glm::vec3(state.player->position.x - 4.5,-4,0));
        } else {
            Util::DrawText(program, lose3TextureID, "You Lose", 0.9, 0.0f, glm::vec3(state.player->position.x - 2,-4,0));
        }
    }
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive == false) { //only for 1 enemy
            Util::DrawText(program, lose3TextureID, "You Win", 0.9, 0.0f, glm::vec3(5, -4 ,0));
            
        }
        
    }
}

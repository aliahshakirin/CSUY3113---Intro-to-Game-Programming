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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"
//26
#define PLATFORM_COUNT 30
#define ENEMY_COUNT 3

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *enemies;
    Entity *checkpoint;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Rise of the AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
   
    // Initialize Game Objects
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-4, -1.25, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-9.81f,0);
    state.player->speed = 1.5f;
    state.player->textureID = LoadTexture("Female.png");
    
    state.player->animRight = new int[3] {6, 7, 8};
    state.player->animLeft = new int[3] {3, 4, 5};
    state.player->animUp = new int[3] {9, 10, 11};
    state.player->animDown = new int[3] {0, 1 , 2};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 3;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 3;
    state.player->animRows = 4;
    
    state.player->height = 1.0f;
    state.player->width = 0.6f;;
    
    state.player->jumpPower = 6.7f;
    
    state.checkpoint = new Entity();
    state.checkpoint->textureID = LoadTexture("checkpoint.png");
    state.checkpoint->position = glm::vec3(4.5, -1.85, 0);
    state.checkpoint->animeIdle = new int[10] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    state.checkpoint->animIndices = state.checkpoint->animeIdle;
    state.checkpoint->animFrames = 10;
    state.checkpoint->animIndex = 0;
    state.checkpoint->animTime = 0;
    state.checkpoint->animCols = 10;
    state.checkpoint->animRows = 1;

    //state.door->size = glm::vec3(3.0f,3.0f,1.0f);
    state.checkpoint->Update(0, NULL, NULL, 0, NULL, 0);
    
    state.platforms = new Entity[PLATFORM_COUNT];
    
    GLuint topTerrainTextureID = LoadTexture("terrain_top_center.png");
    GLuint terrainTextureID = LoadTexture("terrain_center.png");
    GLuint platformTextureID = LoadTexture("terrain_platform_center.png");
    
    for (int i = 0; i < 11; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = topTerrainTextureID;
        state.platforms[i].position = glm::vec3(-5+i,-2.75f,0);
    }
    
    
    for (int i = 11; i < 23; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = terrainTextureID;
        state.platforms[i].position = glm::vec3(-5+(i-12),-3.75f,0);
    }
    
    for (int i = 23; i < 26; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(-5+(i-23),1,0);
    }
    
    for (int i = 26; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(-0.70+(i-26),-0.7,0);
    }
    
    for(int i =0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL, NULL, 0, NULL, 0);
    }
    
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("Soldier.png");
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].width = 0.5f;
        //state.enemies[i].height = 1.0f;
        state.enemies[i].animRight = new int[3] {6, 7, 8};
        state.enemies[i].animLeft = new int[3] {3, 4, 5};
        state.enemies[i].animUp = new int[3] {9, 10, 11};
        state.enemies[i].animDown = new int[3] {0, 1 , 2};
        
        state.enemies[i].animFrames = 3;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 3;
        state.enemies[i].animRows = 4;
        
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        
        state.enemies[i].speed = 1;
    
    }
    
    
    state.enemies[0].animIndices = state.player->animLeft;
    state.enemies[1].animIndices = state.player->animRight;
    state.enemies[2].animIndices = state.player->animLeft;
    
    state.enemies[0].position = glm::vec3(3.5, -1.75, 0);
    state.enemies[1].position = glm::vec3(-3.75, 2, 0);
    state.enemies[1].acceleration = glm::vec3(0,-9.81f,0);
    state.enemies[2].position = glm::vec3(1, 0.3, 0);
    
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    
    state.enemies[1].aiType = JUMPER;
    //state.enemies[1].movement = glm::vec3(1,0,0);
    //state.enemies[1].aiState = JUMPING;
    
    state.enemies[2].aiType = WALKER;
    state.enemies[2].movement = glm::vec3(-1,0,0);
    //state.enemies[2].aiState = WALKING;
    
}

void ProcessInput() {
    
    state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
                        if (state.player->collidedBottom) {
                            state.player->jump = true;
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
    }
    

    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        
        state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        }
        deltaTime -= FIXED_TIMESTEP;
    }
        accumulator = deltaTime;
}
    


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    
    state.checkpoint->Render(&program);
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }

    state.player->Render(&program);
    
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}

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
#include <vector>

#include "Entity.h"

#define PLATFORM_COUNT 28
#define TARGET_COUNT 2

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *target;
    bool gameEnd = false;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

bool gameEnd = false;

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

void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position) {
    
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    
    std::vector<float> vertices;
    std::vector<float> texCoords;
    
    for (int i = 0; i <text.size(); i++) {
        
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        
        float u = (float) (index % 16) / 16.0f;
        float v = (float) (index / 16) / 16.0f;
        
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
        });
        
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
        });
    }
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix,position);
    program->SetModelMatrix(modelMatrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Lunar Lander!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    state.player->position = glm::vec3(-2,3,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-0.2,0);
    state.player->speed = 1.0f;
    state.player->textureID = LoadTexture("playerShip3_green.png");
    
    state.player->height = 1.0f;
    state.player->width = 1.0f;
    
    
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("platformPack_tile041.png");
    
    float x_pos = -4.5;
    for(int i =0; i < 10; i++) {
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(x_pos,-3.25,0);
        x_pos += 1.0;
    }
    float y_pos = -2.25;
    for(int i =10; i < 17; i++) {
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(-4.5,y_pos,0);
        y_pos += 1.0;
    }
    
    y_pos = -2.25;
    for(int i =17; i < PLATFORM_COUNT; i++) {
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(4.5,y_pos,0);
        y_pos += 1.0;
    }
    
    state.platforms[24].textureID = platformTextureID;
    state.platforms[24].position = glm::vec3(-3.5,0.75,0);
    
    state.platforms[25].textureID = platformTextureID;
    state.platforms[25].position = glm::vec3(-2.5,0.75,0);
    
    state.platforms[26].textureID = platformTextureID;
    state.platforms[26].position = glm::vec3(1.5,0.75,0);
    
    state.platforms[27].textureID = platformTextureID;
    state.platforms[27].position = glm::vec3(2.5,0.75,0);
    
    for(int i =0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL, 0, NULL, 0, &state);
        state.platforms[i].entityType = PLATFORM;
    }

    state.target = new Entity[TARGET_COUNT];
    GLuint targetTextureID = LoadTexture("platformIndustrial_064.png");
    
    state.target[0].textureID = targetTextureID;
    state.target[0].position = glm::vec3(0.5,-3.25,0);
    
    state.target[1].textureID = targetTextureID;
    state.target[1].position = glm::vec3(1.5,-3.25,0);
    
    for(int i =0; i < TARGET_COUNT; i++) {
        state.target[i].entityType = TARGET;
        state.target[i].Update(0, NULL, 0, NULL, 0, &state);
    }
    
    state.platforms[5].entityType = TARGET;
    state.platforms[6].entityType = TARGET;
    
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
                }
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        if (!state.gameEnd) {
            state.player->acceleration.x -= 0.1f;
        }
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        if (!state.gameEnd) {
            state.player->acceleration.x += 0.1f;
        }
    }
    else {
        state.player->acceleration.x = 0;
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
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT, state.target, TARGET_COUNT, &state);
        //state.player->Update(FIXED_TIMESTEP, state.target, TARGET_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }
        accumulator = deltaTime;
}
    


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    
    for (int i = 0; i < TARGET_COUNT; i++) {
        state.target[i].Render(&program);
    }
     
    state.player->Render(&program);
    
    GLuint fontTextureID = LoadTexture("font1.png");
    
    if (!state.player->alive) {
        DrawText(&program, fontTextureID, "Mission Failed!", 0.9, -0.5f, glm::vec3(-2.5,0,0));
    }
    
    if (state.player->alive && state.gameEnd) {
        DrawText(&program, fontTextureID, "Mission Successful!", 0.9, -0.5f, glm::vec3(-3.5,0,0));
    }
    
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

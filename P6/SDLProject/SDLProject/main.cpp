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
#include <SDL_mixer.h>

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Menu.h"
#include "MainWorld.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::mat4 uiViewMatrix, uiProjectionMatrix;

Scene *currentScene;
Scene *sceneList[6];

Mix_Music *music_dungeon;
Mix_Music *main_music;
Mix_Chunk *fishing;
Mix_Chunk *door;


int level = 1;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Final Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music_dungeon = Mix_LoadMUS("dungeon_atmo.mp3");
    main_music = Mix_LoadMUS("Royal Coupling.mp3");
    Mix_PlayMusic(main_music,-1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    
    fishing = Mix_LoadWAV("fishing.wav");
    door = Mix_LoadWAV("door_open.wav");
    
    //player_die = Mix_LoadWAV("player_die.wav");
    
    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    sceneList[0] = new Menu();
    sceneList[1] = new MainWorld(&level);
    sceneList[2] = new Level1(&level);
    sceneList[3] = new Level2(&level);
    sceneList[4] = new Level3(&level);
    sceneList[5] = new Level4(&level);
    SwitchToScene(sceneList[0]); // change later
    
}


void ProcessInput() {
    
    if (currentScene != sceneList[0]) {
        currentScene->state.player->movement = glm::vec3(0);
    }
    
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
                        currentScene->state.player->state = ATTACKING;
                        
                        if (currentScene->state.mode == MAIN_MENU) break;
                        
                        if (currentScene->state.player->animIndices == currentScene->state.player->animLeft) {
                            currentScene->state.player->width = 1.2f;
                            currentScene->state.player->animIndices = currentScene->state.player->animAttackL;
                        }
                        else if (currentScene->state.player->animIndices == currentScene->state.player->animRight) {
                            currentScene->state.player->width = 1.2f;
                            currentScene->state.player->animIndices = currentScene->state.player->animAttackR;
                        }
                        else if (currentScene->state.player->animIndices == currentScene->state.player->animUpL) {
                            currentScene->state.player->height = 1.2f;
                            currentScene->state.player->animIndices = currentScene->state.player->animAttackUL;
                        }
                        else if (currentScene->state.player->animIndices == currentScene->state.player->animUpR) {
                            currentScene->state.player->height = 1.2f;
                            currentScene->state.player->animIndices = currentScene->state.player->animAttackUR;
                        }
                        
                        break;
                    case SDLK_RETURN:
                        if (currentScene->state.mode == MAIN_MENU) {
                            currentScene->state.nextScene = 1;
                        }
                        break;
                    case SDLK_e:
                        if (currentScene->state.mode == GAME_LEVEL) {
                            // colliding with entrance must match with player level to switch scene
                            if (currentScene->state.player->lastCollision == ENTRANCE1 && level == 1) {
                                Mix_PlayChannel(-1,door,0);
                                Mix_PlayMusic(music_dungeon,-1);
                                currentScene->state.nextScene = 2;
                            }
                            if (currentScene->state.player->lastCollision == ENTRANCE2 && level == 2) {
                                Mix_PlayChannel(-1,door,0);
                                Mix_PlayMusic(music_dungeon,-1);
                                currentScene->state.nextScene = 3;
                            }
                            if (currentScene->state.player->lastCollision == ENTRANCE3 && level == 3) {
                                Mix_PlayChannel(-1,door,0);
                                Mix_PlayMusic(music_dungeon,-1);
                                currentScene->state.nextScene = 4;
                            }
                            if (currentScene->state.player->lastCollision == ENTRANCE4 && level == 4) {
                                Mix_PlayChannel(-1,door,0);
                                Mix_PlayMusic(music_dungeon,-1);
                                currentScene->state.nextScene = 5;
                            }
                            if (currentScene->state.player->lastCollision == EXIT && currentScene->state.player->key == 1 && currentScene->state.clear == 1) { // + ai is all dead
                                Mix_PlayChannel(-1,door,0);
                                Mix_PlayMusic(main_music,-1);
                                currentScene->state.nextScene = 1;
                            }
                            if (currentScene->state.player->lastCollision == BOSS_DOOR && currentScene->state.player->boss_key == 1) {
                                Mix_PlayChannel(-1,door,0);
                                currentScene->state.objects[4].isActive = false;
                            }
                            if (currentScene->state.player->lastCollision == POND && level == 5
                                && currentScene->state.player->key == 0 && currentScene->state.missing_object_appear == 0) { // change level
                                Mix_PlayChannel(-1,fishing,0);
                                currentScene->state.objects[4].isActive = true;
                                currentScene->state.missing_object_appear = 1;
                                
                            }
                            
                            
                            
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    if (currentScene == sceneList[0]) return;
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        if (currentScene->state.player->animIndex >= currentScene->state.player->animFrames && currentScene->state.player->state == ATTACKING) {
            currentScene->state.player->state = IDLE;
            //currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        } else if (currentScene->state.player->state == IDLE) {
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        }
        
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        if (currentScene->state.player->animIndex >= currentScene->state.player->animFrames && currentScene->state.player->state == ATTACKING) {
            currentScene->state.player->state = IDLE;
            //currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
        } else if (currentScene->state.player->state == IDLE) {
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
        }
        
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        
        currentScene->state.player->movement.y = -1.0f;
        
        if (currentScene->state.player->animIndex >= currentScene->state.player->animFrames && currentScene->state.player->state == ATTACKING) {
            currentScene->state.player->state = IDLE;
            if (currentScene->state.player->animIndices == currentScene->state.player->animAttackR) {
                currentScene->state.player->animIndices = currentScene->state.player->animRight;
            } else if (currentScene->state.player->animIndices == currentScene->state.player->animAttackL) {
                currentScene->state.player->animIndices = currentScene->state.player->animLeft;
            }
        } else if (currentScene->state.player->state == IDLE) {
            if (currentScene->state.player->animIndices == currentScene->state.player->animUpR) {
                currentScene->state.player->animIndices = currentScene->state.player->animRight;
            } else if (currentScene->state.player->animIndices == currentScene->state.player->animUpL) {
                currentScene->state.player->animIndices = currentScene->state.player->animLeft;
            }
        }
        
        
    }
    else if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->movement.y = 1.0f;
        
        if (currentScene->state.player->animIndex >= currentScene->state.player->animFrames && currentScene->state.player->state == ATTACKING) {
            if (currentScene->state.player->animIndices == currentScene->state.player->animAttackUR) {
                currentScene->state.player->animIndices = currentScene->state.player->animUpR;
            } else if (currentScene->state.player->animIndices == currentScene->state.player->animAttackUL) {
                currentScene->state.player->animIndices = currentScene->state.player->animUpL;
            }
        } else if (currentScene->state.player->state == IDLE) {
            if (currentScene->state.player->animIndices == currentScene->state.player->animRight) {
                currentScene->state.player->animIndices = currentScene->state.player->animUpR;
            } else if (currentScene->state.player->animIndices == currentScene->state.player->animLeft) {
                currentScene->state.player->animIndices = currentScene->state.player->animUpL;
            }
        }
        
    }
    

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    program.SetViewMatrix(viewMatrix);
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    
    if (currentScene == sceneList[0]) return;
    
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    //std::cout << currentScene->state.player->position.y << '\n';
    
    if (currentScene->state.player->position.x > 5 &&  currentScene->state.player->position.x < 14.5) {
        
        if (currentScene->state.player->position.y > -4.5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        } else if (currentScene->state.player->position.y <= -4.5 && currentScene->state.player->position.y > -12.5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, -currentScene->state.player->position.y -0.75, 0));
        } else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 11.75, 0));
        }
        
    } else if (currentScene->state.player->position.x >= 14.5) {
        if (currentScene->state.player->position.y > -4.5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-14.5, 3.75, 0));
        } else if (currentScene->state.player->position.y <= -4.5 && currentScene->state.player->position.y > -12.5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-14.5, -currentScene->state.player->position.y -0.75, 0));
        } else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-14.5, 11.75, 0));
        }
        
        //viewMatrix = glm::translate(viewMatrix, glm::vec3(-14.5, 3.75, 0));
    }
    else {
        if (currentScene->state.player->position.y > -4.5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        } else if (currentScene->state.player->position.y <= -4.5 && currentScene->state.player->position.y > -12.5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player->position.y -0.75, 0));
        } else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 11.75, 0));
        }
        
    }
    
    
    
}
    


void Render() {
    GLuint wordTextureID = Util::LoadTexture("pixel_font.png");
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program); // check
    
    program.SetProjectionMatrix(uiProjectionMatrix);
    program.SetViewMatrix(uiViewMatrix);
    
    if (currentScene->state.mode == GAME_LEVEL) {
        if (currentScene->state.player->missing_object == 1) {
            Util::DrawText(&program, wordTextureID, "Congrats!", 0.9, 0.0f, glm::vec3(-3.3,0.5,0));
            Util::DrawText(&program, wordTextureID, "You found the missing ring", 0.3, 0.0f, glm::vec3(-3.5,-1.5,0));
        }
        
    }
    
    if (currentScene->state.mode == GAME_LEVEL) {
        if (currentScene->state.player->isActive == false) {
            Util::DrawText(&program, wordTextureID, "Annihilated", 0.8, 0.0f, glm::vec3(-3.7,0,0));
            Util::DrawText(&program, wordTextureID, "You fail to find the", 0.3, 0.0f, glm::vec3(-2.8,-1,0));
            Util::DrawText(&program, wordTextureID, "missing ring", 0.3, 0.0f, glm::vec3(-1.7,-1.5,0));
        }
        
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
        
        
        if(currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]); //put player live
         
        Render();
         
    }
    
    Shutdown();
    return 0;
}

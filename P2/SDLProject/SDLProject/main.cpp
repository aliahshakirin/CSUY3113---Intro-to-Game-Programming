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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::mat4 player1, player2, ball;

glm::vec3 player1_position = glm::vec3(0,0,0);
glm::vec3 player2_position = glm::vec3(0,0,0);
glm::vec3 ball_position = glm::vec3(0,0,0);

glm::vec3 player1_movement = glm::vec3(0,0,0);
glm::vec3 player2_movement = glm::vec3(0,0,0);

float player_speed = 2.0f;
float window_height = 10.0f;
float window_width = 7.5f;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960); //change to 640 480 before submitting
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    player1 = glm::mat4(1.0f);
    player2 = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
}

void ProcessInput() {
    
    player1_movement = glm::vec3(0);
    player2_movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                    gameIsRunning = false;
                    break;
            case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    // Some sort of action
                    
                    break;
                case SDLK_DOWN:
                    // Some sort of action
                                   
                    break;
                case SDLK_w:
                // Some sort of action
                    
                    break;
                case SDLK_s:
                // Some sort of action
                break;
            }
            break; // SDL_KEYDOWN
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_UP]) {
        player1_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        player1_movement.y = -1.0f;
    }
    if(keys[SDL_SCANCODE_W]) {
        player2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        player2_movement.y = -1.0f;
    }
    if (glm::length(player1_movement) > 1.0f) { player1_movement = glm::normalize(player1_movement);
    }
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f; float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    // Add (direction * units per second * elapsed time)
    
    player1 = glm::mat4(1.0f);
    player2 = glm::mat4(1.0f);
    ball = glm::mat4(1.0f);
    
    glm::vec3 oldP1 = player1_position;
    glm::vec3 oldP2 = player2_position;
    //glm::vec3 oldBall = ball_position;
    
    ball_position.x += 1.0 * deltaTime;
    player1_position += player1_movement * player_speed * deltaTime;
    player2_position += player2_movement * player_speed * deltaTime;
    
    if (player1_position.y > 2.77) {
        player1_position.y = oldP1.y;
    }
    
    if (player1_position.y < -2.77) {
        player1_position.y = oldP1.y;
    }
    
    if (player2_position.y > 2.77) {
         player2_position.y = oldP2.y;
     }
     
     if (player2_position.y < -2.77) {
         player2_position.y = oldP2.y;
     }
    
    player1 = glm::translate(player1, player1_position);
    player2 = glm::translate(player2, player2_position);
    //ball = glm::translate(ball, ball_position);
    
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    //DRAW PADDLE 1
    program.SetModelMatrix(player1);
    float vertices_paddle1[] = {
        //t1
        -5.0f, 1.0f, //top left
        -4.5f, 1.0f, //top right
        -5.0f, -1.0f, //bottom left
        //t2
        -4.5f, 1.0f, //top right
        -5.0f, -1.0f, //bottom left
        -4.5f, -1.0f //bottom right
    };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_paddle1);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    //DRAW PADDLE 2
    program.SetModelMatrix(player2);
    float vertices_paddle2[] = {
        //t1
        4.5f, 1.0f, //top left
        5.0f, 1.0f, //top right
        4.5f, -1.0f, //bottom left
        //t2
        5.0f, 1.0f, //top right
        4.5f, -1.0f, //bottom left
        5.0f, -1.0f //bottom right
    };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_paddle2);
    //glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    //DRAW BALL
    program.SetModelMatrix(ball);
    float vertices_ball[] = {
        //t1
        -0.1f, 0.1f, //top left
        0.1f, 0.1f, //top right
        -0.1f, -0.1f, //bottom left
        //t2
        0.1f, 0.1f, //top right
        -0.1f, -0.1f, //bottom left
        0.1f, -0.1f //bottom right
    };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_ball);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    
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

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
glm::vec3 ball_movement = glm::vec3(0,0,0);

float player_speed = 2.0f;
float ball_speed = 4.0f;

float paddle_height = 2.0f;
float paddle_width = 0.5f;
float ball_length = 0.2f;

float player1_points = 0;
float player2_points = 0;

bool gameEnd = false;

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
    
    //MOVE PADDLES TO STARTING POSITION
    player1_position.x = -4.75;
    player2_position.x = 4.75;
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
                case SDLK_SPACE: // set random value
                    ball_movement.y = 0.5f;
                    ball_movement.x = 0.5f;
                    if (gameEnd) {
                        ball_position.x = 0;
                        ball_position.y = 0;
                        player1_position.y = 0;
                        player2_position.y = 0;
                        gameEnd = false;
                    }
                    break;
            }
    
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_UP]) {
        player2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        player2_movement.y = -1.0f;
//        if (gameEnd) {
//            player2_movement.y = 0;
//            player
//        }
    }
    if(keys[SDL_SCANCODE_W]) {
        player1_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        player1_movement.y = -1.0f;
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
    
    if(gameEnd) {
        player1_movement.y = 0;
        player2_movement.y = 0;
    }
    
    player1_position += player1_movement * player_speed * deltaTime;
    player2_position += player2_movement * player_speed * deltaTime;
    
    float p1_top = player1_position.y + (paddle_height/2);
    float p1_bottom = player1_position.y - (paddle_height/2);
    float p2_top = player2_position.y + (paddle_height/2);
    float p2_bottom = player2_position.y - (paddle_height/2);
    
    float ball_bottom = ball_position.y - (ball_length/2);
    float ball_top = ball_position.y + (ball_length/2);
    float ball_right = ball_position.x + (ball_length/2);
    float ball_left = ball_position.x - (ball_length/2);
    
    // DIFF BALL AND PADDLE 1
    float x_dif1 = fabs(player1_position.x - ball_position.x);
    float y_dif1 = fabs(player1_position.y - ball_position.y);
    float x_dist1 = x_dif1 - (paddle_width + ball_length)/2;
    float y_dist1 = y_dif1 - (paddle_height + ball_length)/2;
    
    // DIFF BALL AND PADDLE 2
    float x_dif2 = fabs(player2_position.x - ball_position.x);
    float y_dif2 = fabs(player2_position.y - ball_position.y);
    float x_dist2 = x_dif2 - (paddle_width + ball_length)/2;
    float y_dist2 = y_dif2 - (paddle_height + ball_length)/2;
 
    
    //CHECK PADDLE 1 WINDOW COLLISION
    if (p1_top > 3.75f) {
        player1_position.y = oldP1.y;
    }
    
    if (p1_bottom < -3.75) {
        player1_position.y = oldP1.y;
    }
    
    //CHECK PADDLE 2 WINDOW COLLISION
    if (p2_top > 3.75f) {
         player2_position.y = oldP2.y;
     }
     
     if (p2_bottom < -3.75f) {
         player2_position.y = oldP2.y;
     }
    
    //CHECK BALL WINDOW COLLISION
    if(ball_top > 3.75) {
        ball_movement.y = -ball_movement.y;
    } else if (ball_bottom < -3.75) {
        ball_movement.y = -ball_movement.y;
    } else if (ball_right > 5.0) {
        ball_movement.x = 0;
        ball_movement.y = 0;
        gameEnd = true;
    } else if (ball_left < -5.0) {
        ball_movement.x = 0;
        ball_movement.y = 0;
        gameEnd = true;
    }
    
    //CHECK BALL PADDLE 1 COLLISION
    if(x_dist1 < 0 && y_dist1 < 0) {
        ball_movement.x = -ball_movement.x;
    }
    
    //CHECK BALL PADDLE 2 COLLISION
    if(x_dist2 < 0 && y_dist2 < 0) {
        ball_movement.x = -ball_movement.x;
    }
    
    ball_position += ball_movement * ball_speed * deltaTime;
    
    player1 = glm::translate(player1, player1_position);
    player2 = glm::translate(player2, player2_position);
    ball = glm::translate(ball, ball_position);
    
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float paddleVertices[] = {
        //t1
        -0.25f, 1.0f, //top left
        0.25f, 1.0f, //top right
        -0.25f, -1.0f, //bottom left
        //t2
        0.25f, 1.0f, //top right
        -0.25f, -1.0f, //bottom left
        0.25f, -1.0f //bottom right
    };
    
    //SET PADDLE VERTICES
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddleVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    //DRAW PADDLE 1
    program.SetModelMatrix(player1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    //DRAW PADDLE2
    program.SetModelMatrix(player2);
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

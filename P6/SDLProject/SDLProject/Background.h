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


class Background {
public:
    
    glm::vec3 position;
    glm::vec3 scale;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    Background();
    
    void Update(float deltaTime);
    void Render(ShaderProgram *program);
    
};

#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    size = glm::vec3(1.0f,1.0f,1.0f);
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other) {
    
    if (isActive == false || other->isActive == false) return false;
    
    if (other == this) return false;
    
    float x_dist = fabs(position.x - other->position.x) - (width + other->width) / 2.0f;
    float y_dist = fabs(position.y - other->position.y) - (height + other->height) / 2.0f;
    
    if (x_dist < 0 && y_dist < 0) {
        lastCollision = other->entityType;
        return true;
    }
    
    return false;
    
}

bool Entity::PitCheck(glm::vec3 sensor, Entity *platforms, int platformCount) {
    
    for (int i = 0; i < platformCount; i++) {
        Entity *platform = &platforms[i];
        glm::vec3 topLeft = glm::vec3(platform->position.x - 0.5, platform->position.y + 0.5, 0);
        glm::vec3 bottomRight = glm::vec3(platform->position.x + 0.5, platform->position.y - 0.5, 0);
        
        if (sensor.x > topLeft.x && sensor.x < bottomRight.x && sensor.y < topLeft.y && sensor.y > bottomRight.y) {
            return true;
        }
    }
    return false;
}

 void Entity::CheckCollisionsY(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];
        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
//            if (object->entityType == ENEMY) {
//
//            }
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                collisionTop = object->entityType;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                collisionBottom = object->entityType;
            }
            if (object->entityType == ENEMY) {
                //std::cout << "yes\n";
                //std::cout << "yes\n";
                object->isActive = false;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];
        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                collisionRight = object->entityType;
                
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                collisionLeft = object->entityType;
            }
            //
            if (object->entityType == ENEMY) {
                // if player is not moving
                if (object->position.x > position.x && (object->position.y <= position.y) && collisionBottom == PLATFORM) {
                    collidedRight = true;
                    
                }
                else if (object->position.x < position.x && (object->position.y <= position.y) && collisionBottom == PLATFORM) {
                    collidedLeft = true;
                }
            }
            
            
            if (object->entityType == ENEMY && (collidedLeft || collidedRight)) {
                
                isActive = false;
            }
            
        }
    }
}


void Entity::AIWalker(glm::vec3 sensorRight, glm::vec3 sensorLeft, Entity *platforms, int platformCount) {
    
    if (!PitCheck(sensorLeft, platforms, platformCount)) {
        animIndices = animRight;
        movement = abs(movement);
        //std::cout << movement.x << '\n';
    } else if (!PitCheck(sensorRight, platforms, platformCount)) {
        animIndices = animLeft;
        movement = -abs(movement);
    }
}

void Entity::AIWaitAndGo(Entity *player) {
    switch (aiState) {
        case IDLE:
            if (!player->isActive) {
                movement = glm::vec3(0);
            }
            if (glm::distance(position, player->position) < 3.0) {
                aiState =  WALKING;
            }
            break;
            
        case WALKING:
            if (player->position.x < position.x && player->isActive ) {
                movement = glm::vec3(-1,0,0);
                animIndices = animLeft;
            } else if (player->position.x > position.x && player->isActive) {
                movement = glm::vec3(1,0,0);
                animIndices = animRight;
            }
            else if (!player->isActive) {
                aiState = IDLE;
            }
            
            break;
        
        case ATTACKING:
            break;
            
        
    }
}

void Entity::AIJumper(Entity *platforms, int platformCount) {
    for(int i = 0; i < platformCount; i++) {
        Entity *platform = &platforms[i];
        if (CheckCollision(platform)) {
            collidedBottom = true;
        }
    }
    if (collidedBottom) {
        jump = true;
    }
    //movement.x = 1.0f;
    //std::cout << acceleration.y << '\n';
}

void Entity::AI(Entity *player, Entity *platforms, int platformCount, glm::vec3 sensorRight, glm::vec3 sensorLeft) {
    switch (aiType) {
        case WAITANDGO:
            AIWaitAndGo(player);
            break;
        case STAND:
            break;
        case WALKER:
            AIWalker(sensorRight, sensorLeft, platforms, platformCount);
            break;
        case JUMPER:
            jumpPower = 4.0f;
            AIJumper(platforms, platformCount);
            break;
    }
}

void Entity::Update(float deltaTime, Entity *player, Entity *platforms, int platformCount, Entity *enemies, int enemyCount)
{
    
    if (isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    collisionRight = NONE;
    collisionLeft = NONE;
    collisionTop = NONE;
    collisionBottom = NONE;
    
    
    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        } else {
            animIndex = 0;
        }
    }
    
    if(jump) {
        jump = false;
        velocity.y += jumpPower;
    }
    
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime; // Move on Y
    
    CheckCollisionsY(platforms, platformCount);// Fix if needed
    
    position.x += velocity.x * deltaTime; // Move on X
    glm::vec3 sensorRight = glm::vec3(position.x + 0.35, position.y - 0.6f, 0);
    glm::vec3 sensorLeft = glm::vec3(position.x - 0.35, position.y - 0.6f, 0);
    
    if(entityType == ENEMY) {
        AI(player, platforms, platformCount, sensorRight, sensorLeft);
    }
    CheckCollisionsX(platforms, platformCount);// Fix if needed
    
    CheckCollisionsX(enemies, enemyCount);
    CheckCollisionsY(enemies, enemyCount);


    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, size);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

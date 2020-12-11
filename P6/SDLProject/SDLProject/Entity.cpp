#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other) {
    
    if (other == this) return false;
    
    if (isActive == false || other->isActive == false) return false;
    
    float x_dist = fabs(position.x - other->position.x) - (width + other->width) / 2.0f;
    float y_dist = fabs(position.y - other->position.y) - (height + other->height) / 2.0f;
    
    if (x_dist < 0 && y_dist < 0)
    {
        return true;
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
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
             
            } else if (state == ATTACKING) {
                if (object->position.y > position.y && (animIndices == animAttackUR || animIndices == animAttackUL)) {
                    collidedTop = true;
                    
                }
                /*
                else if (object->position.y < position.y && (animIndices == animAttackL || animIndices == animAttackR) && object->position.x != position.y) {
                    collidedBottom = true;
                }
                 */
                
            }
            else if (object->position.y < position.y) {
                collidedBottom = true;
            }
            else if (object->position.y > position.y) {
                collidedTop = true;
            }
            
            
            if (object->entityType == ENEMY && entityType == PLAYER) {
                if (collidedTop && (animIndices == animAttackUL || animIndices == animAttackUR) ) {
                    Mix_PlayChannel(-1,monster_die,0);
                    object->isActive = false;
                    
                }
                else if (collidedTop || collidedBottom) {
                    Mix_PlayChannel(-1,player_die,0);
                    isActive = false;
                }
            }
            
            if (object->entityType == KEY && entityType == PLAYER) {
                Mix_PlayChannel(-1,collect,0);
                object->isActive = false;
                key = 1;
            }
            if (object->entityType == BOSS_KEY && entityType == PLAYER) {
                Mix_PlayChannel(-1,collect,0);
                object->isActive = false;
                boss_key = 1;
            }
            if (object->entityType == FISHING_ROD && entityType == PLAYER) {
                Mix_PlayChannel(-1,collect,0);
                object->isActive = false;
                mystery = 1;
            }
            if (object->entityType == RING && entityType == PLAYER) {
                Mix_PlayChannel(-1,collect,0);
                object->isActive = false;
                missing_object = 1;
            }
            
            lastCollision = object->entityType;
            
            
         
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
                position.x -= penetrationX + 0.01;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX + 0.01;
                velocity.x = 0;
                collidedLeft = true;
            } else if (state == ATTACKING) {
                if (object->position.x > position.x && (animIndices == animAttackR)) {
                    // only collide if attack in correct position
                    collidedRight = true;
                    
                } else if (object->position.x < position.x && (animIndices == animAttackL)) {
                    collidedLeft = true;
                }
            }
            else if (object->position.x > position.x) {
                collidedRight = true;
                
            }
            else if (object->position.x < position.x) {
                collidedLeft = true;
            }
            
            if (entityType == KEY) return;
            if (object->entityType == ENEMY && entityType == PLAYER) {
                if (collidedLeft && (animIndices == animAttackL) ) {
                    Mix_PlayChannel(-1,monster_die,0);
                    object->isActive = false;
                } else if (collidedRight && (animIndices == animAttackR) ) {
                    Mix_PlayChannel(-1,monster_die,0);
                    object->isActive = false;
                }
                else if (collidedLeft || collidedRight){
                    Mix_PlayChannel(-1,player_die,0);
                    isActive = false;
                }
            }
            if (object->entityType == KEY && entityType == PLAYER) {
                object->isActive = false;
                Mix_PlayChannel(-1,collect,0);
                key = 1;
            }
            if (object->entityType == BOSS_KEY && entityType == PLAYER) {
                object->isActive = false;
                Mix_PlayChannel(-1,collect,0);
                boss_key = 1;
            }
            if (object->entityType == FISHING_ROD && entityType == PLAYER) {
                Mix_PlayChannel(-1,collect,0);
                object->isActive = false;
                mystery = 1;
            }
            if (object->entityType == RING && entityType == PLAYER) {
                Mix_PlayChannel(-1,collect,0);
                object->isActive = false;
                missing_object = 1;
            }

            lastCollision = object->entityType;
            return;
            //std::cout << lastCollision << '\n';
        
        }
    }
}

void Entity::CheckCollisionsY(Map *map)
{
    // Probes for tiles
    // adjust width and height of entity to fix platform collision
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
        
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
        
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
        //std::cout << "yes\n"; // problem
    }

    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        
        
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        
    }
    
    if (map->IsPond(top) || map->IsPond(top_left) || map->IsPond(top_right) || map->IsPond(bottom) || map->IsPond(bottom_left) || map->IsPond(bottom_right)) {
        lastCollision = POND;
        int x_pos = position.x + 3;
        int y_pos = position.y;
        
        if (missing_object_appear == 0) {
            ring_pos = glm::vec3(x_pos, y_pos, 0);
            missing_object_appear = 1;
        }
        
        
    }
    
  
}

void Entity::CheckCollisionsX(Map *map)
{
    //Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
        
    }
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x + 0.01;
        velocity.x = 0;
        collidedRight = true;
        //std::cout << "yes\n";
    }
    if (map->IsPond(left) || map->IsPond(right)) {
        lastCollision = POND;
        int x_pos;
        if (collidedRight) {
            x_pos = position.x - 1;
        } else {
            x_pos = position.x + 2;
            
        }
        int y_pos = position.y;
        //ring_pos = glm::vec3(x_pos, y_pos, 0);
        
        if (missing_object_appear == 0) {
            ring_pos = glm::vec3(x_pos, y_pos, 0);
            missing_object_appear = 1;
        }
        /*
        if (missing_object == 0) {
            ring_pos = glm::vec3(x_pos, y_pos, 0);
            missing_object = 1;
        }
         */
        
        
    }

}

void Entity::AIWalkerY() {
    
    if (collidedBottom | collidedTop) {
        movement.y = -movement.y;
    } else {
        if (velocity.y == 0) {
            movement = glm::vec3(0,-1,0);
        }
    }
    
}

void Entity::AIWalkerX() {
    if (collidedLeft | collidedRight) {
        movement.x = -movement.x;
    } else {
        if (velocity.x == 0) {
            movement = glm::vec3(1,0,0);
        }
    }
}

void Entity::AIWaitAndGo(Entity *player) {
    switch (state) {
        case IDLE:
            if (glm::distance(position, player->position) < 4.3) {
                state =  WALKING;
            }
            break;
            
        case WALKING:
            if (player->isActive == false) {
                movement = glm::vec3(0);
                state = IDLE;
            }
            else if (player->position.x < position.x ) {
                movement = glm::vec3(-1,0,0);
            }
            else if (player->position.x > position.x ){
                movement = glm::vec3(1,0,0);
            }
            else if (player->isActive == false) {
                state = IDLE;
            }
            break;
        
        case ATTACKING:
            break;
    }
    
}
void Entity::AI(Entity *player) {
    switch (aiType) {
        case WALKERY:
            AIWalkerY();
            break;
        case WALKERX:
            AIWalkerX();
            break;
        case WAITANDGO:
            AIWaitAndGo(player);
            break;
    }
}


//replace objects w/ enemies and add objects parameter
void Entity::Update(float deltaTime, Entity *player, Entity *enemies, int enemyCount, Entity *objects, int objectCount, Map *map)
{
    
    if (isActive == false) return;
    
    if(entityType == ENEMY) {
        AI(player);
    }
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    
    
    if (animIndices != NULL) {
        if (state == ATTACKING) {
            animTime += deltaTime;
            if (animTime >= 0.25f)
            {
                
                animTime = 0.0f;
                animIndex++;
                //std::cout << animIndex << '\n';
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                    if (animIndices == animAttackR) {
                        animIndices = animRight;
                    } else if (animIndices == animAttackL) {
                        animIndices = animLeft;
                    }
                    else if (animIndices == animAttackUL) {
                        animIndices = animUpL;
                    }
                    else if (animIndices == animAttackUR) {
                        animIndices = animUpR;
                    } else {
                        animIndices = animRight;
                    }
                    
                    state = IDLE;
                    width = 0.4f;
                    height = 0.4f;
                    
                    
                }
            }
        } else {
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
            
    }
        
    
    if (entityType == PLAYER && state == IDLE) {
        width = 0.4;
        height = 0.4;
    }
    
    velocity.x = movement.x * speed;
    velocity.y = movement.y * speed;
    
    position.y += velocity.y * deltaTime;
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount);
    CheckCollisionsY(enemies, enemyCount);
    
    position.x += velocity.x * deltaTime;
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount);
    CheckCollisionsX(enemies, enemyCount);
    
    modelMatrix = glm::mat4(1.0f);
   
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    
    
    if (entityType == PLAYER) {
        //std::cout << "w: " << width << '\n';
        //std::cout << "h: " << height << '\n';
        //std::cout << "state: " << state << '\n';
        std::cout << "x: " << position.x << '\n';
        std::cout << "y: " << position.y << '\n';
    }
    
    
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

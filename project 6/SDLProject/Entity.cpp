#include "Entity.h"


#include "Scene.h"

Entity::Entity()
{
    
    speed = 0;
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other){
    if (other == this) return false;
    if (isActive == false || other->isActive == false ) return false;
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) {
        //lastCollided = other->theType;
        return true;
    }
    return false;
}


void Entity::CheckCollisionsY(Entity *objects, int objectCount){
    for (int i = 0; i < objectCount; i++){
        Entity *object = &objects[i];
        
        if (CheckCollision(object)){
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            
            if (velocity.y > 0) { //we are going up.. and we have collided with something.. top was collided
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                if (this->entityType == PLAYER &&object -> entityType == STAR){
                    object->isActive = false;
                }
                
            }
            else if (velocity.y < 0) { //moving down meaning we collided with bottom
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                if (this->entityType == PLAYER &&object -> entityType == STAR){
                    object->isActive = false;
                }
            }
            
        }
    }
    
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount){
    for (int i = 0; i < objectCount; i++){
        Entity *object = &objects[i];
        if (CheckCollision(object)){
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if ((velocity.x >= 0 && this->entityType == PLAYER &&object -> entityType == ENEMY) || velocity.x > 0 ) { // we are moving to the right
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                if (this->entityType == PLAYER && object -> entityType == ENEMY){
                    this->defeated = true;
                    if ((this->lives) > 0 )this->lives -=1; //decrease player lives by 1
                    if ((this->lives) >= 1){
                        //std::cout << this->lives << '\n';
                        this->position = glm::vec3(1, -6,0); //bring the player back to the beginning if there
                                                            //are remaining lives
                    }
                }
                else if (this->entityType == PLAYER && object -> entityType == STAR){
                    object->isActive = false;
                }
                
                
            }
            
            
            
            else if (velocity.x < 0) {
                //we are moving left
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                if (this->entityType == PLAYER && object -> entityType == ENEMY){
                    this->defeated = true;
                    if ((this->lives) > 0 )this->lives -=1; //decrease player lives by 1
                    if ((this->lives) >= 1){
                        //std::cout << this->lives << '\n';
                        this->position = glm::vec3(1, -6,0); //bring the player back to the beginning if there
                        //are remaining lives
                    }
                    //
                   
                    
                }
                else if(this->entityType == PLAYER && object -> entityType == STAR){
                    object->isActive = false;
                }
            }
            
        }
    }
    
}

void Entity::CheckCollisionsY(Map *map)
{
    // Probes for tiles
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
    }
    
    /*
    //new addition start
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.x > 0) {
        // Check if we are going to the right
        position.x -= penetration_x; // move us back to the left
        velocity.x = 0;
        collidedRight = true;
    }
    
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.x < 0) {
        // Check if we are going to the left
        position.x += penetration_x; // move us back to the right
        velocity.x = 0;
        collidedLeft = true;
    }
    //new addition end
    */
    
    
    
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
        //
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    
    /*
    //new addition start
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.x > 0) {
        // Check if we are going to the right
        position.x -= penetration_x; // move us back to the left
        velocity.x = 0;
        collidedRight = true;
    }
    
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.x < 0) {
        // Check if we are going to the left
        position.x += penetration_x; // move us back to the right
        velocity.x = 0;
        collidedLeft = true;
    }
    //new addition end
    */
    
    
}


void Entity::CheckCollisionsX(Map *map)
{
    // Probes for tiles
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
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
    
}





void Entity::AIWalker(){
    movement = glm::vec3(-1,0,0);
    if (0.5f < abs(initPosX - position.x)) {
        //std::cout<< initPosX << '\n';
        //std::cout<< "abs: " << abs(initPosX - position.x) << '\n';
        speed = -speed;
    }
}

void Entity::AIWaitAndGo(Entity* player){
    switch (aiState) {
        case IDLE:
            if (glm::distance(position, player->position)<3.0f){
                aiState = WALKING;
            }
            break;
            
        case WALKING:
            if(player->position.x < position.x){
                movement = glm::vec3(-1,0,0);
            }
            else{
               movement = glm::vec3(1,0,0);
            }
            
            break;
        case ATTACKING:
            break;
    }
}

void Entity::Jumper(Entity* player){
   
    if (velocity.y == 0){
        velocity = glm::vec3(0,5,0);
    }
}


void Entity::AI(Entity* player){
    switch (aiType) {
        case WALKER:
            AIWalker();
            break;
        case WAITANDGO:
            AIWaitAndGo(player);
            break;
        case JUMPER:
            Jumper(player);
            break;
    }
}

void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount, Map *map, Entity *stars, int starCount){
    if(isActive==false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if (entityType == ENEMY){
        AI(player);
    }
    
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
    
    if(jump){
        jump = false;
        velocity.y += jumpPower;
    }
    
    velocity.x = movement.x*speed;
    velocity += acceleration*deltaTime; //without this it doesn't move up
    
    if (entityType == PLAYER){
        velocity.y = movement.y * speed; //without this up and down keys don't work
        //for the player
    }
    
    position.y += velocity.y * deltaTime;
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount);
    CheckCollisionsY(stars, starCount);
    position.x += velocity.x * deltaTime;
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount);
    CheckCollisionsX(stars, starCount);
    // Move on Y
    // Fix if needed
    // Move on X
    // Fix if needed
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
      
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index){
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
    
    if (isActive==false) return;
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

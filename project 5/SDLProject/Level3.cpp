#include "Level3.h"
#define LEVEL3_WIDTH 18
#define LEVEL3_HEIGHT 8


#define LEVEL3_ENEMY_COUNT 1

GLuint fontTextureID4;

unsigned int level3_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,3,
    3, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,3,
    3, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,0,0,0,3,
    3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0,1,1,1,3,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,2,2,2,3,
};





void Level3::Initialize() {
    state.nextScene = -1; 
    fontTextureID4=Util::LoadTexture("font1.png");
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    // Initialize Player
    state.player = new Entity();
    state.player -> entityType = PLAYER;
    state.player->position = glm::vec3(2, 0,0);//player's start position
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    state.player->acceleration = glm::vec3(0,-9.0,0);
    
    
    
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
    
    
    state.player->height=0.8;
    state.player->jumpPower = 8.0f;
    
    state.player->width=0.6;
    
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position= glm::vec3(5.5,-2,0);
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].acceleration = glm::vec3(0,-11.0f,0);
    state.enemies[0].velocity = glm::vec3(0,2,0);
    
    
 
    
}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    for (int i =0; i< LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    }
    
    if (state.player->position.y < -10){
        
        if ((state.player->lives) > 0 )state.player->lives -=1; //decrease player lives by 1
        if ((state.player->lives) >= 1){
            //std::cout << this->lives << '\n';
            state.player->position = glm::vec3(2, 0,0);
        }
    }
    
   
}


void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    
   
    
    if (state.player->position.x < 5){
        Util::DrawText(program, fontTextureID4, "Lives:" + std::to_string(state.player->lives) , 0.5f, -0.2f, glm:: vec3(7.5, -0.5,0.0f));
    }
    
    else if (state.player->position.x >= 5 && state.player->position.x < 12) {
        Util::DrawText(program, fontTextureID4, "Lives:" + std::to_string(state.player->lives) , 0.5f, -0.2f, glm:: vec3(state.player->position.x+2.5, -0.5,0.0f));
    }
    
    else if (state.player->position.x >= 12) {
        Util::DrawText(program, fontTextureID4, "Lives:" + std::to_string(state.player->lives) , 0.5f, -0.2f, glm:: vec3(12+2.5, -0.5,0.0f));
    }
    
    
    
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++ ) {
        state.enemies[i].Render(program);
    }
    
    if (state.player ->lives == 0){
        if (state.player->position.x < 5.5){
            Util::DrawText(program, fontTextureID4, "YOU LOST" , 2.0f, -1.0f, glm:: vec3(2, -2.5,0.0f));
        }
        else if (state.player->position.x >= 5.5 && state.player->position.x < 12){
            Util::DrawText(program, fontTextureID4, "YOU LOST" , 2.0f, -1.0f, glm:: vec3(3, -2.5,0.0f));
        }
        
        else{
            Util::DrawText(program, fontTextureID4, "YOU LOST" , 2.0f, -1.0f, glm:: vec3(9, -2.5,0.0f));
        }
        
        state.paused = true;
    }
    
    if (state.player->position.x >= 15){
        Util::DrawText(program, fontTextureID4, "YOU WON" , 2.0f, -0.9f, glm:: vec3(9, -2.5,0.0f));
        state.paused = true;
    }
     
    
    
}


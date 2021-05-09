#include "Level3.h"
#define LEVEL3_WIDTH 20
#define LEVEL3_HEIGHT 8


#define LEVEL3_ENEMY_COUNT 2
#define LEVEL3_STAR_COUNT 11

GLuint fontTextureID4;

unsigned int level3_data[] ={
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,3,3,3,3,3,3,
    3, 0, 0, 0, 0, 2, 0, 0, 3, 3, 0, 0, 0, 0,0,0,0,0,0,3,
    3, 0, 2, 0, 2, 2, 0, 0, 3, 3, 0, 0, 2, 0,0,0,1,1,1,3,
    3, 0, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0,0,0,1,0,0,3,
    3, 0, 2, 0, 2, 2, 0, 0, 1, 1, 0, 0, 2, 0,0,0,1,0,0,3,
    3, 0, 2, 0, 2, 2, 0, 0, 1, 1, 0, 0, 2, 0,0,0,1,0,0,3,
    3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,0,0,0,0,0,3,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,2,2,2,2,2,3,
};




void Level3::Initialize() {
    state.nextScene = -1;
    
    fontTextureID4=Util::LoadTexture("font1.png");
    
    GLuint mapTextureID = Util::LoadTexture("tileset_new.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    // Initialize Player
    state.player = new Entity();
    state.player -> entityType = PLAYER;
    state.player->position = glm::vec3(1, -6,0);//player's start position
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("playerImg2.png");
    
    
    
    
   
    
   
    
    
    
    state.player->height=0.9;
    state.player->width=0.8;
    
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ufo.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position= glm::vec3(6, -3,0);
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].acceleration = glm::vec3(0,-3.0f,0);
    state.enemies[0].velocity = glm::vec3(0,1,0);
    //state.enemies[0].isActive=false;
    
    
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position= glm::vec3(14.5, -4,0);
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].acceleration = glm::vec3(0,-3.0f,0);
    state.enemies[1].velocity = glm::vec3(0,2.5,0);
    //state.enemies[1].isActive=false;
    
    state.stars = new Entity[LEVEL3_STAR_COUNT];
    GLuint starTextureID = Util::LoadTexture("meteor_DetailedLarge.png");
    state.stars[0].entityType = STAR;
    state.stars[0].textureID = starTextureID;
    state.stars[0].position= glm::vec3(1,-1,0);
    
    
    state.stars[1].entityType = STAR;
    state.stars[1].textureID = starTextureID;
    state.stars[1].position= glm::vec3(3,-6,0);
    
    state.stars[2].entityType = STAR;
    state.stars[2].textureID = starTextureID;
    state.stars[2].position= glm::vec3(9,-6,0);
    
    state.stars[3].entityType = STAR;
    state.stars[3].textureID = starTextureID;
    state.stars[3].position= glm::vec3(8,-3,0);
    
    state.stars[4].entityType = STAR;
    state.stars[4].textureID = starTextureID;
    state.stars[4].position= glm::vec3(11,-6,0);
    
    state.stars[5].entityType = STAR;
    state.stars[5].textureID = starTextureID;
    state.stars[5].position= glm::vec3(10,-1,0);
    
    state.stars[6].entityType = STAR;
    state.stars[6].textureID = starTextureID;
    state.stars[6].position= glm::vec3(13,-6,0);
    
    state.stars[7].entityType = STAR;
    state.stars[7].textureID = starTextureID;
    state.stars[7].position= glm::vec3(18,-6,0);
    
    state.stars[8].entityType = STAR;
    state.stars[8].textureID = starTextureID;
    state.stars[8].position= glm::vec3(17,-4,0);
    
    state.stars[9].entityType = STAR;
    state.stars[9].textureID = starTextureID;
    state.stars[9].position= glm::vec3(17,-3,0);
    
    state.stars[10].entityType = STAR;
    state.stars[10].textureID = starTextureID;
    state.stars[10].position= glm::vec3(18,-1,0);
    
    

}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map, state.stars, LEVEL3_STAR_COUNT);
    
    for (int i =0; i< LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map, state.stars, LEVEL3_STAR_COUNT);
    }
    
    for (int i =0; i< LEVEL3_STAR_COUNT; i++) {
        state.stars[i].Update(deltaTime, state.player, state.enemies,  LEVEL3_ENEMY_COUNT, state.map, state.stars, LEVEL3_STAR_COUNT);
    }
    
   //here
    if (state.stars[0].isActive == false &&state.stars[1].isActive == false &&state.stars[2].isActive == false &&
        state.stars[3].isActive == false && state.stars[4].isActive == false && state.stars[5].isActive == false &&
        state.stars[6].isActive == false && state.stars[7].isActive == false && state.stars[8].isActive == false &&
        state.stars[9].isActive == false && state.stars[10].isActive == false){
        state.nextScene = 4;
    }
}


void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    
    if (state.player->position.x < 5){
        Util::DrawText(program, fontTextureID4, "Lives:" + std::to_string(state.player->lives) , 0.5f, -0.2f, glm:: vec3(7.5, -0.5,0.0f));
    }
    
    else if (state.player->position.x >= 5 && state.player->position.x <14) {
        Util::DrawText(program, fontTextureID4, "Lives:" + std::to_string(state.player->lives) , 0.5f, -0.2f, glm:: vec3(state.player->position.x+2.5, -0.5,0.0f));
    }
    
    else{
        Util::DrawText(program, fontTextureID4, "Lives:" + std::to_string(state.player->lives) , 0.5f, -0.2f, glm:: vec3(16.5, -0.5,0.0f));
    }
    
    
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++ ) {
        state.enemies[i].Render(program);
    }
    
    for (int i = 0; i < LEVEL3_STAR_COUNT; i++ ) {
        state.stars[i].Render(program);
    }
    
    
   
    
    if (state.player ->lives == 0){
        if (state.player->position.x >= 5 && state.player->position.x <= 6){
            Util::DrawText(program, fontTextureID4, "YOU LOST" , 2.0f, -1.0f, glm:: vec3(2, -2.5,0.0f));
            //std::cout<<"here3";
        }
        
        else if (state.player->position.x >6 && state.player->position.x < 8){
            Util::DrawText(program, fontTextureID4, "YOU LOST" , 2.0f, -1.0f, glm:: vec3(3.5, -2.5,0.0f));
            
        }
        
        else if (state.player->position.x > 14){
            Util::DrawText(program, fontTextureID4, "YOU LOST" , 2.0f, -1.0f, glm:: vec3(10.5, -2.5,0.0f));
            
        }
        
        
        state.paused = true;
    }
    
    
}


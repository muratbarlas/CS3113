#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8


#define LEVEL1_ENEMY_COUNT 1

GLuint fontTextureID2;

unsigned int level1_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};




void Level1::Initialize() {
    state.nextScene = -1;
    
    fontTextureID2=Util::LoadTexture("font1.png");

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    // Initialize Player
    state.player = new Entity();
    state.player -> entityType = PLAYER;
    state.player->position = glm::vec3(2, 0,0);//player's start position
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    state.player->acceleration = glm::vec3(0,-2.0,0);
    //state.player->defeated = true;
    
    
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
    state.player->jumpPower = 2.0f;
    
    
    
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    
    state.enemies[0].entityType = ENEMY;
    
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position= glm::vec3(7,-5,0);
    
   
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].acceleration = glm::vec3(0,-11.0f,0);
    state.enemies[0].velocity = glm::vec3(0,2,0);
    
    
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    for (int i =0; i< LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    }
    
    if (state.player->position.x >=12){
        state.nextScene = 1;
    }
}


void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
   
    
    if (state.player->position.x < 5){
        Util::DrawText(program, fontTextureID2, "Lives: " + std::to_string(state.player->lives) , 0.4f, 0.0f, glm:: vec3(5.5, -0.5,0.0f));
    }
    
    else if (state.player->position.x > 5) {
        Util::DrawText(program, fontTextureID2, "Lives:" + std::to_string(state.player->lives) , 0.3f, 0.0f, glm:: vec3(state.player->position.x+0.5, -0.5,0.0f));
    }
    
   
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++ ) {
        state.enemies[i].Render(program);
    }
    
    
}

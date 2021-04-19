#include "LevelMenu.h"

#define LEVELMENU_WIDTH 0
#define LEVELMENU_HEIGHT 0
#define LEVELMENU_ENEMY_COUNT 0

unsigned int LEVELMENU_data[] = {0};

GLuint fontTextureID5;

void LevelMenu::Initialize() {
    
    state.nextScene = -1;
    fontTextureID5=Util::LoadTexture("font1.png");
    state.map = new Map(LEVELMENU_WIDTH, LEVELMENU_HEIGHT, LEVELMENU_data, NULL, 1.0f, 4, 1);
    
    state.player = new Entity();
    state.player->isActive = false;
    
}

void LevelMenu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVELMENU_ENEMY_COUNT, state.map);
    
    if (state.enterPressed == true) {
        std::cout<<"pressed enter"<<'\n';
    }
}

void LevelMenu::Render(ShaderProgram *program) {
    
    state.map->Render(program);
    state.player->Render(program);
    
    Util::DrawText(program,  fontTextureID5, "MONSTER DASH", 1.1f, -0.26f, glm:: vec3(0.5, -2,0.0f));
    Util::DrawText(program,  fontTextureID5, "AVOID BEING TOUCHED BY MONSTERS", 0.4f, -0.10f, glm:: vec3(0.65, -4,0.0f));
    
    Util::DrawText(program,  fontTextureID5, "BUT, YES. YOU MAY JUMP ON THEM.", 0.4f, -0.13f, glm:: vec3(1.2, -4.75,0.0f));
    Util::DrawText(program,  fontTextureID5, "AVOID FALLING", 0.4f, 0.0f, glm:: vec3(2.5, -5.5,0.0f));
    Util::DrawText(program,  fontTextureID5, "PRESS ENTER TO BEGIN", 0.6f, -0.20f, glm:: vec3(1.3, -6.5,0.0f));
    
}

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
        state.nextScene = 1;
    }
}

void LevelMenu::Render(ShaderProgram *program) {
    
    state.map->Render(program);
    state.player->Render(program);
    
    Util::DrawText(program,  fontTextureID5, "MENU TEST", 1.1f, -0.2f, glm:: vec3(0.7, -2.5,0.0f));
    
}

#include "LevelMenu.h"


unsigned int LEVELMENU_data[] = {};

GLuint fontTextureID5;

void LevelMenu::Initialize() {
    state.nextScene = -1;
    fontTextureID5=Util::LoadTexture("font1.png");
    state.player = new Entity();
    state.player->isActive = false;
    
}

void LevelMenu::Update(float deltaTime) {
    if (state.enterPressed == true) {
        //std::cout<<"pressed enter"<<'\n';
        state.nextScene = 1;
    }
}

void LevelMenu::Render(ShaderProgram *program) {
    Util::DrawText(program,  fontTextureID5, "SPACE MAZE", 1.1f, -0.55f, glm:: vec3(2.5, -1.5,0.0f));
    Util::DrawText(program,  fontTextureID5, "Press Enter to start", 0.8f, -0.45f, glm:: vec3(1.9, -2.5,0.0f));
    Util::DrawText(program,  fontTextureID5, "Move with the arrow keys", 0.8f, -0.45f, glm:: vec3(1, -3.5,0.0f));
    Util::DrawText(program,  fontTextureID5, "Collect all the astroids to win", 0.8f, -0.5f, glm:: vec3(0.5, -4.5,0.0f));
    Util::DrawText(program,  fontTextureID5, "Don't touch the green aliens", 0.8f, -0.5f, glm:: vec3(1, -5.5,0.0f));
}

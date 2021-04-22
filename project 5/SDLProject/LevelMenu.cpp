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
    Util::DrawText(program,  fontTextureID5, "GEORGE ON THE RUN", 1.1f, -0.55f, glm:: vec3(0.6, -2.5,0.0f));
    Util::DrawText(program,  fontTextureID5, "Press Enter to start", 0.8f, -0.45f, glm:: vec3(1.9, -3.5,0.0f));
    Util::DrawText(program,  fontTextureID5, "Don't fall or hit the AIs!", 0.6f, -0.35f, glm:: vec3(2.2, -5.5,0.0f));
    Util::DrawText(program,  fontTextureID5, "(it's ok to jump on top of them)", 0.6f, -0.35f, glm:: vec3(1.5, -6.5,0.0f));
    
    
}

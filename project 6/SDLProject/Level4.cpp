#include "Level4.h"


unsigned int LEVEL5_data[] = {};

GLuint fontTextureID_end;

void Level4::Initialize() {
    state.nextScene = -1;
    fontTextureID_end=Util::LoadTexture("font1.png");
    state.player = new Entity();
    state.player->isActive = false;
    
}

void Level4::Update(float deltaTime) {
    //if (state.enterPressed == true) {
        //std::cout<<"pressed enter"<<'\n';
        //state.nextScene = 1;
    //}
}

void Level4::Render(ShaderProgram *program) {
    Util::DrawText(program,  fontTextureID_end, "YOU WON!", 1.2f, -0.55f, glm:: vec3(2.5, -2.5,0.0f));
}


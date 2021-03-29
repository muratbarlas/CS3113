//Murat Barlas mlb771 Project 4

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"
#include <vector>

#define PLATFORM_COUNT 19
#define ENEMY_COUNT 2
struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *enemies;
};

GameState state;
GLuint fontTextureID;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool paused = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    // Initialize Game Objects
    fontTextureID = LoadTexture("font1.png");
    
    // Initialize Player
    state.player = new Entity();
    state.player -> entityType = PLAYER;
    state.player->position = glm::vec3(-3, -1,0);//player's start position
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.5f;
    state.player->textureID = LoadTexture("george_0.png");
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
    state.player->jumpPower = 6.0f;
    //state.player->theType = ROCKET;
    state.platforms = new Entity[PLATFORM_COUNT];
    
    GLuint platformTextureID = LoadTexture("platformPack_tile001.png");
    //GLuint landerTextureID = LoadTexture("platformPack_tile007.png");
    
    
    //platforms on the very bottom
    for(int i = 0; i < 11;i++){
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(-5+i,-3.25f,0);
    }
    
    
    //platforms left screen
    
    state.platforms[11].entityType = PLATFORM;
    state.platforms[11].textureID = platformTextureID;
    state.platforms[11].position = glm::vec3(-5,-2.25f,0);
    
    state.platforms[12].entityType = PLATFORM;
    state.platforms[12].textureID = platformTextureID;
    state.platforms[12].position = glm::vec3(-5,-1.25f,0);
    
    state.platforms[13].entityType = PLATFORM;
    state.platforms[13].textureID = platformTextureID;
    state.platforms[13].position = glm::vec3(-5,-0.25f,0);
    
    
    state.platforms[14].entityType = PLATFORM;
    state.platforms[14].textureID = platformTextureID;
    state.platforms[14].position = glm::vec3(-5,0.75f,0);
    
    
    //left mid screen
    
    state.platforms[15].entityType = PLATFORM;
    state.platforms[15].textureID = platformTextureID;
    state.platforms[15].position = glm::vec3(-4,0.25f,0);
    
    state.platforms[16].entityType = PLATFORM;
    state.platforms[16].textureID = platformTextureID;
    state.platforms[16].position = glm::vec3(-3,0.25f,0);
    
    state.platforms[17].entityType = PLATFORM;
    state.platforms[17].textureID = platformTextureID;
    state.platforms[17].position = glm::vec3(-2,0.25f,0);
    
    state.platforms[18].entityType = PLATFORM;
    state.platforms[18].textureID = platformTextureID;
    state.platforms[18].position = glm::vec3(-1,0.25f,0);
    
    
    for (int i=0; i<PLATFORM_COUNT;i++){
        state.platforms[i].Update(0, NULL, NULL, 0, NULL, 0);
    }
    
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("ctg.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position= glm::vec3(4, -2.25,0);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1;
    

    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position= glm::vec3(-3, 1.25,0);
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].acceleration = glm::vec3(0,-11.0f,0);
    state.enemies[1].velocity = glm::vec3(0,2,0);
    
    
    
}

void ProcessInput() {
    
    state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
                        //if(state.player->collidedBottom==true){
                            state.player->jump=true;
                        //}
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT]) {
        //state.player->acceleration.x -= 4;
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        //state.player->acceleration.x += 4;
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
    }
    
    
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }
    
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return; }
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP,state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        
        for (int i =0; i< ENEMY_COUNT;i++){
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        }
         
        
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
}

void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text,
              float size, float spacing, glm::vec3 position){
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    std::vector<float> vertices;
    std::vector<float> texCoords;
    for(int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
        });
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
        });
    } // end of for loop
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}



void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i=0; i<PLATFORM_COUNT;i++){
        state.platforms[i].Render(&program);
        
    }
    for (int i=0; i<ENEMY_COUNT;i++){
        state.enemies[i].Render(&program);
        //if (state.enemies[i].collidedTop == true){
            //state.enemies[i].isActive = false;
        //}
        
    }
    
    if (state.enemies[0].isActive == false && state.enemies[1].isActive == false){
        DrawText(&program, fontTextureID, "YOU WON", 0.7f, -0.3f, glm::vec3(-3.1f, 0.6f,0));
        paused = true;
    }
    
    if (state.player->defeated == true){
        DrawText(&program, fontTextureID, "YOU LOST", 0.7f, -0.3f, glm::vec3(-3.1f, 0.6f,0));
        paused = true;
    }
    /*
    if ((state.player->lastCollided == ENEMY && state.player->collidedLeft == true) || (state.player->lastCollided == ENEMY && state.player->collidedRight == true) ){
        DrawText(&program, fontTextureID, "YOU LOST", 0.7f, -0.3f, glm::vec3(-3.1f, 0.6f,0));
        paused = true;
    }
    */
    
    state.player->Render(&program);
    
   
   
    
    
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        if (paused == false){
            Update();
        }
        Render();
    }
    
    Shutdown();
    return 0;
}

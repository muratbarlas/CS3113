//project 5 Murat Barlas - mlb771
//The goal of this game is avoiding collising with the jumping AIs from the side. It's ok collide with them
//from their top. You can move the main character with the left and right arrow keys and jump using the space bar.

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
#include <SDL_mixer.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "Util.h"
#include "Entity.h"

#include <vector>


#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "LevelMenu.h"



GLuint fontTextureID;
Mix_Music *music;
Mix_Chunk *bounce;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool paused = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene *currentScene;
Scene *sceneList[4];
void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Project 5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    
    
    
    fontTextureID = Util::LoadTexture("font1.png");
    
    
    sceneList[0] = new LevelMenu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]); //this is the scene the game starts with
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME/10); //turns the volume down
    Mix_PlayMusic(music, -1);
    
    
    bounce = Mix_LoadWAV("bounce.wav");
    
   
}

void ProcessInput() {
    
    currentScene->state.player->movement = glm::vec3(0);
    
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
                        if  (currentScene->state.player->collidedBottom == true){
                            currentScene->state.player->jump=true;
                            Mix_PlayChannel(-1, bounce, 0);
                        }
                        break;
                    case SDLK_RETURN:
                       
                        currentScene->state.enterPressed = true;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT]) {
        //state.player->acceleration.x -= 4;
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        //state.player->acceleration.x += 4;
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    
    
    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
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
        currentScene->Update(FIXED_TIMESTEP);
       
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    
    
    viewMatrix = glm::mat4(1.0f);
    
    if (currentScene != sceneList[3]){ //for the first two scenes
        if (currentScene->state.player->position.x > 5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        } else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
        //3.75 moves the world up in the view
    }
    else{//when on last scene
        if ((currentScene->state.player->position.x > 5) && !(currentScene->state.player->position.x >= 12)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        }else if (currentScene->state.player->position.x <= 5)  {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
        else if (currentScene->state.player->position.x >= 12  ) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-12, 3.75, 0));
        }
        
    }
}



void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    //DrawText(&program, fontTextureID, "test ", 0.85f, -0.25f, glm:: vec3(0, 0,0));
    //do util:: for draw text
    
  
    
    currentScene->Render(&program);
    
    
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        if (currentScene->state.paused == false){ //game freezes if all lives are consumed
            Update();
        }
        if (currentScene->state.nextScene >= 0){
            int livesToPassOver  = currentScene->state.player->lives;
            SwitchToScene(sceneList[currentScene->state.nextScene]); //scene switch happens
            currentScene->state.player->lives =livesToPassOver;
        }
        Render();
    }
    
    Shutdown();
    return 0;
}

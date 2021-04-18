//Murat Barlas Project 2 - mlb771
//Right player controles are the arraw keys
//Left player controls are the W and S keys
//Game starts when the player hits the space key

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
#include <cstdlib>
#include <SDL_mixer.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Mix_Music *music;
Mix_Chunk *bounce;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, sunMatrix, rightMonsterMatrix, ballMatrix, brickRightMatrix;

//float player_y = 0;
//float rot_val = 0;


// Start paddle left on the left
glm::vec3 player_position = glm::vec3(-4.47, 0, 0);
// Don’t go anywhere (yet).
glm::vec3 player_movement = glm::vec3(0, 0, 0);

//start paddle right on right
glm::vec3 right_brick_position = glm::vec3(4.47, 0, 0);
//glm::vec3 brick_scale = glm::vec3(1.3f, 2.2f, 1.0f);
glm::vec3 brick_scale = glm::vec3(1.0f, 1.0f, 1.0f);
//brickRightMatrix = glm::scale(brickRightMatrix, glm::vec3(1.3f, 2.2f, 1.3f));
glm::vec3 right_brick_movement = glm::vec3(0, 0, 0);




//ball
glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement=glm::vec3(0, 0, 0);
//glm::vec3 ball_scale = glm::vec3(0.70f, 0.7f, 0.7f);
glm::vec3 ball_scale = glm::vec3(1.0f, 1.0f, 1.0f);
bool ballStarted = false;
bool gameIsGoing = true;


float player_speed = 1.0f;
float ball_speed = 2.5f;


GLuint playerTextureID, sunTextureID,brickRightTextureID, ballTextureID;

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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Pong Game!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f); //left brick
    
    brickRightMatrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    
    //projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.9f, 1.0f, 1.0f); //background color
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    playerTextureID = LoadTexture("brickLeft.png");
    
    brickRightTextureID = LoadTexture("brickRight.png");
    ballTextureID = LoadTexture("ball.png");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME/10); //turns the volume down
    Mix_PlayMusic(music, -1);
    
    
    bounce = Mix_LoadWAV("bounce.wav");
    
}

void ProcessInput() {
    right_brick_movement = glm::vec3(0);
    player_movement = glm::vec3(0);
    //ball_movement = glm::vec3(0);
    
    
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
                        //player_movement.x +=1.0f;
                        break;
                    case SDLK_SPACE:
                        // Some sort of action
                        break;
                }
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    
    if (keys[SDL_SCANCODE_DOWN]) {
        if ( right_brick_position.y >-2.7f&& gameIsGoing == true){
            right_brick_movement.y -=3.0f;
        }
    }
    else if (keys[SDL_SCANCODE_UP]) {
        if(right_brick_position.y<2.7f&& gameIsGoing == true){
            right_brick_movement.y +=3.0f;
        }
    }
    
    if (keys[SDL_SCANCODE_W]) {
        //PlayerLeft();
        if (player_position.y <2.7f && gameIsGoing == true){
            player_movement.y +=3.0f;
        }
    }
    else if (keys[SDL_SCANCODE_S]) {
        if (player_position.y > -2.7f && gameIsGoing == true){
            player_movement.y -=3.0f;
        }
        
    }
    
    
    if (keys[SDL_SCANCODE_SPACE]) {
        if (ballStarted == false){ //so that space key becomes ineffective when the game starts
            ball_movement.x += 1.0f;
            ball_movement.y += 1.0f;
            ballStarted = true;
        }
       
    }
    
    if (glm::length(ball_movement) > 1.0f) {
        ball_movement = glm::normalize(ball_movement);
    }
    
   
    
    
   
    

}



bool checkCollision(){
    float ball_w = ball_scale.x;
    float ball_h = ball_scale.y;
    float paddle_w = brick_scale.x;
    float paddle_h = brick_scale.y;
    
    
    
    float ball_x = ball_position.x;
    float ball_y = ball_position.y;
    float paddle_xR = right_brick_position.x;
    float paddle_yR = right_brick_position.y;
    
    float paddle_xL = player_position.x;
    float paddle_yL = player_position.y;
    
    
   
    float xdistR = fabs(paddle_xR-(ball_x)) - ((ball_w+paddle_w)/ 2.0f);
    float ydistR = fabs(paddle_yR-ball_y) - ((ball_h+paddle_h)/2.0f);
    
    
    float xdistL = fabs(paddle_xL-(ball_x)) - ((ball_w+paddle_w)/ 2.0f);
    float ydistL = fabs(paddle_yL-ball_y) - ((ball_h+paddle_h)/2.0f);
    
    
    //std::cout<< "xdistR " << xdistR << " ydistR " << ydistR << '\n';
    
    if ((xdistR < 0.0 && ydistR <0.0) || (xdistL < 0.0 && ydistL <0.0)){
        return true;
        
    }
    
   
    else{
        return false;
    }
}



float lastTicks = 0.0f;





void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    // Add (direction * units per second * elapsed time)
    
    if (gameIsGoing == true){ //ball stops when someone loses
        ball_position += ball_movement * ball_speed * deltaTime;
        
    }
    player_position += player_movement * player_speed * deltaTime;
    right_brick_position += right_brick_movement * player_speed * deltaTime;
    //ball_position +=ball_movement * player_speed * deltaTime;
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, brick_scale);
    modelMatrix = glm::translate(modelMatrix, player_position);
    
    ballMatrix = glm::mat4(1.0f);
    ballMatrix =  glm::scale(ballMatrix, ball_scale);
    ballMatrix =glm::translate(ballMatrix, ball_position);
    
    brickRightMatrix =glm::mat4(1.0f);
    brickRightMatrix = glm::scale(brickRightMatrix, brick_scale);
    brickRightMatrix = glm::translate(brickRightMatrix, right_brick_position);
    
    //checkCollision(); //checks collision between ball and paddles
    
    if (ballStarted == true && gameIsGoing == true){
        if (ball_position.y >= 3.25f || ball_position.y<= -3.25f){ //hits top or bottom
            ball_movement.y *= -1.0f;
            
            
        }
        
        if (ball_position.x >= 4.5f || ball_position.x<= -4.5){ //hits sides
            gameIsGoing = false;
        }
        
        //std::cout<<"here"<<ball_position.x<<"end";
    }
    
    
    if (checkCollision() == true ){
        ball_movement.x *= -1.0f;
        Mix_PlayChannel(-1, bounce, 0);
        
        //std::cout << "collided" << '\n';
    }
    
}



void drawPaddleLeft(){
    program.SetModelMatrix(modelMatrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawBall(){
    program.SetModelMatrix(ballMatrix);
    glBindTexture(GL_TEXTURE_2D, ballTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawPaddleRight(){
    program.SetModelMatrix(brickRightMatrix);
    glBindTexture(GL_TEXTURE_2D, brickRightTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    drawBall();
    
    float verticesPaddle[]  = { -0.5, -1.0, 0.5, -1.0, 0.5, 1.0, -0.5, -1.0, 0.5, 1.0, -0.5, 1.0 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, verticesPaddle);
    drawPaddleLeft();
    
    drawPaddleRight();
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
   
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}


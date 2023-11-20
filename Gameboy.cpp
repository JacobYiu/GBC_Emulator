#include <SDL.h>   /* All SDL App's need this */
#include <SDL_opengl.h>
#include "Gameboy.h"
#include "Emulator.h"

static const int windowWidth = 160;
static const int windowHeight = 144;

Gameboy* Gameboy::gameboyInstancePtr = nullptr;
Emulator* Gameboy::EmulatorInstancePtr = nullptr;

 
//Used to create gameboy instance
Gameboy* Gameboy::createGameBoyInstance()
{
    if(gameboyInstancePtr == nullptr)
    {
        gameboyInstancePtr = new Gameboy();
        gameboyInstancePtr -> initSDL();
        return gameboyInstancePtr;
    }

    else
    {
        return gameboyInstancePtr;
    }
}


//Used to get gameboyinstance
Gameboy* Gameboy::getGameBoyInstance()
{
    return gameboyInstancePtr;
}

//Private Constructor
Gameboy::Gameboy()
{
    EmulatorInstancePtr = new Emulator();
    EmulatorInstancePtr -> loadCartridge("ROM/Mario.gb");
    EmulatorInstancePtr -> initEmulator(initEmulatorRender);    
}

//Public Delete
Gameboy::~Gameboy()
{
    delete EmulatorInstancePtr;
}

//Used to define the RenderScreen function in the Emulator Class.
//We pass in this function as an argument
void initEmulatorRender()
{
    Gameboy* gbPtr = Gameboy::getGameBoyInstance();
    if(gbPtr)
    {
        gbPtr -> renderGame();
    }
        
    else
    {
        printf("Unable to renderGame because gameboy is not initialized yet");
    }
}

//Implement RenderGame For emulator. 
//Need to research on it
void Gameboy::renderGame()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //Not sure what I am supposed to put here
    
    SDL_GL_SwapWindow(gWindow);
}

/* 
-----------------------------------------------------------------------------------------
-------------------------------Gameboy Simulation----------------------------------------
-----------------------------------------------------------------------------------------
*/

//Used to start Gameboy simulation in the GameBoyRun.cpp file
void Gameboy::startGameboySimulation()
{
    bool quit = false;
    SDL_Event event;

    float fps = 59.8;
    //Interval before frames refresh is 1000miliseconds / 60
    float interval = 1000 / fps;
    unsigned int totalTimeinMiliPassed = SDL_GetTicks64();

    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            handleInput(event);
            if(event.type == quit)
            {
                quit = true;
            }

            //Check frames and if it is the case then update the Emulator
            unsigned int currentTime = SDL_GetTicks64();
            if(totalTimeinMiliPassed + interval < currentTime)
            {
                //Probably need to implement later on when we want to check issues with timing and what not
                // checkFPS();
                totalTimeinMiliPassed = SDL_GetTicks64();
                EmulatorInstancePtr -> Update();
            }
        }
    }

    SDL_Quit();
}

bool Gameboy::initSDL()
{
    SDL_DisplayMode currentDisplayMode;
    SDL_GLContext gContext;

    printf("Initializing SDL.\n");
    
    /* Initialize defaults, Video and Joystick */
    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == -1)) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        return false;
    }

    printf("SDL initialized.\n");

    //Create the Window
    gWindow = SDL_CreateWindow("HEHEHEHE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
    if (gWindow == NULL)
    {
        printf("Failed To Create SDL Window");
        return false;
    }

    printf("Successfully Created The Window\n");

    //Initialize GL
    if(!initGL())
    {
        printf("Unable to initialize openGL!\n");
        return false;
    }

    return true;
}

bool Gameboy::initGL()
{
    GLenum error = GL_NO_ERROR;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        printf( "Error Loading MatrixMode! %s\n", error);
        return false;        
    }

    glOrtho(0, windowWidth, windowHeight, 0, -1.0, 1.0);
    
    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        printf( "Error Setting Up Othographic Camera! %s\n", error);
        return false;        
    }

    //-------------------------------
    //Set Properties
    //-------------------------------

    //Set color to black with opacity 1
    glClearColor(0, 0, 0, 1.0);
    //Clear the buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    //Use a flat shading, no gradient or smoothness
    glShadeModel(GL_FLAT);  

    //Enable 2D GL_Texture
    glEnable(GL_TEXTURE_2D);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DITHER);
    glDisable(GL_BLEND);  

    return true;        
}

void Gameboy::handleInput(SDL_Event &event)
{
    if(event.type == SDL_KEYDOWN)
    {
        int key = -1;
        switch(event.key.keysym.sym)
        {
            case SDLK_a : key = 4        ; break ;
            case SDLK_s : key = 5        ; break ; 
            case SDLK_SPACE : key = 6    ; break ;
            case SDLK_RETURN : key = 7   ; break ;
            case SDLK_RIGHT : key = 0    ; break ;
            case SDLK_LEFT : key = 1     ; break ;
            case SDLK_UP : key = 2       ; break ;
            case SDLK_DOWN : key = 3     ; break ;
        }

        if(key != -1)
        {
            setKeyPressed(key);
        }

    }

    else if (event.type == SDL_KEYUP)
    {
        int key = -1;
        switch(event.key.keysym.sym)
        {
            case SDLK_a : key = 4        ; break ;
            case SDLK_s : key = 5        ; break ; 
            case SDLK_SPACE : key = 6    ; break ;
            case SDLK_RETURN : key = 7   ; break ;
            case SDLK_RIGHT : key = 0    ; break ;
            case SDLK_LEFT : key = 1     ; break ;
            case SDLK_UP : key = 2       ; break ;
            case SDLK_DOWN : key = 3     ; break ;
        }

        if(key != -1)
        {
            setKeyReleased(key);
        }
    }
}

void Gameboy::setKeyPressed(int keyPressed)
{
    EmulatorInstancePtr->keyPressed(keyPressed);
}

void Gameboy::setKeyReleased(int keyReleased)
{
    EmulatorInstancePtr->keyReleased(keyReleased);
}



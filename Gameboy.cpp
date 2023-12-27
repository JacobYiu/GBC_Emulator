#include "Gameboy.h"
#include "utils.h"

static const int windowWidth = 160;
//og value is 144. Bug
static const int windowHeight = 144;

Gameboy* Gameboy::GameboyInstancePtr = nullptr;
Emulator* Gameboy::EmulatorInstancePtr = nullptr;

SDL_Window* Gameboy::gWindow = nullptr;

 
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

//Used to create gameboy instance
Gameboy* Gameboy::createGameBoyInstance()
{
    if(GameboyInstancePtr == nullptr)
    {
        GameboyInstancePtr = new Gameboy();
        GameboyInstancePtr -> initSDL();
        return GameboyInstancePtr;
    }

    else
    {
        return GameboyInstancePtr;
    }
}


//Used to get gameboyinstance
Gameboy* Gameboy::getGameBoyInstance()
{
    return GameboyInstancePtr;
}

//Used to define the RenderScreen function in the Emulator Class.
//We pass in this function as an argument

//Private Constructor
Gameboy::Gameboy()
{
    EmulatorInstancePtr = new Emulator();
    EmulatorInstancePtr -> loadCartridge("cpu_instrs/individual/01-special.gb");
    EmulatorInstancePtr -> initEmulator(initEmulatorRender);    
}

/*
Test Cases
01-special.gb
03-op\ sp,hl.gb
04-op\ r,imm.gb
05-op\ rp.gb
06-ld\ r,r.gb
07-jr,jp,call,ret,rst.gb
08-misc\ instrs.gb
09-op\ r,r.gb
10-bit\ ops.gb
11-op\ a,\(hl\).gb
*/

//Public Delete
Gameboy::~Gameboy()
{
    delete EmulatorInstancePtr;
}

//Implement RenderGame For emulator. 
//Need to research on it

void Gameboy::debugScreenDisplay()
{
    // for(int i = 0; i < windowWidth; i++)
    // {
    //     for(int j = 0; j < windowHeight; j++)
    //     {
    //         for(int k = 0; k < 3; k++)
    //         {
    //             std::cout << EmulatorInstancePtr -> screen_Display[i][j][k] << std::endl;
    //         }
    //     }
    // }

    // std::cout << "Screen Display is.." << std::endl;
    std::cout << EmulatorInstancePtr -> screen_Display[0][0][0] << std::endl; 
    std::cout << EmulatorInstancePtr -> screen_Display[0][0][1] << std::endl;
}

void Gameboy::renderGame()
{
    // std::cout << "Updating Frame" << std::endl;
    //Reset the Color and Depth
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //Reset the current matrix
    glLoadIdentity();
    //Renders it from top to bottom and left to right as expected
    glRasterPos2i(-1, 1);
	glPixelZoom(1, -1);
    glDrawPixels(windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, EmulatorInstancePtr -> screen_Display);
    // std::cout << EmulatorInstancePtr -> screen_Display << std::endl;
    // debugScreenDisplay();
    
    SDL_GL_SwapWindow(gWindow);
    // std::cout <<"Finished Updating Frames" << std::endl;
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

    float fps = 58.7;
    //Interval before frames refresh is 1000miliseconds / 60
    float interval = 1000 / fps;
    unsigned int totalTimeinMiliPassed = SDL_GetTicks64();

    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            handleInput(event);
            if(event.type == SDL_QUIT)
            {
                std::cout << "Quitting....." << std::endl;
                quit = true;
                break;
            }

            // std::cout << "Testing if game is running properly " << SDL_GetTicks64() << std::endl;
        }

        //Check frames and if it is the case then update the Emulator
        unsigned int currentTime = SDL_GetTicks64();
        if(currentTime > totalTimeinMiliPassed + interval)
        {
            //Probably need to implement later on when we want to check issues with timing and what not
            // checkFPS();
            // std::cout << "Time has elapsed " << std::endl;
            EmulatorInstancePtr -> Update();
            totalTimeinMiliPassed = SDL_GetTicks64();
        }
    }

    SDL_Quit();
}

bool Gameboy::initSDL()
{
    printf("Initializing SDL.\n");
    SDL_GLContext gContext;
    
    /* Initialize defaults, Video and Joystick */
    if((SDL_Init(SDL_INIT_EVERYTHING) == -1)) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        return false;
    }

    printf("SDL initialized.\n");

    //Create the Window
    Uint32 SDLFlags = SDL_WINDOW_OPENGL;
    gWindow = SDL_CreateWindow("GameBoy Color", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowHeight, windowHeight, SDLFlags);
    if (gWindow == NULL)
    {
        printf("Failed To Create SDL Window");
        return false;
    }

    printf("Successfully Created The Window\n");

    gContext = SDL_GL_CreateContext(gWindow);
    if(gWindow == NULL)
    {
        printf("Failed to create gContext");
        return false;
    }

    //Initialize GL
    if(!initGL())
    {
        printf("Unable to initialize openGL!\n");
        return false;
    }

    printf("Sucessfully intializeed openGL\n");

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
        printf( "Error Loading MatrixMode! %d\n", error);
        return false;        
    }

    glOrtho(0, windowWidth, windowHeight, 0, -1.0, 1.0);
    
    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        printf( "Error Setting Up Othographic Camera! %d\n", error);
        return false;        
    }

    //-------------------------------
    //Set Properties
    //-------------------------------

    //Set color to black with opacity 1
    glClearColor(83, 0, 0, 1.0);
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
            std::cout << "Key Pressed" << std::endl;
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



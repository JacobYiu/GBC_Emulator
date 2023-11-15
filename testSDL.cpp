#include <SDL.h>   /* All SDL App's need this */
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <stdio.h>

bool initGL(int windowWidth, int windowHeight)
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

bool initSDL()
{
    SDL_DisplayMode currentDisplayMode;
    SDL_Window* gWindow = NULL;
    SDL_GLContext gContext;
    int windowWidth = 160;
    int windowHeight = 144;

    printf("Initializing SDL.\n");
    
    /* Initialize defaults, Video and Joystick */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)==-1)) { 
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

    //Create the Context to render the window and fiddle with it using openGL 
    // gContext = SDL_GL_CreateContext(gWindow);
    // if(gContext == NULL)
    // {
    //     printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
    //     return false;
    // }

    // printf("Successfully Created The Context\n");

    // //VSync. Controls how frequent the front and back buffers of the window are swapped
    // if(SDL_GL_SetSwapInterval(1) < 0)
    // {
    //     printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    // }

    //Initialize GL
    if(!initGL(windowWidth, windowHeight))
    {
        printf("Unable to initialize openGL!\n");
        return false;
    }

    return true;
}

/*
SDL Event

typedef struct{
    Uint32 type; //Event Type indentifier
    ..
    ..
    ..
    ..
    ..
    SDL_KeyboardEvent key; //Keyboard Event Data
}

| |
| |
| |
v v

*/

/*
SDL_KeyboardEvent

typedef struct{
Uint8 type;
Uint8 state;
SDL_keysym keysym; *****
} SDL_KeyboardEvent;

The SDL_KeyboardEvent describes a keyboard event (obviously). 
The key member of the SDL_Event union is a SDL_KeyboardEvent structure. 
The type field specifies whether the event is a key release (SDL_KEYUP) or a key press (SDL_KEYDOWN) event.
The state is largely redundant, it reports the same information as the type field but uses different values (SDL_RELEASED and SDL_PRESSED). 
The keysym contains information of the key press or release that this event represents.

| |
| |
| |
v v

*/

/*
SDL_keysym

typedef struct{
Uint8 scancode;
SDLKey sym;
SDLMod mod;
Uint16 unicode;
} SDL_keysym;

The SDL_keysym structure describes a key press or a key release. 
The scancode field is hardware specific and should be ignored unless you know what your doing. 
The sym field is the SDLKey value of the key being pressed or released. 
The mod field describes the state of the keyboard modifiers at the time the key press or release occurred.  
Finally, the unicode field stores the 16-bit unicode value of the key.
*/
void HandleInput(SDL_Event& event)
{
    if(event.type == SDL_KEYDOWN)
    {
    switch(event.key.keysym.sym)
    {
            case SDLK_a    :  break; 
            case SDLK_SPACE:  break;
            case SDLK_DOWN :  break;
    }

    }

    else if (event.type == SDL_KEYUP)
    {
        printf("Key released");
    }
}

int main(int argc, char *argv[]) {

    bool quit = false;
    SDL_Event event;
    if(!initSDL())
    {
        printf("Failed to Initialize SDL");
        SDL_Quit();
    }

    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            HandleInput(event);
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }

    SDL_Quit();
}
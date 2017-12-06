#include <iostream>

#include "Window.h"
#include <OgreRenderWindow.h>
#include <OgreRoot.h>

Window::Window(){
    //Do all the SDL things here.
    SDL_Surface *surface = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "The window couldn't be created" << std::endl;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    //Create the OGRE window reference here, but don't actually create the window
    Ogre::NameValuePairList params;
    params["currentGLContext"] = "true";
    renderWindow = Ogre::Root::getSingleton().createRenderWindow("Window", 100, 100, false, &params);

}

void Window::update(){
    SDL_PumpEvents();

    updateMouse();
    updateKeys();

    SDL_UpdateWindowSurface(window);
    SDL_GL_SwapWindow(window);
}

void Window::close(){
    renderWindow->destroy();
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Ogre::RenderWindow* Window::getRenderWindow(){
    return renderWindow;
}

void Window::updateMouse(){
    SDL_GetMouseState(&currentX, &currentY);

    offsetX = currentX - prevX;
    offsetY = currentY - prevY;

//    pointCamera(offsetX, offsetY);

    prevX = currentX;
    prevY = currentY;

}

void Window::updateKeys(){
    SDL_PollEvent(&e);

    if(e.type == SDL_QUIT){
        close();
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    if( currentKeyStates[ SDL_SCANCODE_UP ] ){
        std::cout << "Up" << std::endl;
    }
}


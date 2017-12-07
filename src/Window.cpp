#include <iostream>

#include "Window.h"
#include <OgreRenderWindow.h>
#include <OgreRoot.h>

Window::Window(){
    //Do all the SDL things here.
    SDL_Surface *surface = 0;

    for(int i = 0; i < 300; i++){
        keys[i] = false;
    }

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "The window couldn't be created" << std::endl;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    //Create the OGRE window reference here, but don't actually create the window
    Ogre::NameValuePairList params;
    params["currentGLContext"] = "true";
    renderWindow = Ogre::Root::getSingleton().createRenderWindow("Window", width, height, false, &params);

    open = true;

}

void Window::update(){
    SDL_PumpEvents();

    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                close();
                break;
            case SDL_KEYDOWN:
                keyPressed(event);
                break;
            case SDL_KEYUP:
                keyReleased(event);
                break;
            case SDL_WINDOWEVENT:
                switch(event.window.event){
                    case SDL_WINDOWEVENT_RESIZED:
                        resizeWindow(event);
                }
        }
    }

    SDL_UpdateWindowSurface(window);
    SDL_GL_SwapWindow(window);
}

void Window::close(){
    open = false;

    renderWindow->destroy();
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::keyPressed(SDL_Event &event){
    keys[event.key.keysym.scancode] = true;
}

void Window::keyReleased(SDL_Event &event){
    keys[event.key.keysym.scancode] = false;
}

bool Window::getKey(int k){
    return keys[k];
}

Ogre::RenderWindow* Window::getRenderWindow(){
    return renderWindow;
}

bool Window::isOpen(){
    return open;
}

void Window::resizeWindow(SDL_Event &event){
    width = event.window.data1;
    height = event.window.data2;

    renderWindow->resize(width, height);
}


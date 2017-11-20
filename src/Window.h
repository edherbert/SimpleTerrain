#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Ogre.h>
#include <SDL.h>

class Window{
private:
    Ogre::RenderWindow *renderWindow;
    SDL_Window *window;

    void updateMouse();
    void updateKeys();

    int prevX, prevY;
    int offsetX, offsetY;
    int currentX, currentY;

    SDL_Event e;

public:
    Window();
	void update();
    
    void close();

    Ogre::RenderWindow* getRenderWindow();
};

#endif

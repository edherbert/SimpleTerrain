#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Ogre.h>
#include <SDL.h>

#define KEY_W SDL_SCANCODE_W
#define KEY_A SDL_SCANCODE_A
#define KEY_S SDL_SCANCODE_S
#define KEY_D SDL_SCANCODE_D


class Window{
private:
    Ogre::RenderWindow *renderWindow;
    SDL_Window *window;

	int width = 800;
	int height = 600;

    void updateMouse();
    void updateKeys();

    int prevX, prevY;
    int offsetX, offsetY;
    int currentX, currentY;

	void keyPressed(SDL_Event &event);
	void keyReleased(SDL_Event &event);
	void resizeWindow(SDL_Event &event);

    bool open = false;

	bool keys[300];

    SDL_Event e;

public:
    Window();
	void update();

	bool getKey(int k);

    void close();
    bool isOpen();

    Ogre::RenderWindow* getRenderWindow();
};

#endif

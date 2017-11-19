#ifndef GAME_H
#define GAME_H

#include <Ogre.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreHlmsPbs.h>
#include <OgreMeshManager.h>

#include <OgreMeshManager2.h>
#include <OgreMesh2.h>
#include "OgreSubMesh2.h"

class SDL_Window;

class Game
{
    public:
        Game();
        virtual ~Game();

    protected:

    private:
        Ogre::Root *root;
        Ogre::RenderWindow *renderWindow;
        Ogre::SceneManager *sceneManager;
        Ogre::Camera *camera;

        SDL_Window *window;

        Ogre::CompositorWorkspace *workspace;

        Ogre::Real yaw;
        Ogre::Real pitch;

        void captureMouse();

        bool mouseCaptured = false;

        void registerHlms();
        void createSceneManager();
        void createCamera();
        void closeWindow();
        void pointCamera(int xOffset, int yOffset);

        float radians(float value);

        Ogre::CompositorWorkspace* setupCompositor();

        Ogre::MeshPtr createStaticMesh();
        Ogre::IndexBufferPacked* createIndexBuffer(void);

};

#endif // GAME_H

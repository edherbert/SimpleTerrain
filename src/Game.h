#ifndef GAME_H
#define GAME_H

#include <Ogre.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreHlmsPbs.h>

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

        Ogre::CompositorWorkspace *workspace;

        void registerHlms();
        void createSceneManager();
        void createCamera();
        Ogre::CompositorWorkspace* setupCompositor();
};

#endif // GAME_H

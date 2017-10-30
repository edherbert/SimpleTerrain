#ifndef GAME_H
#define GAME_H

#include <Ogre.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreHlmsPbs.h>
#include <OgreMeshManager.h>

#include <OgreMeshManager2.h>
#include <OgreMesh2.h>
#include "OgreSubMesh2.h"

struct CubeVertices
{
    float px, py, pz;   //Position
    float nx, ny, nz;   //Normals

    CubeVertices() {}
    //Populate the floats with values.
    CubeVertices( float _px, float _py, float _pz,
                  float _nx, float _ny, float _nz ) :
        px( _px ), py( _py ), pz( _pz ),
        nx( _nx ), ny( _ny ), nz( _nz )
    {
    }
};

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

        Ogre::MeshPtr createStaticMesh();
        Ogre::IndexBufferPacked* createIndexBuffer(void);

        const CubeVertices c_originalVertices[8] =
        {
            CubeVertices( -1, -1,  1, -0.57737, -0.57737,  0.57737 ),
            CubeVertices(  1, -1,  1,  0.57737, -0.57737,  0.57737 ),
            CubeVertices(  1,  1,  1,  0.57737,  0.57737,  0.57737 ),
            CubeVertices( -1,  1,  1, -0.57737,  0.57737,  0.57737 ),
            CubeVertices( -1, -1, -1, -0.57737, -0.57737, -0.57737 ),
            CubeVertices(  1, -1, -1,  0.57737, -0.57737, -0.57737 ),
            CubeVertices(  1,  1, -1,  0.57737,  0.57737, -0.57737 ),
            CubeVertices( -1,  1, -1, -0.57737,  0.57737, -0.57737 )
        };
};

#endif // GAME_H

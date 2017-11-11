#ifndef GAME_H
#define GAME_H

#include <Ogre.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreHlmsPbs.h>
#include <OgreMeshManager.h>

#include <OgreMeshManager2.h>
#include <OgreMesh2.h>
#include "OgreSubMesh2.h"

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

	//List the vertex data.
	//This represents positions and normals.
        /*const CubeVertices c_originalVertices[4] =
        {
            //CubeVertices( -1, -1,  1, -0.57737, -0.57737,  0.57737 ),
            //CubeVertices(  1, -1,  1,  0.57737, -0.57737,  0.57737 ),
            //CubeVertices(  1,  1,  1,  0.57737,  0.57737,  0.57737 ),
            //CubeVertices( -1,  1,  1, -0.57737,  0.57737,  0.57737 ),
  
        CubeVertices( -1, -1,  0, -0.57737,  0.57737,  0.57737 ),
	    CubeVertices(1, -1, 0, 0.57737, -0.57737, 0.57737),
	    CubeVertices(1, 1, 0, 0.5, 0.5, 0.5),
	    CubeVertices(-1, 1, 0, 0.5, 0.5, 0.5)
        };
        */
	//Sooo... 
	//Generating a flat mesh means four verticies on the side that are only used once, as well as the verticies on the side that are used twice.
	//Other than that, all the other verticies will be used four times. 
    //So try to create a system where you can input a width and height in tiles and then render that.
    
    //I could go through and assign indexes to each vertex before they are actually needed.
    //Actually creating the data will be simple, the indexing might be more complex.
    //So find the values
};

#endif // GAME_H

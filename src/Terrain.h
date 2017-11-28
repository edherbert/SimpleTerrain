#ifndef TERRAIN_H
#define TERRAIN_H

#include <Ogre.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreHlmsPbs.h>
#include <OgreMeshManager.h>

#include <OgreMeshManager2.h>
#include <OgreMesh2.h>
#include <OgreItem.h>
#include "OgreSubMesh2.h"

#include <iostream>

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

class Terrain{
public:
    Terrain(Ogre::SceneManager *sceneManager, int width, int height);

    Ogre::IndexBufferPacked* createIndexBuffer();
    Ogre::MeshPtr createStaticMesh();

    int width, height;
    Ogre::SceneNode *node;
    Ogre::SceneManager *sceneManager;
};

#endif

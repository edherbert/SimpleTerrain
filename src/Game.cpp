#include "Game.h"

#include <iostream>

Game::Game(){
	root = new Ogre::Root("../setup/Plugins.cfg");
	root->showConfigDialog();
	//root->restoreConfig();

	//root->addResourceLocation("/home/edward/Documents/ogre/Samples/Media/models", "FileSystem");
	root->addResourceLocation("../media", "FileSystem");
	//root->addResourceLocation("/home/edward/Documents/ogre/Samples/Media/materials/scripts", "FileSystem");
    root->getRenderSystem()->setConfigOption( "sRGB Gamma Conversion", "Yes" );
	root->initialise(false);

	renderWindow = root->createRenderWindow("A window", 900, 800, false, 0);

    //All classes need a workspace, whatever that is

    registerHlms();

    //Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(false);

    createSceneManager();
    createCamera();
    workspace = setupCompositor();

    /*Ogre::SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::SCENE_STATIC);
    //Ogre::Item *item = sceneManager->createItem("Barrel2.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_STATIC);
    Ogre::Item *item = sceneManager->createItem("ogrehead2.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_STATIC);
    node->attachObject((Ogre::MovableObject*)item);
    node->setScale(0.1, 0.1, 0.1);*/


    //This creates a pointer to a mesh
    Ogre::MeshPtr staticMesh = createStaticMesh();

    //Then just attach it to an item as normal.
    Ogre::SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::SCENE_STATIC);
    Ogre::Item *item = sceneManager->createItem(staticMesh, Ogre::SCENE_STATIC);
    node->attachObject((Ogre::MovableObject*)item);

    sceneManager->setForward3D( true, 4,4,5,96,3,200 );

    //sceneManager->setAmbientLight( Ogre::ColourValue( 0.33f, 0.61f, 0.98f ) * 0.01f, Ogre::ColourValue( 0.02f, 0.53f, 0.96f ) * 0.01f, Ogre::Vector3::UNIT_Y );
    sceneManager->setAmbientLight( Ogre::ColourValue( 0.33f, 0.61f, 0.98f ) * 0.1f, Ogre::ColourValue( 0.02f, 0.53f, 0.96f ) * 0.1f, Ogre::Vector3::UNIT_Y );

    Ogre::Light *directionLight = sceneManager->createLight();
    Ogre::SceneNode *lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject( directionLight );
    directionLight->setPowerScale( Ogre::Math::PI );
    directionLight->setType( Ogre::Light::LT_DIRECTIONAL );
    directionLight->setDiffuseColour(1, 1, 1);
    directionLight->setDirection( Ogre::Vector3( -1, -1, -1 ).normalisedCopy() );

    Ogre::Light* light2 = sceneManager->createLight();
    Ogre::SceneNode* light2Node = sceneManager->getRootSceneNode( )->createChildSceneNode();
    light2Node->attachObject( light2 );
    light2->setDiffuseColour( 0.8f, 0.4f, 0.2f );
    light2->setSpecularColour( 0.8f, 0.4f, 0.2f );
    light2->setCastShadows( false );
    light2->setType( Ogre::Light::LT_POINT );
    light2->setAttenuationBasedOnRadius( 100, 0.00192f );
    light2Node->setPosition(0,3,8);

    //Put the ogre head in as well
    float countt = 0.0f;
    int dist = 10;
	while(!renderWindow->isClosed()){
        countt += 0.005f;
        camera->setPosition(dist * cos(countt), dist / 8, dist * sin(countt));
        light2Node->setPosition(dist * cos(countt), 5, dist * sin(countt));
        camera->lookAt(0, 0, 0);
		Ogre::WindowEventUtilities::messagePump();
		root->renderOneFrame();
	}
}

Game::~Game(){

}

void Game::registerHlms(){
    Ogre::String rootHlmsFolder = "../setup/Hlms";

    Ogre::Archive *archiveLibrary = Ogre::ArchiveManager::getSingletonPtr()->load("/home/edward/Documents/ogre/Samples/Media/Hlms/Common/GLSL", "FileSystem", true );

    Ogre::ArchiveVec library;
    library.push_back( archiveLibrary );

    Ogre::Archive *archivePbs = Ogre::ArchiveManager::getSingletonPtr()->load("/home/edward/Documents/ogre/Samples/Media/Hlms/Pbs/GLSL", "FileSystem", true );
    Ogre::HlmsPbs *hlmsPbs = OGRE_NEW Ogre::HlmsPbs( archivePbs, &library );

    root->getHlmsManager()->registerHlms(hlmsPbs);
}

void Game::createSceneManager(){
    sceneManager = root->createSceneManager(Ogre::ST_GENERIC, 2, Ogre::INSTANCING_CULLING_SINGLETHREAD, "Scene Manager");
}

void Game::createCamera(){
    camera = sceneManager->createCamera( "Main Camera" );

    // Position it at 500 in Z direction
    camera->setPosition( Ogre::Vector3( 0, 5, 15 ) );
    // Look back along -Z
    camera->lookAt( Ogre::Vector3( 0, 0, 0 ) );
    camera->setNearClipDistance( 0.2f );
    camera->setFarClipDistance( 1000.0f );
    camera->setAutoAspectRatio( true );
}

Ogre::CompositorWorkspace* Game::setupCompositor(){
    Ogre::CompositorManager2 *compositorManager = root->getCompositorManager2();

    const Ogre::String workspaceName("test Workspace");
    if(!compositorManager->hasWorkspaceDefinition(workspaceName)){
        compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue(0, 0, 0, 1), Ogre::IdString());
    }

    return compositorManager->addWorkspace(sceneManager, renderWindow, camera, workspaceName, true);
}

//Create the index buffer and return a pointer to it.
Ogre::IndexBufferPacked* Game::createIndexBuffer(void){
    //So create a nice pointer
    Ogre::IndexBufferPacked *indexBuffer = 0;

    //A simple array containing index data.
    //There are three points in a triangle, and two triangles on each face of a cube. There are six faces on a cube.
    const Ogre::uint16 c_indexData[3 * 2 * 6] =
    {
        0, 1, 2, 2, 3, 0, //Front face
        6, 5, 4, 4, 7, 6, //Back face

        3, 2, 6, 6, 7, 3, //Top face
        5, 1, 0, 0, 4, 5, //Bottom face

        4, 0, 3, 3, 7, 4, //Left face
        6, 2, 1, 1, 5, 6, //Right face
    };

    Ogre::uint16 *cubeIndices = reinterpret_cast<Ogre::uint16*>( OGRE_MALLOC_SIMD(
                                                                     sizeof(Ogre::uint16) * 3 * 2 * 6,
                                                                     Ogre::MEMCATEGORY_GEOMETRY ) );
    //Copy the data into the vector.
    memcpy( cubeIndices, c_indexData, sizeof( c_indexData ) );

    //Get the vao manager.
    Ogre::RenderSystem *renderSystem = root->getRenderSystem();
    Ogre::VaoManager *vaoManager = renderSystem->getVaoManager();

    try
    {
	//Actually create an index buffer and assign it to the pointer created earlier.
	//Also populate the index buffer with these values.
	//This goes, type, number of indices, Buffer type, the actual data, keep as shadow
        indexBuffer = vaoManager->createIndexBuffer( Ogre::IndexBufferPacked::IT_16BIT,
                                                     3 * 2 * 6,
                                                     Ogre::BT_IMMUTABLE,
                                                     cubeIndices, true );
    }
    catch( Ogre::Exception &e )
    {
        // When keepAsShadow = true, the memory will be freed when the index buffer is destroyed.
        // However if for some weird reason there is an exception raised, the memory will
        // not be freed, so it is up to us to do so.
        // The reasons for exceptions are very rare. But we're doing this for correctness.
        OGRE_FREE_SIMD( indexBuffer, Ogre::MEMCATEGORY_GEOMETRY );
        indexBuffer = 0;
        throw e;
    }

    return indexBuffer;
}

Ogre::MeshPtr Game::createStaticMesh(){
    Ogre::RenderSystem *renderSystem = root->getRenderSystem();
    Ogre::VaoManager *vaoManager = renderSystem->getVaoManager();
	
    //Create a mesh that will contain the vao things.
    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual("A mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    //Create a sub mesh
    Ogre::SubMesh *subMesh = mesh->createSubMesh();
    
    //I think this describes how the elements will be spaced out.
    Ogre::VertexElement2Vec vertexElements;
    vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT3, Ogre::VES_POSITION));
    vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT3, Ogre::VES_NORMAL));

    //Cube Verticies is a struct, and we have eight of them for the geometry.
    CubeVertices *cubeVertices = reinterpret_cast<CubeVertices*>( OGRE_MALLOC_SIMD(sizeof(CubeVertices) * 8, Ogre::MEMCATEGORY_GEOMETRY ) );

    //Copy the verticies
    memcpy(cubeVertices, c_originalVertices, sizeof(CubeVertices) * 8);

    //Create a pointer to the buffer.
    Ogre::VertexBufferPacked *vertexBuffer = 0;
    try{
	//So pass in the element details, the number of verticies, the buffer type, the actual vertex data and the shadow boolean
        vertexBuffer = vaoManager->createVertexBuffer(vertexElements, 8, Ogre::BT_DEFAULT, cubeVertices, true);
    }catch(Ogre::Exception &e){
        vertexBuffer = 0;
        throw e;
    }

    Ogre::VertexBufferPackedVec vertexBuffers;
    vertexBuffers.push_back(vertexBuffer);
    Ogre::IndexBufferPacked *indexBuffer = createIndexBuffer();
    Ogre::VertexArrayObject *vao = vaoManager->createVertexArrayObject(vertexBuffers, indexBuffer, Ogre::OT_TRIANGLE_LIST);

    subMesh->mVao[Ogre::VpNormal].push_back(vao);
    subMesh->mVao[Ogre::VpShadow].push_back(vao);

    mesh->_setBounds( Ogre::Aabb( Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_SCALE ), false );
    mesh->_setBoundingSphereRadius( 1.732f );

    return mesh;
}

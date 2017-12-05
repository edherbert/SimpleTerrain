#include "Game.h"

#include "Terrain.h"
#include "Window.h"

#include <iostream>

Game::Game(){
	root = new Ogre::Root("../setup/Plugins.cfg");
	//root->showConfigDialog();
    root->restoreConfig();

	//root->addResourceLocation("/home/edward/Documents/ogre/Samples/Media/models", "FileSystem");
	root->addResourceLocation("../media", "FileSystem");
	//root->addResourceLocation("/home/edward/Documents/ogre/Samples/Media/materials/scripts", "FileSystem");
    root->getRenderSystem()->setConfigOption( "sRGB Gamma Conversion", "Yes" );
	root->initialise(false);


    window = new Window();

    registerHlms();

    //Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(false);

    createSceneManager();
    createCamera();
    workspace = setupCompositor();

    Terrain *terr = new Terrain(sceneManager, 20, 20);


    Ogre::SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::SCENE_STATIC);
    //Ogre::Item *item = sceneManager->createItem("Barrel2.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_STATIC);
    Ogre::Item *item = sceneManager->createItem("ogrehead2.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_STATIC);
    //node->attachObject((Ogre::MovableObject*)item);
    node->setScale(0.1, 0.1, 0.1);


    sceneManager->setForward3D( true, 4,4,5,96,3,200 );

    //sceneManager->setAmbientLight( Ogre::ColourValue( 0.33f, 0.61f, 0.98f ) * 1.0f, Ogre::ColourValue( 0.02f, 0.53f, 0.96f ) * 0.1f, Ogre::Vector3::UNIT_Y );
    sceneManager->setAmbientLight( Ogre::ColourValue( 1.0f, 1.0f, 1.0f ), Ogre::ColourValue( 0.02f, 0.53f, 0.96f ) * 0.1f, Ogre::Vector3::UNIT_Y );

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

    bool running = true;

	while(running){
	    window->update();

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
    //camera->setPosition( Ogre::Vector3( 0, 0, 50 ) );
    camera->setPosition( Ogre::Vector3( 0, 50, 50 ) );
    // Look back along -Z
    camera->lookAt( Ogre::Vector3( 0, 0, 0 ) );
    camera->setNearClipDistance( 0.2f );
    camera->setFarClipDistance( 1000.0f );
    camera->setAutoAspectRatio( true );
}

void Game::pointCamera(int xOffset, int yOffset){
    float sense = 0.05;
    float xCamera = xOffset;
    float yCamera = yOffset;
    //xOffset *= sense;
    //yOffset *= sense;
    xCamera *= sense;
    yCamera *= sense;

    yaw += xCamera;
    pitch += yCamera;
    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    Ogre::Vector3 front;
    front.x = cos(radians(yaw)) * cos(radians(pitch));
    front.y = sin(radians(pitch));
    front.z = sin(radians(yaw)) * cos(radians(pitch));

    //Normalise the camera front
    float length = sqrt((front.x * front.x) + (front.y * front.y) + (front.z * front.z));
    Ogre::Vector3 cameraFront = Ogre::Vector3(front.x / length, front.y / length, front.z / length);

    //std::cout << xCamera << std::endl;

    camera->setDirection(cameraFront);

    std::cout << camera->getDirection() << std::endl;
}

float Game::radians(float value){
    return value * (M_PI / 180);
}

Ogre::CompositorWorkspace* Game::setupCompositor(){
    Ogre::CompositorManager2 *compositorManager = root->getCompositorManager2();

    const Ogre::String workspaceName("test Workspace");
    if(!compositorManager->hasWorkspaceDefinition(workspaceName)){
        compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue(0, 0, 0, 1), Ogre::IdString());
    }

    return compositorManager->addWorkspace(sceneManager, window->getRenderWindow(), camera, workspaceName, true);
}

void Game::captureMouse(){
    std::cout << "Capturing Mouse" << std::endl;
    mouseCaptured = !mouseCaptured;

    //SDL_SetRelativeMouseMode(SDL_TRUE);
}

#include "Game.h"

#include <iostream>

Game::Game(){
	root = new Ogre::Root("../setup/Plugins.cfg");
	root->showConfigDialog();
	//root->restoreConfig();

	//root->addResourceLocation("/home/edward/Documents/ogre/Samples/Media/models", "FileSystem");
	root->addResourceLocation("../media", "FileSystem");
	//root->addResourceLocation("/home/edward/Documents/ogre/Samples/Media/materials/scripts", "FileSystem");

//	std::cout << "hello" << std::endl;
    root->getRenderSystem()->setConfigOption( "sRGB Gamma Conversion", "Yes" );
	root->initialise(false);

	renderWindow = root->createRenderWindow("A window", 900, 800, false, 0);

    root->getRenderSystem()->setConfigOption( "sRGB Gamma Conversion", "Yes" );

    //All classes need a workspace, whatever that is

    registerHlms();

    //Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(false);

    createSceneManager();
    createCamera();
    workspace = setupCompositor();

    Ogre::SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::SCENE_STATIC);
    //Ogre::Item *item = sceneManager->createItem("Barrel2.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_STATIC);
    Ogre::Item *item = sceneManager->createItem("ogrehead2.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_STATIC);
    node->attachObject((Ogre::MovableObject*)item);
    node->setScale(0.1, 0.1, 0.1);

    sceneManager->setForward3D( true, 4,4,5,96,3,200 );

    //maybe try adding a direction light.

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

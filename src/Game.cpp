#include "Game.h"

#include "Terrain.h"

#include <SDL.h>

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

    //Do all the SDL things here.
    SDL_Surface *surface = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    //Create the OGRE window reference here, but don't actually create the window
    Ogre::NameValuePairList params;
    params["currentGLContext"] = "true";
    renderWindow = root->createRenderWindow("Window", 100, 100, false, &params);


	//renderWindow = root->createRenderWindow("A window", 900, 800, false, 0);

    registerHlms();

    //Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(false);

    createSceneManager();
    createCamera();
    workspace = setupCompositor();

    Terrain *terr = new Terrain(sceneManager, 20, 20);

    sceneManager->setForward3D( true, 4,4,5,96,3,200 );

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

    SDL_Event event;

    bool running = true;
    //Put the ogre head in as well
	while(running){
		Ogre::WindowEventUtilities::messagePump();

        SDL_UpdateWindowSurface(window);
        SDL_GL_SwapWindow(window);
        SDL_WaitEvent(&event);

        switch(event.type){
            case SDL_QUIT:
                closeWindow();                
                running = false;
                break;
        }

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
    camera->setPosition( Ogre::Vector3( 0, 0, 50 ) );
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

void Game::closeWindow(){
    std::cout << "Closing window" << std::endl;

    renderWindow->destroy();
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#include "Terrain.h"

Terrain::Terrain(Ogre::SceneManager *sceneManager, int width, int height) : 
    width(width),
    height(height),
    sceneManager(sceneManager){

    Ogre::MeshPtr staticMesh = createStaticMesh();
    node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::SCENE_STATIC);
    Ogre::Item *item = sceneManager->createItem(staticMesh, Ogre::SCENE_STATIC);
    //Ogre::Item *item = sceneManager->createItem("ogrehead2.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_STATIC);
    node->attachObject((Ogre::MovableObject*)item);
}

Ogre::IndexBufferPacked* Terrain::createIndexBuffer(void){
    Ogre::IndexBufferPacked *indexBuffer = 0;

    int arraySize = 6 * width * height;
/*    const Ogre::uint16 c_indexData[arraySize] =
    {
        0, 1, 2, 2, 3, 0, //Single face 
        
    };*/

    Ogre::uint16 c_indexData[arraySize];


    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            //Each cube will add 6 values to the array.
            int start = (x * 6) + (y * (width - 1) * 6);

            for(int i = 0; i < 6; i++){
                c_indexData[start + i] = 0;
            }

            c_indexData[start] = x + y * (width + 1);
            c_indexData[start + 1] = 1 + x + y * (width + 1);
            c_indexData[start + 2] = 1 + x + (y + 1) * (width + 1);
            c_indexData[start + 3] = 1 + x + (y + 1) * (width + 1);
            c_indexData[start + 4] = x + (y + 1) * (width + 1);
            c_indexData[start + 5] = x + (y + 0) * (width + 1);

            for(int i = 0; i < 6; i++){
                std::cout << c_indexData[start + i] << std::endl;
            }
        }
    }

    /*c_indexData[0] = 0;
    c_indexData[1] = 1;
    c_indexData[2] = 3;
    c_indexData[3] = 3;
    c_indexData[4] = 2;
    c_indexData[5] = 0;*/

    

    Ogre::uint16 *cubeIndices = reinterpret_cast<Ogre::uint16*>( OGRE_MALLOC_SIMD(
                                                                     sizeof(Ogre::uint16) * arraySize, 
                                                                     Ogre::MEMCATEGORY_GEOMETRY ) );
    memcpy( cubeIndices, c_indexData, sizeof( c_indexData ) );

    Ogre::RenderSystem *renderSystem = Ogre::Root::getSingletonPtr()->getRenderSystem();
    Ogre::VaoManager *vaoManager = renderSystem->getVaoManager();

    try
    {
	//Actually create an index buffer and assign it to the pointer created earlier.
	//Also populate the index buffer with these values.
	//This goes, type, number of indices, Buffer type, the actual data, keep as shadow
        indexBuffer = vaoManager->createIndexBuffer( Ogre::IndexBufferPacked::IT_16BIT,
                                                     arraySize,
                                                     Ogre::BT_IMMUTABLE,
                                                     cubeIndices, true );
    }
    catch( Ogre::Exception &e )
    {
        OGRE_FREE_SIMD( indexBuffer, Ogre::MEMCATEGORY_GEOMETRY );
        indexBuffer = 0;
        throw e;
    }

    return indexBuffer;
}

Ogre::MeshPtr Terrain::createStaticMesh(){
    Ogre::RenderSystem *renderSystem = Ogre::Root::getSingletonPtr()->getRenderSystem();
    Ogre::VaoManager *vaoManager = renderSystem->getVaoManager();
	
    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual("Random Mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::SubMesh *subMesh = mesh->createSubMesh();
    
    Ogre::VertexElement2Vec vertexElements;
    vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT3, Ogre::VES_POSITION));
    vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT3, Ogre::VES_NORMAL));

    CubeVertices *cubeVertices = reinterpret_cast<CubeVertices*>( OGRE_MALLOC_SIMD(sizeof(CubeVertices) * 4, Ogre::MEMCATEGORY_GEOMETRY ) );

    //Create the verticies array but don't put anything in it.
    int count = (width + 1) * (height + 1);
    CubeVertices c_originalVertices[count];

    std::cout << count << std::endl;

    //Fill the array with verticies. 
    /*for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            std::cout << "x: " << x << " y: " << y << std::endl;
            //c_originalVertices[x + y * width] = CubeVertices(x, y, 1, 0.5, 0.5, 0.5);
        }
    }*/

    int arrayCount = 0;
    for(int y = 0; y < height + 1; y++){
        for(int x = 0; x < width + 1; x++){
            std::cout << "x: " << x << " y: " << y << std::endl;
            c_originalVertices[arrayCount] = CubeVertices(x, y, 0, 0.5, 0.5, 0.5);
            arrayCount++;
        }
    } 

//    memcpy(cubeVertices, c_originalVertices, sizeof(CubeVertices) * 4);
    memcpy(cubeVertices, c_originalVertices, sizeof(CubeVertices) * count);

    Ogre::VertexBufferPacked *vertexBuffer = 0;
    try{
        //Try checking the parameters to see if the correct stride and things is being fed in here.
        vertexBuffer = vaoManager->createVertexBuffer(vertexElements, count, Ogre::BT_DEFAULT, cubeVertices, true);
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

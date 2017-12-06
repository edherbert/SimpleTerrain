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

    item->setDatablock("terrain");
}

Ogre::IndexBufferPacked* Terrain::createIndexBuffer(void){
    Ogre::IndexBufferPacked *indexBuffer = 0;

    int arraySize = 6 * width * height;

    Ogre::uint16 c_indexData[arraySize];

    for(int i = 0; i < arraySize; i++){
        c_indexData[i] = 0;
    }

    int currentStart = 0;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            c_indexData[currentStart + 0] = x + y * (width + 1);
            c_indexData[currentStart + 1] = x + (y + 1) * (width + 1);
            c_indexData[currentStart + 2] = 1 + x + (y + 1) * (width + 1);
            c_indexData[currentStart + 3] = 1 + x + (y + 1) * (width + 1);
            c_indexData[currentStart + 4] = 1 + x + y * (width + 1);
            c_indexData[currentStart + 5] = x + y * (width + 1);

            currentStart += 6;
        }
    }

    Ogre::uint16 *cubeIndices = reinterpret_cast<Ogre::uint16*>( OGRE_MALLOC_SIMD(sizeof(Ogre::uint16) * arraySize, Ogre::MEMCATEGORY_GEOMETRY));
    memcpy( cubeIndices, c_indexData, sizeof( c_indexData ) );

    Ogre::RenderSystem *renderSystem = Ogre::Root::getSingletonPtr()->getRenderSystem();
    Ogre::VaoManager *vaoManager = renderSystem->getVaoManager();

    try
    {
	//Actually create an index buffer and assign it to the pointer created earlier.
	//Also populate the index buffer with these values.
	//This goes, type, number of indices, Buffer type, the actual data, keep as shadow
        indexBuffer = vaoManager->createIndexBuffer(Ogre::IndexBufferPacked::IT_16BIT, arraySize, Ogre::BT_IMMUTABLE, cubeIndices, true);
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
    vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES));

    //Create the verticies array but don't put anything in it.
    int count = (width + 1) * (height + 1);
    CubeVertices c_originalVertices[count];

    CubeVertices *cubeVertices = reinterpret_cast<CubeVertices*>( OGRE_MALLOC_SIMD(sizeof(CubeVertices) * count, Ogre::MEMCATEGORY_GEOMETRY ) );

    //Put the vertices in. Just walk through the grid adding them.
    int arrayCount = 0;
    for(int y = 0; y < height + 1; y++){
        for(int x = 0; x < width + 1; x++){
            float value = (rand() % 100) * 0.01;

            //c_originalVertices[arrayCount] = CubeVertices(x, value, y, 0.5, 0.5, 0.5, x * width, y * height);
            c_originalVertices[arrayCount] = CubeVertices(x, value, y, 1, 1, 1, x * width, y * height);
            arrayCount++;
        }
    }

    memcpy(cubeVertices, c_originalVertices, sizeof(CubeVertices) * count);

    Ogre::VertexBufferPacked *vertexBuffer = 0;
    try{
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

#include "testingPage.hpp"

void TestingPage::onDraw()
{
    ImGui::ShowMetricsWindow();
    imguiShowSceneHierarchy();

    m_renderBatch->batchBegin();

    m_renderBatch->push(e);

    m_renderBatch->batchEnd();
    
    renderer::Renderer::instance()->push(m_renderBatch);

    // test error checking
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        CORE_ERROR("OPENGL ERROR: {}", err);
    }
}

void TestingPage::onUpdate()
{
    if (core::InputManager::instance()->keyPressed(SDL_SCANCODE_X))
    {
        core::Graphics::instance()->wireframe(!core::Graphics::instance()->wireframe());
    }
    
    bool inputDisabled = core::InputManager::instance()->disableInput();
    core::InputManager::instance()->disableInput(false);
    if (core::InputManager::instance()->keyPressed(SDL_SCANCODE_ESCAPE))
    {
        core::Graphics::instance()->relativeMouse(!core::Graphics::instance()->relativeMouse());
        core::InputManager::instance()->disableInput(!inputDisabled);
    } else {
        core::InputManager::instance()->disableInput(inputDisabled);
    }

}

void TestingPage::onCreate()
{
    m_rand = random::prng("seed");
    
    m_perspective = new renderer::PerspectiveProjection;
    m_cam3D = new renderer::Camera3D();
    m_cam3D->entityId("Camera3D");
    m_cam3D->getTransform()->translate({0,0.5f,-5.0f});
    
    renderer::Renderer::instance()->setProjection(m_perspective);
    renderer::Renderer::instance()->setCamera(m_cam3D);
    
    m_renderBatch = new renderer::RenderBatch;
    m_physicsWorld = new physics::PhysicsWorld();
    m_physicsWorld->entityId("PhysicsWorld");

    // m_renderBatch->enableFaceCulling();
    // m_renderBatch->faceCull(GL_BACK);

    e = new entity::Entity();
    e->entityId("textureTest");

    e->addComponent(new component::Transform(e, "trans"));
    e->addComponent(new component::Texture(e, "tex", "res/stone.png", "u1"));
    e->addComponent(new component::Texture(e, "tex2", "res/oak_plank.png", "u2"));

    vertex::VertexLayout *vlayout = new vertex::VertexLayout();
    vlayout->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(testVertex, m_pos)));
    vlayout->push(new vertex::VertexComponent("texCoords", vertex::texture_coords, 2, GL_FLOAT, GL_FALSE, offsetof(testVertex, m_texCoords)));
    vlayout->push(new vertex::VertexComponent("textureId", vertex::texture_id, 1, GL_FLOAT, GL_FALSE, offsetof(testVertex, m_texId)));

    texChoice = 2.0f;

    std::vector<testVertex> vertices = {
        { {-1.0f/2.0f,-1.0f/2.0f, 0.0f}, {0,1}, texChoice },
        { { 1.0f/2.0f,-1.0f/2.0f, 0.0f}, {1,1}, texChoice },
        { { 1.0f/2.0f, 1.0f/2.0f, 0.0f}, {1,0}, texChoice },
        { {-1.0f/2.0f, 1.0f/2.0f, 0.0f}, {0,0}, texChoice },
    };

    std::vector<unsigned int> indices = {
        0,1,2,
        0,2,3
    };

    e->addComponent(new component::Mesh(e, "mesh", vlayout, GL_TRIANGLES));
    e->getComponent<component::Mesh>("mesh")->setVertices((void*)&vertices[0], sizeof(testVertex) * vertices.size());
    e->getComponent<component::Mesh>("mesh")->setIndices((void*)&indices[0], sizeof(unsigned int) * indices.size());
    e->getComponent<component::Mesh>("mesh")->loadShader("main.vertex", "main.fragment");
    e->getComponent<component::Transform>("trans")->addAllMeshes();
    e->getComponent<component::Mesh>("mesh")->addAllTextures();

    pushEntity(m_cam3D);
    pushEntity(e);
    pushEntity(m_physicsWorld);
}
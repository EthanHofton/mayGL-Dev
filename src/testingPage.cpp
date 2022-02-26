#include "testingPage.hpp"

void TestingPage::onDraw()
{
    // ImGui::ShowMetricsWindow();
    imguiShowSceneHierarchy();

    component::RenderComponent::preDraw();
    
    e->getComponent<component::RenderComponent>("renderComponent")->draw();

    // test error checking
    // GLenum err;
    // while((err = glGetError()) != GL_NO_ERROR)
    // {
    //     CORE_ERROR("OPENGL ERROR: {}", err);
    // }
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
    
    component::RenderComponent::setProjection(m_perspective);
    component::RenderComponent::setCamera(m_cam3D);
    
    m_physicsWorld = new physics::PhysicsWorld();
    m_physicsWorld->entityId("PhysicsWorld");

    e = new entity::Entity();
    e->entityId("test");

    e->addComponent(new component::Transform(e, "trans"));
    e->addComponent(new component::Transform(e, "trans2"));
    e->addComponent(new component::Texture(e, "tex", glm::vec2(32, 32), "one"));
    e->addComponent(new component::Texture(e, "tex2", "res/stone.png", "two"));

    vertex::VertexLayout *vlayout = new vertex::VertexLayout();
    vlayout->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(testVertex, m_pos)));
    vlayout->push(new vertex::VertexComponent("texCoords", vertex::texture_coords, 2, GL_FLOAT, GL_FALSE, offsetof(testVertex, m_texCoords)));

    vertex::VertexLayout *vlayout2 = new vertex::VertexLayout();
    vlayout2->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(testVertex, m_pos)));
    vlayout2->push(new vertex::VertexComponent("texCoords", vertex::texture_coords, 2, GL_FLOAT, GL_FALSE, offsetof(testVertex, m_texCoords)));


    texChoice = 1.0f;

    std::vector<testVertex> vertices = {
        { {-1.0f/2.0f,-1.0f/2.0f, 0.0f}, {0,1} },
        { { 1.0f/2.0f,-1.0f/2.0f, 0.0f}, {1,1} },
        { { 1.0f/2.0f, 1.0f/2.0f, 0.0f}, {1,0} },
        { {-1.0f/2.0f, 1.0f/2.0f, 0.0f}, {0,0} },
    };

    std::vector<unsigned int> indices = {
        0,1,2,
        0,2,3
    };

    e->addComponent(new component::Mesh(e, "mesh", vlayout));
    e->getComponent<component::Mesh>("mesh")->setVertices((void*)&vertices[0], sizeof(testVertex) * vertices.size());
    e->getComponent<component::Mesh>("mesh")->setIndices((void*)&indices[0], sizeof(unsigned int) * indices.size());
    e->getComponent<component::Transform>("trans")->addMesh("mesh");

    e->addComponent(new component::Mesh(e, "mesh2", vlayout2));
    e->getComponent<component::Mesh>("mesh2")->setVertices((void*)&vertices[0], sizeof(testVertex) * vertices.size());
    e->getComponent<component::Mesh>("mesh2")->setIndices((void*)&indices[0], sizeof(unsigned int) * indices.size());
    e->getComponent<component::Transform>("trans2")->addMesh("mesh2");

    e->addComponent(new component::RenderComponent(e, "renderComponent", vlayout));
    e->getComponent<component::RenderComponent>("renderComponent")->loadShaders("main.vertex", "main.fragment");
    e->getComponent<component::RenderComponent>("renderComponent")->setPrimativeType(GL_TRIANGLES);
    e->getComponent<component::RenderComponent>("renderComponent")->addAllMeshes();
    e->getComponent<component::Mesh>("mesh")->addAllTextures();
    e->getComponent<component::Mesh>("mesh2")->addAllTextures();

    float data[32][32][4];
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            data[i][j][0] = i / 32.0f;
            data[i][j][1] = j / 32.0f;
            data[i][j][2] = 0;
            data[i][j][3] = 1;
        }
    }

    unsigned int texId = e->getComponent<component::Texture>("tex")->getTextureId();
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 32, 32, GL_RGBA, GL_FLOAT, (void*)&data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    pushEntity(m_cam3D);
    pushEntity(e);
    pushEntity(m_physicsWorld);
}
#include "testingPage.hpp"

void TestingPage::onDraw()
{
    // ImGui::ShowMetricsWindow();
    imguiShowSceneHierarchy();

    ImGui::Begin("madlebrot peramiter");

    ImGui::DragFloat("zoom factor##zoom", &zoomFactor, 0.01f);
    ImGui::DragInt("number of iterations##iter", &numIter, 10);
    ImGui::DragInt("cutoff##iter", &cutoff, 1);
    ImGui::DragFloat2("x, y##transform", &trans[0], 0.1f);
    ImGui::ColorEdit3("color", &color[0]);

    ImGui::End();

    component::RenderComponent::preDraw();
    
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->setUniform2f("zoom", zoom);
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->setUniform2f("trans", trans);
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->setUniform1i("numIter", numIter);
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->setUniform1i("cutoff", cutoff);
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->setUniform3f("color", color);
    e->getComponent<component::RenderComponent>("renderComponent")->draw();
}

void TestingPage::onUpdate()
{
    if (core::InputManager::instance()->keyPressed(SDL_SCANCODE_X))
    {
        core::Graphics::instance()->wireframe(!core::Graphics::instance()->wireframe());
    }

    bool inputDisabled = core::InputManager::instance()->disableInput();
    core::InputManager::instance()->disableInput(false);

    if (core::InputManager::instance()->mouseButtonPressed(core::InputManager::left)) 
    {
        glm::vec2 mousePos = core::InputManager::instance()->getMousePos();

        float width = config::Config::instance()->getWidth();
        float height = config::Config::instance()->getHeight();

        float x = math::map(mousePos.x, 0, width, zoom.x, zoom.y);
        float y = math::map(mousePos.y, 0, height, zoom.x, zoom.y);

        trans += glm::vec2(x, y);

        zoom *= zoomFactor;
    }
    
    
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
    zoom = glm::vec2(-2, 2);
    zoomFactor = 14.0 / 16.0f;
    trans = glm::vec2(0);
    m_rand = random::prng("seed");
    numIter = 100;
    cutoff = 16;
    color = glm::vec3(1,0,0);
    
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

    vertex::VertexLayout *vlayout = new vertex::VertexLayout();
    vlayout->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(testVertex, m_pos)));
    vlayout->push(new vertex::VertexComponent("texCoords", vertex::texture_coords, 2, GL_FLOAT, GL_FALSE, offsetof(testVertex, m_texCoords)));

    std::vector<testVertex> vertices = {
        { {-1.0f,-1.0f, 0.0f}, {0,1} },
        { { 1.0f,-1.0f, 0.0f}, {1,1} },
        { { 1.0f, 1.0f, 0.0f}, {1,0} },
        { {-1.0f, 1.0f, 0.0f}, {0,0} },
    };

    std::vector<unsigned int> indices = {
        0,1,2,
        0,2,3
    };

    e->addComponent(new component::Mesh(e, "mesh", vlayout));
    e->getComponent<component::Mesh>("mesh")->setVertices((void*)&vertices[0], sizeof(testVertex) * vertices.size());
    e->getComponent<component::Mesh>("mesh")->setIndices((void*)&indices[0], sizeof(unsigned int) * indices.size());
    e->getComponent<component::Transform>("trans")->addMesh("mesh");

    e->addComponent(new component::RenderComponent(e, "renderComponent", vlayout));
    e->getComponent<component::RenderComponent>("renderComponent")->loadShaders("main.vertex", "main.fragment");
    e->getComponent<component::RenderComponent>("renderComponent")->setPrimativeType(GL_TRIANGLES);
    e->getComponent<component::RenderComponent>("renderComponent")->addAllMeshes();
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->addUniform("zoom");
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->addUniform("trans");
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->addUniform("numIter");
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->addUniform("cutoff");
    e->getComponent<component::RenderComponent>("renderComponent")->getShader()->addUniform("color");

    pushEntity(m_cam3D);
    pushEntity(e);
    pushEntity(m_physicsWorld);
}
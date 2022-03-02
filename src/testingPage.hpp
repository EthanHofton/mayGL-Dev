#ifndef testingPage_hpp
#define testingPage_hpp

#include <mayGL/core/window.hpp>
#include <mayGL/core/page.hpp>
#include <mayGL/core/logger.hpp>
#include <mayGL/entity/entity.hpp>
#include <mayGL/camera/camera2D.hpp>
#include <mayGL/camera/camera3D.hpp>
#include <mayGL/projection/perspectiveProjection.hpp>
#include <mayGL/component/textureComponent.hpp>
#include <mayGL/component/textureAtlasComponent.hpp>
#include <mayGL/geometry/cube.hpp>
#include <mayGL/geometry/sphere.hpp>
#include <mayGL/geometry/rectangle.hpp>
#include <mayGL/physics/physicsWorld.hpp>
#include <mayGL/physics/physicsBody.hpp>
#include <mayGL/physics/sphereCollider.hpp>
#include <mayGL/physics/cubeCollider.hpp>
#include <mayGL/geometry/model.hpp>
#include <mayGL/component/materialComponent.hpp>
#include <mayGL/component/renderComponent.hpp>
#include <mayGL/core/config.hpp>

using namespace mayGL;

struct testVertex
{
    glm::vec3 m_pos;
    glm::vec2 m_texCoords;
};

class TestingPage : public core::Page
{
public:
    TestingPage(core::Window *t_parent, std::string t_title) : core::Page(t_parent, t_title) {}
    ~TestingPage() override {}
    
    void onDraw() override;
    void onUpdate() override;        
    void onCreate() override;
    
    inline void onDelete() override {}
    inline void onEvent(SDL_Event t_e) override {}
    
private:
    
    renderer::PerspectiveProjection *m_perspective;
    renderer::Camera3D *m_cam3D;
    physics::PhysicsWorld *m_physicsWorld;
    random::prng m_rand;

    entity::Entity *e;

    glm::vec2 zoom;
    float zoomFactor;
    glm::vec2 trans;
    int numIter;
    int cutoff;
    glm::vec3 color;
};

#endif
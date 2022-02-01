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

using namespace mayGL;

struct testVertex
{
    glm::vec3 m_pos;
    glm::vec2 m_texCoords;
    float m_texId;
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
    renderer::RenderBatch *m_renderBatch;
    physics::PhysicsWorld *m_physicsWorld;
    random::prng m_rand;

    entity::Entity *e;
    float texChoice;
    // entity::Entity *r;
    // entity::Entity *dragon;
    // Ball *m_ball;
    // Ball *m_ball2;

    // Ball *m_ball2;
    // Floor *m_floor;

    // TODO: bind material to mesh (instead of mesh to material)
    // TODO: mesh load in bindeed materials as array (allows multiple materials)

    // TODO: add std::string m_uniformID to texture class.
    //       textures can be referenced by there uniform id in the shaders.
    //       add textureID filed to vertex attrib, when mesh gets pushed to
    //       renderer, convert the mesh uid to the texture unit.
};

#endif
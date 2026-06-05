/*
    Magnum + Bullet Domino Example (stable version)
*/

#include <vector>
#include <cmath>
#include <iostream>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>

#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/Math/Functions.h>

#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>

#include <Magnum/Trade/MeshData.h>

#include <btBulletDynamicsCommon.h>

namespace Magnum { namespace Examples {

using namespace Math::Literals;

class DominoExample: public Platform::Application {
public:
    explicit DominoExample(const Arguments& arguments);
    ~DominoExample();

private:
    void drawEvent() override;
    void pointerMoveEvent(PointerMoveEvent& event) override;

    struct Domino {
        btRigidBody* body;
    };

    GL::Mesh _mesh;
    Shaders::PhongGL _shader;

    Matrix4 _camera;
    Matrix4 _projection;

    std::vector<Domino> _dominoes;

    btDiscreteDynamicsWorld* _world{};

    btDefaultCollisionConfiguration* _config{};
    btCollisionDispatcher* _dispatcher{};
    btBroadphaseInterface* _broadphase{};
    btSequentialImpulseConstraintSolver* _solver{};

    std::vector<btCollisionShape*> _shapes;
};

DominoExample::DominoExample(const Arguments& arguments):
    Platform::Application{
        arguments,
        Configuration{}.setTitle("Domino Example")
    }
{
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    _mesh = MeshTools::compile(Primitives::cubeSolid());

    _camera = Matrix4::translation({0.0f, -5.0f, -12.0f}) *
              Matrix4::rotationX(20.0_degf) * Matrix4::rotationY(-25.0_degf);

    _projection =
        Matrix4::perspectiveProjection(
            35.0_degf,
            Vector2{windowSize()}.aspectRatio(),
            0.01f,
            100.0f
        );

    /* Bullet init */
    _config = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_config);
    _broadphase = new btDbvtBroadphase();
    _solver = new btSequentialImpulseConstraintSolver();

    _world = new btDiscreteDynamicsWorld(
        _dispatcher,
        _broadphase,
        _solver,
        _config
    );

    _world->setGravity(btVector3(0, -9.81f, 0));

    /* Ground */
    auto* groundShape = new btStaticPlaneShape(btVector3(0,1,0), 0);
    _shapes.push_back(groundShape);

    auto* groundMotion = new btDefaultMotionState(
        btTransform::getIdentity()
    );

    auto* groundBody = new btRigidBody(
        btRigidBody::btRigidBodyConstructionInfo(
            0.0f,
            groundMotion,
            groundShape
        )
    );

    _world->addRigidBody(groundBody);

    /* Domino shape */
    auto* boxShape = new btBoxShape(btVector3(0.1f, 0.5f, 0.25f));
    _shapes.push_back(boxShape);

    /* Create chain */
    for(Int i = 0; i != 25; ++i) {

        btTransform t;
        t.setIdentity();
        t.setOrigin(btVector3(-4.0f + i*0.35f, 0.5f, 0));

        btVector3 inertia(0,0,0);
        boxShape->calculateLocalInertia(1.0f, inertia);

        auto* motion = new btDefaultMotionState(t);

        auto* body = new btRigidBody(
            btRigidBody::btRigidBodyConstructionInfo(
                1.0f,
                motion,
                boxShape,
                inertia
            )
        );

        body->setFriction(0.8f);
        body->setRestitution(0.0f);

        _world->addRigidBody(body);
        _dominoes.push_back({body});
    }

    /* Start impulse */
    _dominoes.front().body->applyTorqueImpulse(btVector3(5.0f, 0.0f, 0.0f));
}

DominoExample::~DominoExample() {
    for(auto* shape: _shapes)
        delete shape;

    delete _world;
    delete _solver;
    delete _broadphase;
    delete _dispatcher;
    delete _config;
}

void DominoExample::drawEvent() {
    GL::defaultFramebuffer.clear(
        GL::FramebufferClear::Color |
        GL::FramebufferClear::Depth
    );

    Matrix4 projection =
        Matrix4::perspectiveProjection(
            35.0_degf, Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f) *
        Matrix4::translation({0.0f, -1.0f, -16.0f}) *
        Matrix4::rotationX(55.0_degf);

    _shader.setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
        .setDiffuseColor(0xff0000_rgbf)
        .setAmbientColor(Color3{0.3f});

    constexpr Int Count = 100;
    constexpr Float Radius = 3.0f;

    for(Int i = 0; i != Count; ++i) {

        Float t =
            Float(i)/(Count - 1) *
            2.0f *
            Math::Constants<float>::pi();

        /* liegende Acht */
        Float x = Radius*std::sin(t);
        Float z = Radius*std::sin(t)*std::cos(t);

        /* Tangente für Domino-Ausrichtung */
        Float dx = Radius*std::cos(t);
        Float dz = Radius*std::cos(2.0f*t);

        Float angle = std::atan2(dx, dz);

        Matrix4 transform =
            Matrix4::translation({x, 0.5f, z}) *
            Matrix4::rotationY(Rad{angle}) *
            Matrix4::scaling({0.15f, 0.8f, 0.4f});

        _shader
            .setTransformationMatrix(transform)
            .setNormalMatrix(transform.normalMatrix())
            .setProjectionMatrix(projection)
            .draw(_mesh);
    }

    swapBuffers();
}

/*void DominoExample::drawEvent() {
    GL::defaultFramebuffer.clear(
        GL::FramebufferClear::Color |
        GL::FramebufferClear::Depth
    );

    Matrix4 projection =
        Matrix4::perspectiveProjection(
            35.0_degf,
            Vector2{windowSize()}.aspectRatio(),
            0.01f,
            100.0f
        ) *
        Matrix4::translation(Vector3::zAxis(-10.0f));

    Matrix4 transform =
        Matrix4::translation({0.0f, 0.0f, 0.0f}) *
        Matrix4::rotationX(30.0_degf) *
        Matrix4::rotationY(40.0_degf);

    _shader
        .setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
        .setDiffuseColor(0xff0000_rgbf)
        .setAmbientColor(Color3{0.3f})
        .setTransformationMatrix(transform)
        .setNormalMatrix(transform.normalMatrix())
        .setProjectionMatrix(projection)
        .draw(_mesh);

    swapBuffers();
}*/

void DominoExample::pointerMoveEvent(PointerMoveEvent& event)
{
  if (!event.isPrimary() ||
      !(event.pointers() & (Pointer::MouseLeft | Pointer::Finger)))
    return;

  Vector2 delta =
      3.0f * Vector2{event.relativePosition()} / Vector2{windowSize()};

  _camera = Matrix4::rotationX(Rad{delta.y()}) * _camera *
            Matrix4::rotationY(Rad{delta.x()});

  event.setAccepted();
}
}} // namespace Magnum::Examples

MAGNUM_APPLICATION_MAIN(Magnum::Examples::DominoExample)

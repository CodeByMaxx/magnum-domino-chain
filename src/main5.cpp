#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>

#include <btBulletDynamicsCommon.h>

#include <vector>

namespace Magnum { namespace Examples {

using namespace Math::Literals;

class DominoExample : public Platform::Application {
public:
    explicit DominoExample(const Arguments& arguments);
    ~DominoExample();

private:
    void drawEvent() override;

    struct Domino { btRigidBody* body; };

    GL::Mesh _mesh;
    Shaders::PhongGL _shader;

    Matrix4 _projection;

    btDiscreteDynamicsWorld* _world{};
    btDefaultCollisionConfiguration* _config{};
    btCollisionDispatcher* _dispatcher{};
    btBroadphaseInterface* _broadphase{};
    btSequentialImpulseConstraintSolver* _solver{};

    std::vector<btCollisionShape*> _shapes;
    std::vector<Domino> _dominoes;
};

/* ---------------- INIT ---------------- */

DominoExample::DominoExample(const Arguments& arguments)
    : Platform::Application{arguments,
        Configuration{}.setTitle("∞ 8 + Spiral + Loop Domino")}
{
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);

    _mesh = MeshTools::compile(Primitives::cubeSolid());

    _projection =
        Matrix4::perspectiveProjection(
            35.0_degf,
            Vector2{windowSize()}.aspectRatio(),
            0.01f,
            120.0f
        ) *
        Matrix4::translation(Vector3::zAxis(-20.0f));

    /* ---------------- BULLET ---------------- */

    _config = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_config);
    _broadphase = new btDbvtBroadphase();
    _solver = new btSequentialImpulseConstraintSolver();

    _world = new btDiscreteDynamicsWorld(
        _dispatcher, _broadphase, _solver, _config
    );

    _world->setGravity(btVector3(0, -9.81f, 0));

    _world->getSolverInfo().m_numIterations = 25;

    /* ---------------- GROUND ---------------- */

    auto* groundShape = new btStaticPlaneShape(btVector3(0,1,0), 0);
    _shapes.push_back(groundShape);

    _world->addRigidBody(new btRigidBody(
        btRigidBody::btRigidBodyConstructionInfo(
            0.0f,
            new btDefaultMotionState(btTransform::getIdentity()),
            groundShape
        )
    ));

    /* ---------------- DOMINO ---------------- */

    auto* boxShape = new btBoxShape(btVector3(0.08f, 0.4f, 0.2f));
    _shapes.push_back(boxShape);

    constexpr int Count = 180;
    constexpr float R = 3.5f;

    for(int i = 0; i != Count; ++i) {

        float t = float(i) / float(Count - 1) * 2.0f * 3.1415926f;

        /* -------- PERFECT 8 -------- */
        float x = R * std::sin(t);
        float z = R * std::sin(t) * std::cos(t);

        /* -------- SPIRAL MOD -------- */
        float spiral = 1.0f + 0.15f * std::sin(6.0f * t);
        x *= spiral;
        z *= spiral;

        /* -------- LOOP HEIGHT -------- */
        float y = 0.4f + 0.15f * std::sin(2.0f * t);

        btTransform tr;
        tr.setIdentity();
        tr.setOrigin(btVector3(x, y, z));

        btVector3 inertia(0,0,0);
        boxShape->calculateLocalInertia(1.0f, inertia);

        auto* body = new btRigidBody(
            btRigidBody::btRigidBodyConstructionInfo(
                1.0f,
                new btDefaultMotionState(tr),
                boxShape,
                inertia
            )
        );

        body->setFriction(1.0f);
        body->setRestitution(0.0f);
        body->setDamping(0.05f, 0.8f);

        body->setCcdMotionThreshold(0.01f);
        body->setCcdSweptSphereRadius(0.04f);

        _world->addRigidBody(body);
        _dominoes.push_back({body});
    }

    /* ---------------- ONE-SHOT KICK ---------------- */

    btRigidBody* first = _dominoes.front().body;

    first->setLinearVelocity(btVector3(7.0f, 0.0f, 0.0f));
}

/* ---------------- CLEANUP ---------------- */

DominoExample::~DominoExample() {
    for(auto* s : _shapes) delete s;

    delete _world;
    delete _solver;
    delete _broadphase;
    delete _dispatcher;
    delete _config;
}

/* ---------------- DRAW ---------------- */

void DominoExample::drawEvent() {

    GL::defaultFramebuffer.clear(
        GL::FramebufferClear::Color |
        GL::FramebufferClear::Depth
    );

    _world->stepSimulation(1.0f/60.0f, 10);

    Matrix4 camera =
        Matrix4::rotationX(25.0_degf) *
        Matrix4::rotationY(35.0_degf);

    Matrix4 vp = _projection * camera;

    for(const Domino& d : _dominoes) {

        btTransform tr;
        d.body->getMotionState()->getWorldTransform(tr);

        btVector3 p = tr.getOrigin();

        Matrix4 model =
            Matrix4::translation({p.x(), p.y(), p.z()}) *
            Matrix4::scaling({0.15f, 0.8f, 0.4f});

        _shader
            .setLightPositions({{4.0f, 7.0f, 5.0f, 0.0f}})
            .setDiffuseColor(0xff5555_rgbf)
            .setAmbientColor(0x222222_rgbf)
            .setTransformationMatrix(model)
            .setNormalMatrix(model.normalMatrix())
            .setProjectionMatrix(vp)
            .draw(_mesh);
    }

    swapBuffers();
    redraw();
}

}} // namespace Magnum::Examples

MAGNUM_APPLICATION_MAIN(Magnum::Examples::DominoExample)

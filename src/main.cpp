#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Image.h>
#include <Magnum/ImageView.h>
#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Functions.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData.h>
#include <btBulletDynamicsCommon.h>

#include <cmath>
#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
namespace Magnum {
namespace Examples {

using namespace Math::Literals;

class DominoExample : public Platform::Application {
public:
    explicit DominoExample(const Arguments& arguments);
    ~DominoExample();

private:
    void drawEvent() override;

    struct Domino {
        btRigidBody* body;
    };

    GL::Mesh _mesh;
    Shaders::PhongGL _shader;
    Matrix4 _projection;

    std::vector<Domino> _dominoes;

    btDiscreteDynamicsWorld* _world{};
    btDefaultCollisionConfiguration* _config{};
    btCollisionDispatcher* _dispatcher{};
    btBroadphaseInterface* _broadphase{};
    btSequentialImpulseConstraintSolver* _solver{};

    std::vector<btCollisionShape*> _shapes;
};

/* ===================== INIT ===================== */

DominoExample::DominoExample(const Arguments& arguments)
    : Platform::Application{arguments,
        Configuration{}.setTitle("Real Domino Circle Physics")}
{
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);

    _mesh = MeshTools::compile(Primitives::cubeSolid());

    _projection =
        Matrix4::perspectiveProjection(
            35.0_degf,
            Vector2{windowSize()}.aspectRatio(),
            0.01f,
            200.0f
        );

    /* ===== BULLET WORLD ===== */
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

    /* stabiler Solver (wichtig!) */
    _world->getSolverInfo().m_numIterations = 60;
    _world->getSolverInfo().m_splitImpulse = true;
    _world->getSolverInfo().m_erp = 0.8f;

    /* ===== GROUND ===== */
    auto* groundShape = new btStaticPlaneShape(btVector3(0,1,0), 0);
    _shapes.push_back(groundShape);

    auto* groundMotion = new btDefaultMotionState(btTransform::getIdentity());

    auto* groundBody = new btRigidBody(
        btRigidBody::btRigidBodyConstructionInfo(
            0.0f,
            groundMotion,
            groundShape
        )
    );

    _world->addRigidBody(groundBody);

    /* ===== DOMINO ===== */
    auto* boxShape = new btBoxShape(btVector3(0.12f, 0.6f, 0.25f));
    _shapes.push_back(boxShape);

    /* Abstand zwischen Steinen (wichtig für echte Kettenreaktion) */
    const float spacing = 0.22f;

    constexpr int Count = 140;
    constexpr float Radius = 9.5f;

    for (int i = 0; i < Count; ++i) {
      float a = float(i) / float(Count) * 2.0f * 3.1415926f;

      /* ===== Kreisposition ===== */
      float x = Radius * std::cos(a);
      float z = Radius * std::sin(a);

      /* ===== Tangente (Richtung entlang Kreis) ===== */
      float tx = -std::sin(a);
      float tz = std::cos(a);

      /* normalisieren */
      float len = std::sqrt(tx * tx + tz * tz);
      tx /= len;
      tz /= len;

      /* ===== Bullet Transform ===== */
      btTransform t;
      t.setIdentity();

      /* leicht über Boden (halbe Höhe) */
      t.setOrigin(btVector3(x, 0.6f, z));

      /* ===== WICHTIG: korrekte Ausrichtung =====
         +π/2 Offset weil Cube in Magnum +X forward ist
      */
      float angle = std::atan2(tx, tz) + 1.57079632679f;

      btQuaternion rot;
      rot.setRotation(btVector3(0, 1, 0), angle);

      t.setRotation(rot);

      /* ===== Physik ===== */
      btVector3 inertia(0, 0, 0);
      boxShape->calculateLocalInertia(1.0f, inertia);

      auto* motion = new btDefaultMotionState(t);

      auto* body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(
          1.0f, motion, boxShape, inertia));

      body->setFriction(0.8f);
      body->setRestitution(0.0f);

      body->setDamping(0.02f, 0.2f);

      body->setActivationState(DISABLE_DEACTIVATION);

      body->setCcdMotionThreshold(0.001f);
      body->setCcdSweptSphereRadius(0.05f);

      _world->addRigidBody(body);
      _dominoes.push_back({body});
}

/*==== START IMPULSE ===== */
btRigidBody* first = _dominoes.front().body;
first->activate(true);

first->applyImpulse(btVector3(0.0f, 0.0f, 2.5f), btVector3(0.0f, 0.6f, 0.0f));
}

/* ===================== CLEANUP ===================== */

DominoExample::~DominoExample()
{
  for (auto* s : _shapes) delete s;

  delete _world;
  delete _solver;
  delete _broadphase;
  delete _dispatcher;
  delete _config;
}

/* ===================== RENDER ===================== */

void DominoExample::drawEvent()
{
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color |
                               GL::FramebufferClear::Depth);

  _world->stepSimulation(1.0f / 240.0f, 8);

  Matrix4 viewProj = _projection * Matrix4::translation({0.0f, 0.0f, -45.0f}) *
                     Matrix4::rotationX(60.0_degf) *
                     Matrix4::rotationY(25.0_degf);

  _shader.setLightPositions({{5.0f, 8.0f, 10.0f, 0.0f}})
      .setDiffuseColor(0xff4444_rgbf)
      .setAmbientColor(Color3{0.25f});

  for (const Domino& d : _dominoes) {
    btTransform tr;
    d.body->getMotionState()->getWorldTransform(tr);

    btVector3 p = tr.getOrigin();
    btQuaternion q = tr.getRotation();
    q.normalize();

    Quaternion rot{Vector3{q.x(), q.y(), q.z()}, q.w()};

    Matrix4 model = Matrix4::translation({p.x(), p.y(), p.z()}) *
                    Matrix4::from(rot.toMatrix(), {}) *
                    Matrix4::scaling({0.2f, 1.0f, 0.5f});

    _shader.setTransformationMatrix(model)
        .setNormalMatrix(model.normalMatrix())
        .setProjectionMatrix(viewProj)
        .draw(_mesh);
  }

  swapBuffers();
  redraw();
}

}  // namespace Examples
}  // namespace Magnum

MAGNUM_APPLICATION_MAIN(Magnum::Examples::DominoExample)

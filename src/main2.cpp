#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Trade/MeshData.h>
#include <btBulletDynamicsCommon.h>
#include <vector.h>

#include <cmath>

namespace Magnum {
namespace Examples {

using namespace Math::Literals;

class DominoExample : public Platform::Application {
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

DominoExample::DominoExample(const Arguments& arguments)
    : Platform::Application{arguments,
                            Configuration{}.setTitle("Magnum Bullet Domino")}
{
  GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

  _mesh = MeshTools::compile(Primitives::cubeSolid());

  _camera = Matrix4::lookAt(
      Vector3(6.0f, 8.0f, 6.0f),  // 👈 Position (oben + seitlich)
      Vector3(0.0f, 0.0f, 0.0f),  // 👈 Ziel (Mitte vom Achter)
      Vector3::yAxis()            // 👈 "oben"
  );

  _projection = Matrix4::perspectiveProjection(
      35.0_degf, Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f);

  /* Bullet setup */
  _config = new btDefaultCollisionConfiguration();
  _dispatcher = new btCollisionDispatcher(_config);
  _broadphase = new btDbvtBroadphase();
  _solver = new btSequentialImpulseConstraintSolver();

  _world =
      new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _config);

  _world->setGravity(btVector3(0, -9.81f, 0));

  /* Ground */
  auto* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
  _shapes.push_back(groundShape);

  auto* groundMotion = new btDefaultMotionState(btTransform::getIdentity());

  auto* groundBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(
      0.0f, groundMotion, groundShape));

  _world->addRigidBody(groundBody);

  /* Domino shape */
  auto* boxShape = new btBoxShape(btVector3(0.1f, 0.5f, 0.25f));
  _shapes.push_back(boxShape);

  /* Domino chain */
  for (Int i = 0; i != 100; ++i) {
    Float t = i / 50.0f * Math::Constants<float>::pi() * 2.0f;

    Float radius = 3.0f;

    /* Infinity shape */
    Float x = radius * std::sin(t);
    Float z = radius * std::sin(t) * std::cos(t);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(x, 0.5f, z));

    btVector3 inertia(0,0,0);
    boxShape->calculateLocalInertia(1.0f, inertia);

    auto* motion = new btDefaultMotionState(transform);

    auto* body = new btRigidBody(
        btRigidBody::btRigidBodyConstructionInfo(
            1.0f,
            motion,
            boxShape,
            inertia
        )
    );

    body->setFriction(1.0f);
    body->setRollingFriction(0.2f);
    body->setRestitution(0.0f);

    _world->addRigidBody(body);
    _dominoes.push_back({body});
  }
  /* Start impulse */
  _dominoes.front().body->applyTorqueImpulse(btVector3(0, 0, 3));
}

DominoExample::~DominoExample()
{
  for (auto* s : _shapes) delete s;

  delete _world;
  delete _solver;
  delete _broadphase;
  delete _dispatcher;
  delete _config;
}

void DominoExample::drawEvent()
{
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color |
                               GL::FramebufferClear::Depth);

  _world->stepSimulation(1.0f / 60.0f, 10);

  Matrix4 vp = _projection * _camera;

  for (const Domino& d : _dominoes) {
    btTransform t;
    d.body->getMotionState()->getWorldTransform(t);

    btVector3 p = t.getOrigin();
    btQuaternion q = t.getRotation();

    /* Safety */
    if (!std::isfinite(p.x()) || !std::isfinite(q.x()) || !std::isfinite(q.w()))
      continue;

    q.normalize();

    /* ✅ CORRECT QUATERNION CONVERSION */
    Quaternion rotation{Vector3(q.x(), q.y(), q.z()), q.w()};

    Matrix4 transform = Matrix4::translation({p.x(), p.y(), p.z()}) *
                        Matrix4(rotation.toMatrix()) *
                        Matrix4::scaling({0.2f, 1.0f, 0.5f});

    _shader.setLightPositions({{5.0f, 8.0f, 10.0f, 0.0f}})
        .setDiffuseColor(0x4ccccc_rgbf)
        .setTransformationMatrix(transform)
        .setNormalMatrix(transform.normalMatrix())
        .setProjectionMatrix(vp)
        .draw(_mesh);
  }

  swapBuffers();
  redraw();
}

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

}  // namespace Examples
}  // namespace Magnum

MAGNUM_APPLICATION_MAIN(Magnum::Examples::DominoExample)

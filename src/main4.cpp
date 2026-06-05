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

class DominoExample: public Platform::Application {
public:
    explicit DominoExample(const Arguments& arguments);
    ~DominoExample();

private:
    void drawEvent() override;
    void saveScreenshot(const std::string& filename, const Vector2i& size);

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
    int _frame = 0;
    int _captureEvery = 10; // alle 10 Frames
};

/* ---------------- CONSTRUCTOR ---------------- */

DominoExample::DominoExample(const Arguments& arguments):
    Platform::Application{arguments, Configuration{}.setTitle("Stable Domino Physics")}
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

    auto* groundMotion = new btDefaultMotionState(btTransform::getIdentity());

    auto* groundBody = new btRigidBody(
        btRigidBody::btRigidBodyConstructionInfo(
            0.0f,
            groundMotion,
            groundShape
        )
    );

    _world->addRigidBody(groundBody);

    auto* boxShape = new btBoxShape(btVector3(0.12f, 0.6f, 0.25f));
    _shapes.push_back(boxShape);

    const Int Count = 160;
    const Float Radius = 4.0f;

    for (Int i = 0; i != Count; ++i) {
      Float a = Float(i) / Float(Count) * 2.0f * Math::Constants<float>::pi();

      Float x = Radius * std::sin(a);
      Float z = Radius * std::sin(a) * std::cos(a);

      /* Tangente für saubere Ausrichtung */
      Float dx = Radius * std::cos(a);
      Float dz = Radius * std::cos(2.0f * a);
      Float angle = std::atan2(dx, dz);

      btTransform t;
      t.setIdentity();
      t.setOrigin(btVector3(x, 0.6f, z));
      t.setRotation(btQuaternion(0, angle, 0));

      btVector3 inertia(0, 0, 0);
      boxShape->calculateLocalInertia(1.0f, inertia);

      auto* motion = new btDefaultMotionState(t);

      auto* body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(
          1.0f, motion, boxShape, inertia));

      body->setFriction(0.6f);
      body->setRestitution(0.0f);
      body->setRollingFriction(0.1f);
      body->setSpinningFriction(0.1f);
      body->setDamping(0.02f, 0.2f);
      body->setActivationState(DISABLE_DEACTIVATION);

      body->setCcdMotionThreshold(0.001f);
      body->setCcdSweptSphereRadius(0.05f);

      _world->addRigidBody(body);
      _dominoes.push_back({body});
    }

    /* ---------------- START IMPULSE ---------------- */

    _dominoes[10].body->applyImpulse(btVector3(1.5f, 0.0f, 0.0f),
                                     btVector3(0.0f, 0.6f, 0.0f));
}

/* ---------------- DESTRUCTOR ---------------- */

DominoExample::~DominoExample() {
    for(auto* s: _shapes) delete s;

    delete _world;
    delete _solver;
    delete _broadphase;
    delete _dispatcher;
    delete _config;
}

/* ---------------- DRAW ---------------- */

void DominoExample::saveScreenshot(const std::string& filename,
                                   const Vector2i& size)
{
  Magnum::Image2D image = GL::defaultFramebuffer.read(
      Range2Di{{0, 0}, size}, Image2D{PixelFormat::RGBA8Unorm});

  stbi_write_png(filename.c_str(), size.x(), size.y(), 4, image.data(),
                 size.x() * 4);
}
void DominoExample::drawEvent()
{
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color |
                               GL::FramebufferClear::Depth);

  _world->stepSimulation(1.0f / 320.0f, 4);

  Matrix4 viewProj = _projection * Matrix4::translation({0.0f, 0.0f, -20.0f}) *
                     Matrix4::rotationX(60.0_degf) *
                     Matrix4::rotationY(25.0_degf);

  _shader.setLightPositions({{5.0f, 8.0f, 10.0f, 0.0f}})
      .setDiffuseColor(0xff0000_rgbf)
      .setAmbientColor(Color3{0.3f});

  for (const Domino& d : _dominoes) {
    btTransform tr;
    d.body->getMotionState()->getWorldTransform(tr);

    btVector3 p = tr.getOrigin();
    btQuaternion q = tr.getRotation();

    q.normalize();

    Quaternion rot{Vector3{q.x(), q.y(), q.z()}, q.w()};

    rot = rot.normalized();

    Matrix4 transform = Matrix4::translation({p.x(), p.y(), p.z()}) *
                        Matrix4::from(rot.toMatrix(), {}) *
                        Matrix4::scaling({0.2f, 1.0f, 0.5f});

    _shader.setTransformationMatrix(transform)
        .setNormalMatrix(transform.normalMatrix())
        .setProjectionMatrix(viewProj)
        .draw(_mesh);

  }

  _frame++;

  if (_frame % _captureEvery == 0) {
    char name[64];
    std::snprintf(name, 64, "frame_%05d.png", _frame);

    saveScreenshot(name, windowSize());
   }

   swapBuffers();
   redraw();
}
}  // namespace Examples
}  // namespace Magnum

MAGNUM_APPLICATION_MAIN(Magnum::Examples::DominoExample)

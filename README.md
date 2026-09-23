# Magnum Domino Chain

A real-time **domino chain simulation** built with **C++**, **Magnum**, and **Bullet Physics**.

The project demonstrates rigid-body physics, collision detection, and real-time 3D rendering by simulating a chain of falling dominoes.

---

## Features

* 🧱 Real-time domino physics simulation
* ⚙️ Bullet Physics rigid-body simulation
* 🎨 3D rendering with Magnum
* 💥 Collision detection between dominoes
* 🔄 Configurable domino chain
* 🖥️ Interactive real-time visualization

---

## Technologies

| Technology         | Purpose                                    |
| ------------------ | ------------------------------------------ |
| **C++**            | Application and simulation logic           |
| **Magnum**         | 3D rendering and application framework     |
| **Bullet Physics** | Rigid-body physics and collision detection |
| **OpenGL**         | Graphics rendering                         |
| **CMake**          | Build system                               |

---

## Architecture

The simulation consists of three main components:

```text
                  ┌──────────────────┐
                  │  Domino Setup    │
                  │  / Initialization│
                  └────────┬─────────┘
                           │
                           ▼
                  ┌──────────────────┐
                  │ Bullet Physics   │
                  │ Simulation       │
                  └────────┬─────────┘
                           │
                           ▼
                  ┌──────────────────┐
                  │ Magnum Rendering │
                  │ / OpenGL         │
                  └────────┬─────────┘
                           │
                           ▼
                    Real-time View
```

Bullet Physics is responsible for the physical simulation while Magnum handles rendering and application-level graphics.

---

## Physics Simulation

Each domino is represented as a rigid body.

The simulation calculates:

* Gravity
* Linear and angular movement
* Collision detection
* Collision response
* Friction between objects

When the first domino falls, its collision transfers momentum to the next domino and creates the chain reaction.

---

## Building

### Requirements

* C++ compiler with C++17 support
* CMake
* Magnum
* Bullet Physics
* OpenGL-compatible graphics environment

Clone the repository:

```bash id="a6qf8m"
git clone https://github.com/CodeByMaxx/magnum-domino-chain.git
cd magnum-domino-chain
```

Create a build directory:

```bash id="r5d3xk"
mkdir build
cd build
```

Configure the project:

```bash id="m2r8sj"
cmake ..
```

Build:

```bash id="q7x1nf"
cmake --build .
```

---

## Running

After building the project, start the generated executable from the build directory.

The application opens the real-time 3D simulation and visualizes the domino chain reaction.

---

## Project Goals

The project was created to explore the interaction between:

* Real-time rendering
* Rigid-body physics
* Collision detection
* C++ application development
* 3D graphics
* Simulation-driven visualization

It provides a compact example of combining a rendering framework with a physics engine in a real-time application.

---

## Future Improvements

Possible improvements include:

* [ ] Interactive domino placement
* [ ] Configurable domino spacing
* [ ] Different domino layouts
* [ ] Camera controls
* [ ] Additional physics parameters
* [ ] Performance improvements
* [ ] More complex chain configurations

---

## License

This project is licensed under the **MIT License**.


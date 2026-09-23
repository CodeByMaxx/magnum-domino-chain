# Magnum Domino Chain

A real-time **domino chain simulation** built with **C++**, **Magnum**, and **Bullet Physics**.

The project demonstrates rigid-body physics, collision detection, and real-time 3D rendering by simulating a chain of falling dominoes.

## Features

* 🧱 Real-time domino chain simulation
* ⚙️ Rigid-body physics with Bullet Physics
* 🎨 3D rendering with Magnum
* 💥 Collision detection and response
* 🖥️ Real-time visualization
* 🔄 Domino chain configuration

## Technologies

| Technology         | Purpose                                    |
| ------------------ | ------------------------------------------ |
| **C++**            | Application and simulation logic           |
| **Magnum**         | 3D rendering and application framework     |
| **Bullet Physics** | Rigid-body physics and collision detection |
| **OpenGL**         | Graphics rendering                         |
| **CMake**          | Build system                               |

## Architecture

The simulation combines three main components:

```text
Domino Setup
     │
     ▼
Bullet Physics
Simulation
     │
     ▼
Magnum / OpenGL
Rendering
     │
     ▼
Real-time 3D View
```

Bullet Physics handles the physical simulation, while Magnum is used for rendering and application-level graphics.

## Physics Simulation

Each domino is represented as a rigid body.

The simulation includes:

* Gravity
* Linear and angular movement
* Collision detection
* Collision response
* Friction between objects

When the first domino falls, its collision transfers momentum to the next domino, creating a chain reaction.

## Building

### Requirements

* C++ compiler with C++17 support
* CMake
* Magnum
* Corrade
* Bullet Physics
* OpenGL-compatible graphics environment

### 1. Clone the repository

```bash
git clone https://github.com/CodeByMaxx/magnum-domino-chain.git
cd magnum-domino-chain
```

### 2. Build Corrade

Corrade needs to be available before building Magnum.

```bash
cd corrade
mkdir build
cd build
cmake ..
cmake --build .
```

### 3. Build Magnum

```bash
cd ../../magnum
mkdir build
cd build
cmake .. -DMAGNUM_WITH_SDL2APPLICATION=ON
cmake --build .
cmake --install .
```

### 4. Build the project

Return to the project root and create a build directory:

```bash
cd ../..
mkdir build
cd build
cmake ..
cmake --build .
```

## Results

The repository contains several rendered simulation frames showing the domino chain at different points during the simulation.

### Simulation frames

![Domino simulation – frame 50](frame_00050.png)

![Domino simulation – frame 1000](frame_01000.png)

![Domino simulation – frame 4000](frame_04000.png)

![Domino simulation – frame 5000](frame_05000.png)

## Project Goals

The project was created to explore the interaction between:

* Real-time rendering
* Rigid-body physics
* Collision detection
* C++ application development
* 3D graphics
* Simulation-driven visualization

It provides a compact example of combining a rendering framework with a physics engine in a real-time application.

## Future Improvements

Possible improvements include:

* [ ] Interactive domino placement
* [ ] Configurable domino spacing
* [ ] Different domino layouts
* [ ] Camera controls
* [ ] Additional physics parameters
* [ ] Performance improvements
* [ ] More complex chain configurations


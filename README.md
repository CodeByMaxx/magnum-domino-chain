# Magnum Domino Chain

A C/C++ physics simulation demonstrating a chain of falling dominoes using **Bullet Physics** and the **Magnum** graphics engine.

The project combines rigid-body physics with real-time 3D visualization.

## Overview

The simulation creates a chain of dominoes and uses a physics engine to calculate their movement and collisions.

```text
Domino Chain
     |
     v
Bullet Physics
     |
     +------------------+
     |                  |
     v                  v
Rigid Body        Collision Detection
     |                  |
     +--------+---------+
              |
              v
        Physics Update
              |
              v
       Magnum Rendering
```

The project is mainly intended as an example of integrating a physics simulation with a C++ rendering framework.

---

## Simulation

The dominoes are simulated as rigid bodies.

When the first domino falls, its collision with the next domino transfers momentum through the chain:

```text
   |      |      |      |      |
   |      |      |      |      |
   |      |      |      |      |
   |      |      |      |      |
   +      +      +      +      +

      ↓

   /      |      |      |      |
  /       |      |      |      |
 /        |      |      |      |
          +      +      +      +

                 ↓

   /      /      /      |      |
  /      /      /       |      |
 /      /      /        |      |
```

Bullet Physics is responsible for calculating the physical interactions between the dominoes.

---

# Screenshots

The following frames show the progression of the domino simulation.

![Bullet Physics Simulation](frame_00050.png)

![Bullet Physics Simulation](frame_01000.png)

![Bullet Physics Simulation](frame_04000.png)

![Bullet Physics Simulation](frame_05000.png)

---

# Technology

The project is based on:

* C/C++
* Bullet Physics
* Magnum
* Corrade
* CMake

### Bullet Physics

Bullet Physics is used for:

* Rigid-body simulation
* Collision detection
* Gravity
* Physical interactions between dominoes

### Magnum

Magnum provides the rendering and application framework used to visualize the simulation.

### Corrade

Corrade provides utility functionality used by the Magnum ecosystem.

---

# Dependencies

Before building this project, **Corrade must be built before Magnum**.

The project expects the Magnum and Corrade source trees to be available locally.

A typical directory layout is:

```text
.
├── build
├── CMakeLists.txt
├── corrade -> ../../corrade/
├── magnum -> ../../magnum/
├── modules
├── README.md
└── src
```

---

# Building Corrade

Clone or provide the Corrade source tree and build it first.

```bash
cd corrade
mkdir build
cd build
cmake ..
cmake --build .
```

---

# Building Magnum

After Corrade has been built:

```bash
cd magnum
mkdir build
cd build

cmake .. \
    -DMAGNUM_WITH_SDL2APPLICATION=ON

make
make install
```

After Magnum has been built and installed, the domino simulation can be compiled.

---

# Building the Project

From the project root:

```bash
mkdir build
cd build
```

Configure the project:

```bash
cmake ..
```

Build:

```bash
cmake --build .
```

---

# Running

After building the project, start the generated application from the build directory.

The simulation then visualizes the domino chain and its physical interactions in real time.

---

# Project Structure

```text
magnum-domino-chain
├── build
├── CMakeLists.txt
├── corrade -> ../../corrade/
├── magnum -> ../../magnum/
├── modules
├── src
├── frame_00050.png
├── frame_01000.png
├── frame_04000.png
├── frame_05000.png
└── README.md
```

---

# Physics Pipeline

The basic simulation loop can be summarized as:

```text
Initialize Scene
      |
      v
Create Domino Rigid Bodies
      |
      v
Apply Gravity
      |
      v
Run Physics Step
      |
      v
Calculate Collisions
      |
      v
Update Transforms
      |
      v
Render Scene
      |
      +------> Next Frame
```

The physics state is updated continuously and the resulting transforms are passed to the rendering system.

---

# Project Goals

The project demonstrates several concepts that are useful when developing real-time C++ simulations:

* Rigid-body physics
* Collision detection
* Physics/rendering synchronization
* 3D visualization
* Magnum application development
* Bullet Physics integration
* CMake-based C++ development

It can also serve as a starting point for more complex physics simulations involving multiple interacting objects.

---

# License

See the repository for the applicable project license.


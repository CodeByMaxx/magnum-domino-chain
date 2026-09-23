# Magnum Domino Chain

A C++ domino-chain simulation rendered with the Magnum graphics engine.

The project demonstrates a simple physics-based domino simulation where falling dominoes trigger the next pieces in the chain.

## Features

* Domino chain simulation
* Real-time 3D rendering
* C++ implementation
* Magnum graphics engine
* CMake build system
* Visual simulation of the chain reaction

## Technology Stack

* **C++**
* **Magnum**
* **CMake**

## How It Works

The simulation starts with a sequence of dominoes.

When one domino falls, it can collide with the next domino and trigger another fall. This creates a chain reaction that propagates through the scene.

```text
Domino 1
   │
   ▼
Domino 2
   │
   ▼
Domino 3
   │
   ▼
Domino 4
   │
   ▼
Chain Reaction
```

## Project Structure

```text
magnum-domino-chain/
├── src/
├── modules/
├── CMakeLists.txt
└── README.md
```

## Build

Create a build directory:

```bash
mkdir build
cd build
```

Configure the project:

```bash
cmake ..
```

Build the application:

```bash
cmake --build .
```

## Run

After building the project, start the generated executable from the build directory.

The exact executable location depends on the configured CMake build.

## Results

The following screenshots show different stages of the domino simulation.

### Frame 50

![Domino Chain – Frame 50](frame_00050.png)

### Frame 1000

![Domino Chain – Frame 1000](frame_01000.png)

### Frame 4000

![Domino Chain – Frame 4000](frame_04000.png)

### Frame 5000

![Domino Chain – Frame 5000](frame_05000.png)

## Simulation

The main visual effect of the project is the propagation of the domino chain reaction.

The screenshots show how the simulation develops over time as more dominoes fall.

## Magnum

The project uses the **Magnum** graphics engine for rendering and application functionality.

Magnum provides the graphics framework used to display the 3D simulation.

## Possible Improvements

Possible future extensions include:

* More complex domino arrangements
* Adjustable simulation parameters
* Additional camera controls
* Different physical layouts
* More detailed visual effects
* Interactive domino placement

## Project Purpose

The project is a compact demonstration of C++ graphics programming and real-time simulation using Magnum.

It combines a simple physical interaction with a visually understandable 3D result.

## Author

**Markus**


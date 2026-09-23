# Magnum Domino Chain

A graphical **domino chain simulation** that demonstrates the propagation of motion through a sequence of connected dominoes.

The project combines simulation logic with real-time graphical rendering to visualize the chain reaction.

## Overview

The simulation starts with a sequence of dominoes positioned in the scene.

When the first domino falls, its movement can cause the following domino to fall, producing a chain reaction through the entire sequence.

The project is primarily focused on simulation, visualization and experimenting with graphical parameters.

---

## Features

* Domino chain simulation
* Real-time graphical visualization
* Configurable simulation parameters
* Animated domino states
* Example simulation frames
* Command-line configuration
* Graphical rendering

---

## Project Structure

```text
magnum-domino-chain/
│
├── ...
├── frame_00050.png
├── frame_01000.png
├── frame_04000.png
├── frame_05000.png
│
└── README.md
```

The repository also contains the source and build files required by the application.

---

## Requirements

A C++ development environment and the libraries required by the project are required to build the application.

The project uses a graphics library for rendering the simulation.

---

## Build

Build the project using the build configuration included in the repository.

After compiling, start the generated application to launch the graphical simulation.

The exact executable location depends on the selected build configuration.

---

## Simulation

The application creates a chain of dominoes and simulates their movement.

The general sequence is:

```text
Initial State
     │
     ▼
First Domino Falls
     │
     ▼
Collision With Next Domino
     │
     ▼
Next Domino Falls
     │
     ▼
Chain Reaction
     │
     ▼
Complete Domino Chain
```

The simulation updates the graphical scene continuously while the dominoes move and interact.

---

## Simulation Parameters

The project exposes configuration options for the simulation.

These parameters can be used to experiment with aspects such as:

* domino positioning
* chain dimensions
* simulation timing
* rendering
* physical behavior

The available options depend on the current application version.

---

## Screenshots

The repository contains several captured simulation frames:

### Early simulation

`frame_00050.png`

### Chain reaction

`frame_01000.png`

### Advanced state

`frame_04000.png`

### Final state

`frame_05000.png`

These frames demonstrate the progression of the domino simulation over time.

---

## Graphics

The project uses a graphical rendering library to display the simulation.

The renderer is responsible for:

* displaying the dominoes
* updating their positions
* rendering the scene
* visualizing the simulation state

The graphical component makes it possible to observe the chain reaction directly rather than only inspecting numerical simulation output.

---

## Performance

Simulation performance depends on:

* number of dominoes
* scene complexity
* rendering configuration
* simulation parameters
* available hardware

For performance comparisons, use the same simulation configuration and hardware environment.

---

## Project Goals

The project serves as an experiment in:

* real-time simulation
* graphical programming
* object movement
* collision/interaction handling
* animation
* simulation performance

The domino chain provides a simple visual example of how local interactions can produce a larger sequential process.

---

## Screenshots and Examples

Example frames included in the repository:

```text
frame_00050.png
frame_01000.png
frame_04000.png
frame_05000.png
```

They can be used to compare different stages of the simulation.

---

## Project Status

The project contains a graphical domino-chain simulation together with example output frames.

Possible future improvements include:

* additional chain configurations
* improved physical parameters
* interactive controls
* additional camera options
* performance benchmarks
* automated simulation tests
* more complex domino layouts

---

## Author

**Markus**

Simulation / C++ graphics project.


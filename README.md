# IMEJ

![example](./assets/example.gif)

IMEJ is a software renderer, implementing a fixed-function 3D graphics pipeline in C.
This is designed with an emphasis on modularity and portability.
Completed as part of a third year Computer Science project, this demonstrated an understanding of computer graphics, linear algebra and systems programming.

---

## Project Summary

IMEJ implements a classic fixed-function rendering pipeline, similar in structure to early OpenGL style graphics systems.
This takes a set of 3D mesh objects, applies geometric transformations to each, projects them from the scene to a camera view and then rasterises the result to a framebuffer.

Rather than relying on existing graphical APIs, this pipeline is implemented manually as an exercise to explore the mechanics of real-time rendering.

---

## Core Features

### Graphics Pipeline

* Fixed-function 3D rendering pipeline
* Model → World → View → Projection transformations
* Triangle rasterisation to a framebuffer in memory

### Geometry & Math

* `.obj` mesh loading
* Affine transformations:
  * Translation
  * Rotation
  * Scaling
* Matrix-based perspective projection

### Architecture

* Fully software renderer
* Modular pipeline stages
* Platform agnostic framebuffer abstraction
* Suitable for both desktop and embedded targets

---

## Output Targets

A key goal of IMEJ is portability.
The same rendering pipeline can drive multiple output backends:

### PPM Image Export

Export rendered frames directly to PPM images.

![3D sin wave plot](./assets/sinGraph.png)

---

### ANSI Terminal Renderer

Visualise 3D scenes using ASCII/ANSI output in a terminal.

![teapot drawn in a terminal](./assets/term.png)

---

### X11 GUI Application

Interactive real-time renderer using X11 on Linux.

![many utah teapots](./assets/pots.png)

---

### Embedded Display (Micro:bit + TFT LCD)

Deployed on a Micro:bit driving an external TFT LCD display, demonstrating suitability for constrained embedded systems.

![running on a microbit](./assets/microbit.jpg)

---

## Example Output

---

## Build & Usage

---

## Repository Structure

---

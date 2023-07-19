# Water-Shader-with-Tessellation

In this project, the goal is to create a water shader that utilizes tessellation, geometry shaders, and displacement maps. The program takes a simple mesh of triangles as input, tesselates the mesh to create more vertices, and manipulates the mesh in the geometry shader to generate realistic waves. The end product is a program that renders a patch of water.

## How to Run

1. Clone this repository or download the necessary files.
2. Make sure you have the required libraries installed (OpenGL, GLFW).
3. Use the provided code for vertex specification to generate the mesh or create your own mesh.
4. Use a displacement map and a water texture for realistic effects.
5. Implement the vertex, tessellation control, tessellation evaluation, geometry, and fragment shaders as described in the instructions.
6. Run the program to visualize the water simulation with waves and realistic reflections.

## Camera Options

You can choose one of the following camera paradigms:

1. Third-Person Camera: Allows movement within the world across the top of the water.
2. Globe Movement: Similar to assignment 5, this paradigm lets you view the water from a globe-like perspective.

## Vertex Specification

The program provides code to generate a mesh of quads over the range [min, max] in both 𝑥 and 𝑧 to create a flat plane. Each quad has a size specified by the step size. Alternatively, you can create your own mesh.

## Vertex Shader

The vertex shader outputs vertex positions in world coordinates and computes values like light and eye direction using uniforms for later use in the Phong-like fragment shader. The UV coordinates change over time to reflect the movement of waves in the geometry shader.

## Tesselation

The program uses a Tesselation Control Shader and a Tesselation Evaluation Shader to tesselate the quad mesh into a finer grid. Quads are used as the abstract patch type, and both inner and outer tesselation levels are set to at least 16.

## Geometry Shader

The bulk of the work in this assignment is performed by the geometry shader. It manipulates the vertices in the plane to create waves, particularly Gerstner waves. The geometry shader adds the influence of multiple waves over time to each point on the tesselated quad plane.

## Fragment Shader

The fragment shader implements a Phong-like reflection model with ambient, diffuse, and specular components. The base diffuse color is obtained from a texture (e.g., water.bmp).

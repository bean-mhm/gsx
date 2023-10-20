# What is ECX?

ECX is a C++20 library providing many of the basic stuff needed to build a game, simulation, or any other type of graphics projects.

# `ecx::ecs`

ECX lets you quickly write systems and create a world that manages those systems.

## Systems

A system is just an abstract class that defines what happens at the start, in each iteration, at the end, and when the system is triggered by an event.

## Events

When the world is running, one system might broadcast an event with an arbitrary type and some data, which will then trigger only the systems that can be triggered by that event type.

## Worlds

A world holds a list of systems, and has a run() function that starts a loop and invokes the abstract functions of the systems in the right order.

## Parallelization

ECX supports system parallelization with custom ordering. For example, the following can be easily done using update orders:

1. Run System A and wait for it to finish.

2. Run System B and System C in parallel, and wait for them both to finish.

3. Run System D.

## Loggers

ECX lets you write your own custom logger for a world, while also providing some built-in loggers by default, including a CSV logger and a `std::ostream` logger, which can be used to output to the console `std::cout` or a file.

# `ecx::math`

ECX provides a GLSL-like math and linear algebra library, useful for many graphical applications and simulations.

- Vectors
- Quaternions
- Axis-Aligned Bounding Boxes
- Circular and Spherical Bounding Shapes
- Matrices
- Transformations
- Generic Math Functions
- Random Number Generators
- ...

# `ecx::spatial`

You might often store a list of particles or objects, and later need to find the ones that are a certain distance away from a point (or closer), or inside a bounding box. A naive implementation of this usually results in a time complexity of O(N^2) which is far from ideal. For this, ECX provides spatial data structures, specifically designed to solve this issue by optimizing the way they store your objects.

- Base Container 2D/3D: An abstract class meant to store, query, and manage objects with spatial properties.
- Grid 2D/3D
- Hash Grid 2D/3D
- Quadtree
- Octree
- Linear 2D/3D (naive implementations with no optimizations, useful for benchmarking)

# More

ECX also provides other utility functions and classes that can be found in namespaces `ecx::str`, `ecx::misc`, and `ecx::common`.

# Demos

...

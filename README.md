Solo project plan:

1.
Continue work on the LILLIS engine, a c++ game engine of my own design that I've been working on continuously.
As a portfolio piece, an engine is a major undertaking and demonstrates my abilities as a tools programmer more effectively and intensely than any other option. For career goals, I wish to not rely on OOTB engines for my personal projects or larger studio work and having a self-made engine at my disposal would be immaculately helpful.

2.
The language of the engine is C++, using GLFW, MiniAudio, OpenGL/GLAD, STBI, GLM, and a few other assorted libraries, likely IMGUI in the future. The IDE and compiler tools are CLION, MSVC + CLang, and Cmake. A physics engine is yet to be decided.
The tools are justified in this context by the fact that they are already implemented in the existing engine, and as such fit within the project requirements. IMGUI in the future is likely to be implemented specifically for editor functionality to make project editing easier to implement.

3.
The project scope is to add as many of the following functionalities to the engine as possible. This is not listing optimizations.
For now I will abstain from multithreading and an editor unless someone else is to join in on the project.

-Tilemaps (Isometric and standard) with easy pathing and tile access built in.
-Normal maps, User defined editions to the shader pipeline, and postprocessing abilities.
-Physics components including joints and colliders.
-Basic audio functionality
-Prefabs/Cloneable objects
-User Defined events
-Puppet pin animation
-A particle system
-Build settings

The primary restraints or limitations are optimizations, which many of these systems will need in place within the architecture to work reasonably.

4.
The primary deliverable will be a small generic game that shows every major feature at least once. Along with a walkthrough of various systems of the interface.
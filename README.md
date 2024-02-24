# Slime mold simulation, now in C++!
- This is a port of the Python version of this simulation I made! [(Check out here)](https://github.com/AhmedBineuro/SlimeMold)
- This port uses C++ (of course) as well as [SFML](https://www.sfml-dev.org/) graphics library to improve the performance of the simulation
![Updated Slime Mold demo](Updated_Slime_Mold.mov)
## Main Improvements
- Shifting from Python to C++ gave the expected boost in performance
- Simplifying the code a lot more
  - No classes to be seen anywhere! Data oriented approach
- SHADERS!
  - Fragment shaders were used to process all the grid operations (diffusion and evaporation)
  - Diffusion is done by using a simple blurring shader and the evaporation is just done in a dimming shader!
- RGB Slime molds!
  - Now you can have 3 different colored slime molds (red, blue and green slime molds) to watch as they chase their respective groups
## Build Information
### Requirements
- g++ or similar compiler for C++
- SFML graphics library
### Build command
```shell
g++ optimized.cpp ./sources/*.cpp -o SlimeMold -I./includes -lsfml-graphics -lsfml-window -lsfml-system -O3
```
## Cool Patterns Generated
[Blue and yellow slime mold](blueNyellow.png)
[Fleshy looking slime mold](fleshmold.png)
[Red, green, and blue slime molds creating a web like pattern](NEW_WEB_PATTERN.png)

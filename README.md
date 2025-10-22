<a id="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h1 align="center">Procedural Building Generator</h1>
</div>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>


<!-- ABOUT THE PROJECT -->
## About The Project

This project is a 3D procedural house generator that interprets CGA-style shape grammar from a ```.shp``` file to build complex architectural structures in OpenGL.
Each rule in the grammar defines how shapes split, scale, translate, and combine to form walls, roofs, doors, and chimneys, allowing for dynamic and deterministic procedural modeling.   
The system consists of:
* Grammar Parser: reads ```.shp``` files defining procedural rules
* Interpreter: expands symbols recursively to generate building geometry
* OpenGL Renderer: visualizes the final #D structure with transformations and materials

### Built With
- C++ 17
- OpenGL (GLFW, GLAD)
- GLSL
- GLM for vector and matrix math

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

Follow these instructions to build and run the project locally. 

### Prerequisites
You need the following installed:
* CMake (>= 3.15)
* C++ 17 compiler
* OpenGL drivers
* GLFW and GLAD
* GLM

### Installation
1. Clone the repository
  ```sh
  git clone https://github.com/bhavyaven/BuildingGenerator.git
  ```
2. Navigate to the project directory
  ```sh
  cd BuildingGenerator
  ```
3. Load a ```.shp``` grammar file from under ```assets/``` in ```main.cpp```
```cpp
 interpreter.loadGrammar("../../assets/config3-house.shp");
```
4. Navigate to ```out\build\BuildingGenerator.sln``
5. Click "Start Without Debugging"


<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage
* Modify ```.shp``` grammar files to define new architectural rules
* Adjust view with mouse and keyboard navigation
* Extend grammar to include doors, chimneys, and multi-floor logic

### Example:
```
param width = 1.0
param height = 1.0
param depth = 1.0

# S: Start shape
shape S
split(z) { 0.5: B | 0.5: F }

# B: Back half
shape B
add_chimney(right)

# F: Front half, split into 3 blocks across x
shape F
split(x) { 0.33: I | 0.34: J | 0.33: K }

# I: Left cube
shape I
scale(z) 1.5
scale(y) 0.5

# J: Middle cube with door
shape J
scale(z) 1.5
scale(y) 0.5
add_door

# K: Right cube
shape K
scale(z) 1.5
scale(y) 0.5
```

### Result:
![alt text](<Screenshot 2025-10-22 170758.png>)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ROADMAP -->
## Roadmap

- Add window modules
- Implement randomized grammar rule selection
- Add materials and textures for realism
- GUI to edit grammar interactively

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Bhavya Venkataraghavan - bhavya.v04@gmail.com   
LinkedIn: http://www.linkedin.com/in/bhavya-venkat    
GitHub: https://github.com/bhavyaven   

Project Link: [https://github.com/bhavyaven/BuildingGenerator](https://github.com/bhavyaven/BuildingGenerator)


<p align="right">(<a href="#readme-top">back to top</a>)</p>
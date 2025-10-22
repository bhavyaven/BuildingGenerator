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
3. Build using CMake
    ```sh
    mkdir build && cd build
    cmake ..
    make
    ```
4. Load a ```.shp``` grammar file from under ```assets/```

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage
* Convenient app to keep track of reading list
* Add new books to your reading list
* Update book details (title, author, genre, reading status)
* Delete books you have finished/no longer want to read
* Filter the list dynamically by status

![alt text](<images/Screenshot 2025-10-22 155549.png>)
![alt text](<images/Screenshot 2025-10-22 155612.png>)
![alt text](<images/Screenshot 2025-10-22 155627.png>)
![alt text](<images/Screenshot 2025-10-22 155637.png>)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ROADMAP -->
## Roadmap

- Add user authentication
- Implement book rating system
- Add book recommendation feature
- Enhance frontend UI with better styling

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Bhavya Venkataraghavan - bhavya.v04@gmail.com   
LinkedIn: http://www.linkedin.com/in/bhavya-venkat    
GitHub: https://github.com/bhavyaven   

Project Link: [https://github.com/bhavyaven/ToBeRead-List](https://github.com/bhavyaven/ToBeRead-List)


<p align="right">(<a href="#readme-top">back to top</a>)</p>



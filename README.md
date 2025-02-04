# OpenGL-SSAO

## Description

This repository showcases an implementation of Screen Space Ambient Occlusion (SSAO) using C++ and GLSL with OpenGL. It demonstrates the technique of adding ambient occlusion effects in screen space to enhance the visual depth and realism of rendered scenes.

## Screenshots

![Screenshot 1](Screenshot1.png)
![Screenshot 2](Screenshot2.png)

## Dependencies

Before building and running this project, you need to install the following dependencies:

*   **GLEW (OpenGL Extension Wrangler Library):** Download GLEW and add the `include` directory and library files (e.g., `glew32.lib`) to your system environment variables.
*   **GLM (OpenGL Mathematics):** Download GLM. GLM is a header-only library, so simply copy the `glm` directory into your project's include path.

**Note:** Adding GLEW and GLM to your system environment variables ensures that Visual Studio can find them during the build process. This typically involves adding paths to the `INCLUDE` and `LIB` environment variables.

## Build Instructions (Visual Studio 2022)

Follow these steps to build the project using Visual Studio 2022:

1.  **Clone the Repository:** Clone this repository to your local machine using the following command:
    ```bash
    git clone https://github.com/Tushar-Wagdare/OpenGL-SSAO.git
    ```
2.  **Open the Solution:** Open the `OGL.sln` file in Visual Studio 2022.
3.  **Build the Solution:** Go to `Build` -> `Build Solution` (or press `Ctrl+Shift+B`).
4.  **Ensure all dependencies are present:** Make sure all the dependencies are installed and placed in the system environment.
5.  **Run the Solution:** After building, run the solution.

## Usage

After successfully building the project, you can run the executable located in the `x64/Debug` directory. The demo showcases the Screen Space Ambient Occlusion effect.

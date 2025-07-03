# Project Stone
A simple map generator written in C++, inspired by [Son of Spades](https://sonofspades.com/).

## ✨ Features
- **C++**: Core game logic and rendering engine.

## 🚀 How to Build & Run

### Prerequisites

- [CMake](https://cmake.org/)
- IDE:
  - [CLion](https://www.jetbrains.com/clion/) (Cross-platform) — *Recommended*
  - [Visual Studio](https://visualstudio.microsoft.com/) (Windows)

```bash
git clone --recursive https://github.com/nhat05thud/project-stone.git
```

### Configuration

#### ⏺ CLion Settings:

- Go to: ```Settings``` → ```Build, Execution, Deployment``` → ```CMake```
	- Generator: ```Visual Studio 17 2022```
	- CMake options:
		```bash
		-G "Visual Studio 17 2022"
		-DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=$CMakeProjectDir$/build
		-DCMAKE_CXX_STANDARD_REQUIRED:BOOL=ON
		-DCMAKE_CXX_STANDARD:STRING=23
		```
	- Build directory: ```cmake\build```

#### ⏺ Visual Studio Settings:

- Edit ```CMakeSettings.json:```
```json
{
  "configurations": [
    {
      "name": "x64-Debug",                                          //// Configuration name (Win64/Win32)
      "generator": "Visual Studio 17 2022 Win64",                   //// Generator for the project
      "configurationType": "Debug",                                 //// Configuration type (Debug/Release)
      "inheritEnvironments": [ "msvc_x64_x64" ],                    //// Inherit environment variables from the specified environment
      "buildRoot": "${projectDir}\\cmake\\build\\${name}",          //// Build directory for the configuration
      "installRoot": "${projectDir}\\cmake\\install\\${name}",      //// Install directory for the configuration
      "cmakeCommandArgs": "",
      "variables": [
        {
          "name": "CMAKE_CXX_STANDARD",                             //// C++ standard version
          "value": "23"
        },
        {
          "name": "CMAKE_CXX_STANDARD_REQUIRED",                    //// C++ standard required
          "value": "ON"
        },
        {
          "name": "CMAKE_RUNTIME_OUTPUT_DIRECTORY",                 //// Output directory for the executable
          "value": "${projectDir}\\build"
        }
      ]
    }
  ]
}
```
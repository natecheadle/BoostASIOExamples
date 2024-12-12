[![Build Status](https://dev.azure.com/natecheadle/CMakeCPPTemplate/_apis/build/status%2Fnatecheadle.CMakeCPPTemplate?branchName=main)](https://dev.azure.com/natecheadle/CMakeCPPTemplate/_build/latest?definitionId=2&branchName=main)
![Azure DevOps coverage](https://img.shields.io/azure-devops/coverage/natecheadle/CMakeCPPTemplate/2)

# Introduction
This is a template project for starting a new cmake project. 

It includes the following:
1. CMake based project for a library, app, and test binaries
2. A basic CI script for building on Windows with MSVC and Linux with specific LLVM and GCC versions
3. A clang-format file initialized from the Google template
4. A vcpkg.json file for easy integration of cmake/cpp packages

# Quickstart
1. Clone the project into a new folder
2. Delete .git folder
3. Rename folder to new project name
4. git init in new folder
5. Create a CMakeUserPresets.json file (see example below) that specifies your vcpkg location


```
{
  "version": 3,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 21,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "MyPC",
      "hidden": true,
      "toolchainFile": "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/vcpkg/scripts/buildsystems/vcpkg.cmake"
    },
    {
      "name": "Windows-MSVC-Debug-MyPC",
      "inherits": [
        "Windows-MSVC-x64-Debug",
        "MyPC"
      ],
      "binaryDir": "${sourceDir}/out/build/MSVC/Debug"
    },
    {
      "name": "Windows-MSVC-Release-MyPC",
      "inherits": [
        "Windows-MSVC-x64-Release",
        "MyPC"
      ],
      "binaryDir": "${sourceDir}/out/build/MSVC/Release"
    },
    {
      "name": "MyMac",
      "toolchainFile": "/home/user/repos/vcpkg/scripts/buildsystems/vcpkg.cmake",
      "hidden": true
    },
    {
      "name": "OSX-LLVM-Debug-MyMac",
      "inherits": ["OSX-LLVM-Debug", "MyMac"],
      "binaryDir": "${sourceDir}/out/build/LLVM/Debug"
    },
    {
      "name": "OSX-LLVM-Release-MyMac",
      "inherits": ["OSX-LLVM-Release", "MyMac"],
      "binaryDir": "${sourceDir}/out/build/LLVM/Release"
    },
    {
      "name": "MyUbuntu",
      "toolchainFile": "/home/user/repos/vcpkg/scripts/buildsystems/vcpkg.cmake",
      "hidden": true
    },
    {
      "name": "Linux-GCC-Debug-MyUbuntu",
      "inherits": ["Linux-GCC-Debug", "MyUbuntu"],
      "binaryDir": "${sourceDir}/out/build/GCC/Debug"
    },
    {
      "name": "Linux-GCC-Release-MyUbuntu",
      "inherits": ["Linux-GCC-Release", "MyUbuntu"],
      "binaryDir": "${sourceDir}/out/build/GCC/Release"
    }
  ]
}
```

# Yunomi
## A (soon to be) multi-platform game engine

## Building
Building requires three steps

### 1) Run Setup.py
Setup.py is a convenience script that copies some header only dependencies and build
scripts for github module dependencies to their proper places. Relative filepaths are
used so there shouldn't be any difficulties here.

### 2) Run Premake
This stage is different depending on your platform. The premake script provided in the
root directory should be fine, but the command you will run is different. These examples
are based on the environments used during development. If you wish to build project files
for a different IDE, for example, check out the premake wiki.

#### Windows: premake5.exe vs2022

### 3) Build Project
Now that you have your project files, provide them to your IDE/compiler of choice.

## Dependencies
Most dependencies are handled for you via github modules and/or the setup script.

Dependencies with a * are header only files. Add them by putting the header in Yunomi/Yunomi/vendor.

VulkanSDK is currently expected to be at the default installation path. If you have installed it
somewhere differently, please adjust the path in premake5.lua.

### Dependency List:
 - spdlog
 - glfw
 - glm
 - VulkanSDK
 - *tiny_obj_loader
 - *stb_image
 - OpenFBX
# Moer_ScenePreviewer
Moer_ScenePreviewer is a hardware rasterizer designed to offer fast scene previews for [Moer](https://github.com/NJUCG/Moer).

See [Moer](https://github.com/NJUCG/Moer) for more information about the scene format.

# Build
Moer_ScenePreviewer uses [CMake](https://cmake.org/) for building. You need to have CMake installed.

# Dependencies
Moer_ScenePreviewer depends on the following third-party libraries. You may try to use higher versions, but successful build is not guaranteed.
```
eigen3:x64-windows                                3.4.0#3     
glad:x64-windows                                  0.1.36      
glad[loader]:x64-windows                                      
glfw3:x64-windows                                 3.3.9       
imgui:x64-windows                                 1.90#4      
imgui[docking-experimental]:x64-windows                       
imgui[glfw-binding]:x64-windows                               
imgui[opengl3-binding]:x64-windows                            
nlohmann-json:x64-windows                         3.11.3      
tinyobjloader:x64-windows                         2.0.0-rc9
```

These dependencies can be installed using a package manager like [vcpkg](https://github.com/microsoft/vcpkg). Refer to the `Getting Started` section in vcpkg's README for a comprehensive guide on how to install these libraries and integrate vcpkg with CMake.

Please ensure all dependencies are correctly installed before attempting to build Moer_ScenePreviewer.


# Branches
The main branch uses OpenGL for rendering and CMake for building.

If you want to check the CPU rendering version of the code, please switch to the cpu-rendering branch. The cpu-rendering branch uses xmake for building.

# Gallery
![v1_0_teapot.png](https://z4a.net/images/2024/02/18/v1_0_teapot.png)
![v1_0_classromm.png](https://z4a.net/images/2024/02/18/v1_0_classromm.png)
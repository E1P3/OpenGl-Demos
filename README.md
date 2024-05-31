# graphics-year5

Opengl graphics Engine for most of my work on real time rendering and animation for my Masters. Both Mac and Windows compatible.
The Engine features a number of pre built shaders as well as a framework to easily create new shaders and gameplay modules to suit the needs of the game.
Allows for creation of models, shaders, materials and entityModules that can be added to a gameObject to allow for variety of usecases.
Simply inherit EntityModule or Shader modules to quickly iterate and test different rendering techiniques and gameplay features.

## Build

Thanks to cmake all you need is to build it and its dependencies and run the generated exe. (Assimp has to be built manually)

```mkdir build && cd build```
```cmake ..```
```cmake --build . --config Debug```

**For Windows:** make sure to add assimp dll and lib files into the folder where exe is built. 

## Demos

### Plane rotation showcase

![animation1](https://github.com/E1P3/graphics-year5/blob/main/media/anim1.gif)

Showcase of heirarchical object placement as well as the camera system of the engine. Allows to change the orientation of the plane as well as the camera, with appropriate quaternion and euler rotations presented in the UI.

### Shader showcase

![rendering1](https://github.com/E1P3/graphics-year5/blob/main/media/render1.gif)

Presentation of three shaders serving different purposes. It features a PBR shader based on cook torrace model, a toon shader and a simple blinn phong. All shaders rendered on selection of models. The UI allows to controll the material properties to change the look of each shader.

### Reflection/Refraction showcase

![rendering2](https://github.com/E1P3/graphics-year5/blob/main/media/render2.gif)

Features my implementation of transmittance shader with reflection, refraction and chromatic abberation effects built in. Aside from material properites, it also allows to change the skybox of the scene to view the shaded objects at different lighting conditions. 

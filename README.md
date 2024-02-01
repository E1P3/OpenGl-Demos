# graphics-year5

Opengl graphics Engine for most of my work on real time rendering and animation for my Masters. Both Mac and Windows compatible. 

Thanks to cmake all you need is to build it and its dependencies and run the generated exe. (Assimp has to be built manually)

```mkdir build && cd build```
```cmake ..```
```cmake --build . --config Debug```

**For Windows:** make sure to add assimp dll and lib files into the folder where exe is built. 

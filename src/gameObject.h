#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "entity.h"
#include "shader.h"
#include "texture.h"
#include <vector>

class GameObject : public Entity {

public:

private:
    std::vector<Shader*> shaders;
    std::vector<Texture*> textures;
};

#endif // GAMEOBJECT_H

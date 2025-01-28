//
// Created by Somed on 1/27/2025.
//

#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <glm/vec2.hpp>
#include "EngineStuffs/MemoryManagement/LilObj.h"

class GameObject;

class Prototype
{
public:
  static LilObj<GameObject> CreatePrototype(const std::string& name, glm::vec2 pos, float rot);
private:
  //To update names
  static std::unordered_map<std::string, unsigned int> numExisting;
};

#endif //PROTOTYPE_H

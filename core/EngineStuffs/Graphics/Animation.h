

//
// Created by Somed on 1/4/2025.
//

#ifndef ANIMATION_H
#define ANIMATION_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

enum RepeatType
{
  REPEAT_STOP,
  REPEAT_LOOP,
  REPEAT_CLAMP
};

struct FrameTransformData
{
  glm::vec3 objPos = glm::vec3(0.0f);
  glm::vec2 objScale = glm::vec2(0.0f);
  unsigned int objRot = 0;
};

struct FrameSpriteData
{
  int sprFrame = 0;
  glm::vec2 sprOffset = glm::vec2(0.0f);
  glm::vec2 sprSize = glm::vec2(-1.0f);
  //std::string sprImage = "";
  //unsigned int sprRot = 0;
};

struct FrameColliderData
{
  glm::vec2 colSize = glm::vec2(1.0f);
  glm::vec2 colOffset = glm::vec2(0.0f);
};

class KeyFrame
{
public:

  KeyFrame(float frt) {frameDuration = frt;}

  float frameDuration = 0.0f;
  //Object transformation data
  bool hasTransformData = false;
  FrameTransformData ftd;
  //Sprite data
  bool hasSpriteData = false;
  FrameSpriteData fsd;
  //ColliderData
  bool hasColliderData = false;
  FrameColliderData fcd;
};

class Animation
{
public:
  Animation(RepeatType rType)
  {
    repeatType = rType;
  }

  Animation()
  {
    repeatType = REPEAT_STOP;
  }

  ~Animation() = default;

  RepeatType getRepeatType() {return repeatType;}
  void insertKeyFrame(KeyFrame k) {keyFrames.emplace_back(k);}
  KeyFrame& getKeyFrame(size_t i) {return keyFrames[i];}
  size_t getKeyFrameCount() {return keyFrames.size();}
private:
  RepeatType repeatType = REPEAT_STOP;
  std::vector<KeyFrame> keyFrames;
};



#endif //ANIMATION_H

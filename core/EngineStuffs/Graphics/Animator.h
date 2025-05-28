//
// Created by Somed on 1/7/2025.
//

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "../Component.h"
#include "../StateObject.h"
#include "EngineStuffs/Transform.h"

class RigidBody;
class Sprite;
//class MemoryPool;

class Animator : public Component
{
public:

    ~Animator() = default;
    Animator() = default;

    void SetSingleAnimation(Animation* anim);
    void SetMultiAnimation(StateObject* stateObj);

    bool swapAtEndAnim = false;

    bool ChangeState(const std::string& newState);
    void ForceChangeState(const std::string& newState);
    std::string GetState() const {return mCurrentState->name;}
    void SetGoing(bool moving);

    void ResetAnim();
    void Update(double deltaTime);
    void ConnectComponents();

    void SetSelfID(LilObj<Animator> id)
    {
        selfRef = id;
    }

private:

    //No interpolation yet.
    void SwapKeyFrame();
    void SwapState(const std::string& newState);

    bool singleAnim = false;
    //Find way of changing state manually through this, and also getting data back.
    std::string previousState;
    StateObject* stateObject = nullptr;

    Animation* singleAnimation = nullptr;

    LilState* mCurrentState = nullptr;
    //the time per frame
    double animTime = 0;
    //Used for states
    double totalTime = 0;
    size_t currentKeyFrame = 0;
    bool isGoing = true;
    bool goingForward = true;

    LilObj<RigidBody> rectCollider;
    LilObj<Sprite> sprt;
    LilObj<Transform> transform;
    LilObj<Animator> selfRef;

    glm::vec2 startPos;
    float startRot;
};



#endif //ANIMATOR_H

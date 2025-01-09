//
// Created by Somed on 1/7/2025.
//

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "../Component.h"
#include "../StateObject.h"

class RectangleCollider;
class Sprite;

class Animator : public Component
{
public:

    ~Animator() = default;
    Animator() = default;

    void SetSingleAnimation(Animation* anim)
    {
        singleAnim = true;
        singleAnimation = anim;
        mCurrentState = nullptr;
        stateObject = nullptr;
    }

    void SetMultiAnimation(StateObject* stateObj)
    {
        singleAnim = false;
        singleAnimation = nullptr;
        stateObject = stateObj;
        mCurrentState = stateObject->GetStateByName(stateObject->GetDefaultState());
        previousState = mCurrentState->name;
    }

    bool swapAtEndAnim = false;

    bool ChangeState(const std::string& newState);
    void ForceChangeState(const std::string& newState);
    std::string GetState() const {return mCurrentState->name;}
    void SetGoing(bool moving) {isGoing = moving;}

    void ResetAnim()
    {
        currentKeyFrame = 0;
        animTime = 0;
        isGoing = true;
        goingForward = true;
        SwapKeyFrame();
    }
    void Update(double deltaTime);
    void ConnectComponents();
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
    double animTime = 0;
    double totalTime = 0;
    size_t currentKeyFrame = 0;
    bool isGoing = true;
    bool goingForward = true;

    LilObj<RectangleCollider> rectCollider;
    LilObj<Sprite> sprt;
};



#endif //ANIMATOR_H

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
        mAnim = anim;
        stateObject = nullptr;
    }

    void SetMultiAnimation(StateObject* stateObj)
    {
        singleAnim = false;
        mAnim = nullptr;
        stateObject = stateObj;
    }

    bool ChangeState(const std::string& newState);
    void ForceChangeState(const std::string& newState);
    std::string GetState() const {return currentState;}
    void SetGoing(bool moving) {isGoing = moving;}

    void ResetAnim()
    {
        currentKeyFrame = 0;
        animTime = 0;
        isGoing = true;
        SwapKeyFrame();
    }
    void Update(double deltaTime);
    void ConnectComponents();
private:

    //No interpolation yet.
    void SwapKeyFrame();

    bool singleAnim = false;
    //Find way of changing state manually through this, and also getting data back.
    std::string currentState;
    StateObject* stateObject = nullptr;

    Animation* mAnim = nullptr;
    double animTime = 0;
    size_t currentKeyFrame = 0;
    bool isGoing = true;
    bool goingForward = true;

    LilObj<RectangleCollider> rectCollider;
    LilObj<Sprite> sprt;
};



#endif //ANIMATOR_H

//
// Created by Somed on 1/7/2025.
//

#include "Animator.h"
#include "../GameObject.h"
#include "Sprite.h"
#include "../Physics/RectangleCollider.h"
#include "Utils/Events/StateChangeEvent.h"

void Animator::ConnectComponents()
{
    sprt = thisObject->getSprite();
    rectCollider = thisObject->getCollider();
}

void Animator::Update(double deltaTime)
{
    if (!isGoing)
    {
        return;
    }

    Animation* currentAnim;
    //If not a single-animation, it is confirmed that there is a connected state machine.
    if (singleAnimation)
    {
        currentAnim = singleAnimation;
    } else
    {
        currentAnim = mCurrentState->anim;
        totalTime += deltaTime;
        if (mCurrentState->maxStateTime <= totalTime && mCurrentState->maxStateTime > 0)
        {
            for (const auto& stlist : mCurrentState->toStates)
            {
                if (stlist.second == COND_FIXED_TIME)
                {
                    SwapState(stlist.first);
                    return;
                }
                if (stlist.second == COND_FIXED_TIME_RETURN)
                {
                    SwapState(previousState);
                    return;
                }
            }
        }
    }

    animTime += deltaTime;

    KeyFrame& kframe = currentAnim->getKeyFrame(currentKeyFrame);

    //Update position based on spline if exists
    if (currentAnim->followSpline && kframe.frameDuration > 0)
    {
        //Safety check
        if (currentAnim->spline.getInitted())
        {
            //TODO Remove assumption that every frame has a spline point!
            SplineInfo s = currentAnim->spline.getPosition((animTime/kframe.frameDuration) + currentKeyFrame);
            thisObject->transform.SetLocalPosition(
                glm::vec3(s.position.x + startPos.x,s.position.y + startPos.y, thisObject->transform.LocalPosition().z));
            if (currentAnim->rotSpline)
            {
                thisObject->transform.SetLocalRotation(std::atan2(s.direction.y, s.direction.x));
            }
        }
    }

    if (animTime >= kframe.frameDuration)
    {
        if (goingForward)
        {
            currentKeyFrame++;
            if (currentAnim->getKeyFrameCount() == currentKeyFrame)
            {
                switch (currentAnim->getRepeatType())
                {
                    case REPEAT_STOP:
                        if (!singleAnimation)
                        {
                            for (const auto& stlist : mCurrentState->toStates)
                            {
                                if (swapAtEndAnim)
                                {
                                    if (stlist.second == COND_USER_END_ANIM)
                                    {
                                        swapAtEndAnim = false;
                                        SwapState(stlist.first);
                                        return;
                                    }
                                } else
                                {
                                    if (stlist.second == COND_END_ANIM)
                                    {
                                        SwapState(stlist.first);
                                        return;
                                    }
                                    if (stlist.second == COND_END_ANIM_RETURN)
                                    {
                                        SwapState(previousState);
                                        return;
                                    }
                                }
                            }
                        }
                        isGoing = false;
                        break;
                    case REPEAT_LOOP:
                        currentKeyFrame = 0;
                        SwapKeyFrame();
                        break;
                    case REPEAT_CLAMP:
                        currentKeyFrame -= 2;
                        goingForward = false;
                        SwapKeyFrame();
                        break;
                    default:
                        break;
                }
            } else
            {
                SwapKeyFrame();
            }
        } else
        {
            if (currentKeyFrame == 0)
            {
                currentKeyFrame++;
                goingForward = true;
            } else
            {
                currentKeyFrame--;
            }
            SwapKeyFrame();
        }
        animTime = 0;
    }
}

void Animator::SwapState(const std::string &newState)
{
    EventSystem::getInstance()->fireEvent(StateChangeEvent(entityID, stateObject->GetName(), mCurrentState->name, newState));
    animTime = 0;
    totalTime = 0;
    currentKeyFrame = 0;
    goingForward = true;
    const std::string prev = previousState;
    mCurrentState = stateObject->GetStateByName(newState);
    previousState = prev;
    startPos = thisObject->transform.LocalPosition();
    SwapKeyFrame();
}


void Animator::SwapKeyFrame()
{
    Animation* currentAnim;
    if (singleAnimation)
    {
        currentAnim = singleAnimation;
    } else
    {
        currentAnim = mCurrentState->anim;
    }

    KeyFrame kf = currentAnim->getKeyFrame(currentKeyFrame);
    if (kf.hasSpriteData)
    {
        if (sprt.Exists())
        {
            sprt->frame = kf.fsd.sprFrame;
            sprt->image = kf.fsd.sprImage;
            sprt->offset = kf.fsd.sprOffset;
            sprt->renderSize = kf.fsd.sprSize;
        } else
        {
            std::cerr << "Animation requires a functional sprite!" << '\n';
        }
    }

    if (kf.hasColliderData)
    {
        if (rectCollider.Exists())
        {
            rectCollider->setWidth(kf.fcd.colSize.x);
            rectCollider->setHeight(kf.fcd.colSize.y);
        } else
        {
            std::cerr << "Animation requires a functional collider!" << '\n';
        }
    }

    if (kf.hasTransformData)
    {
        if (!currentAnim->followSpline)
        {
            thisObject->transform.SetLocalPosition(kf.ftd.objPos + startPos);
        }
        if (!currentAnim->rotSpline)
        {
            thisObject->transform.SetLocalRotation(kf.ftd.objRot + startRot);
        }
        thisObject->transform.SetLocalScale(kf.ftd.objScale);
    }

}


void Animator::ForceChangeState(const std::string &newState)
{
    if (!singleAnimation)
    {
        SwapState(newState);
    }
}


bool Animator::ChangeState(const std::string &newState)
{
    if (!singleAnimation)
    {
        if (stateObject->GetValidStateChange(mCurrentState->name, newState))
        {
            SwapState(newState);
            return true;
        }
        return false;
    }
    return false;
}

void Animator::SetSingleAnimation(Animation *anim)
{
    singleAnim = true;
    singleAnimation = anim;
    mCurrentState = nullptr;
    stateObject = nullptr;
    if (thisObject.Exists())
    {
        startPos = thisObject->transform.LocalPosition();
        startRot = thisObject->transform.LocalRotation();
    }
}

void Animator::SetMultiAnimation(StateObject *stateObj)
{
    singleAnim = false;
    singleAnimation = nullptr;
    stateObject = stateObj;
    mCurrentState = stateObject->GetStateByName(stateObject->GetDefaultState());
    previousState = mCurrentState->name;
    if (thisObject.Exists())
    {
        startPos = thisObject->transform.LocalPosition();
        startRot = thisObject->transform.LocalRotation();
    }
}

void Animator::ResetAnim()
{
    currentKeyFrame = 0;
    animTime = 0;
    isGoing = true;
    goingForward = true;
    startPos = thisObject->transform.LocalPosition();
    startRot = thisObject->transform.LocalRotation();
    SwapKeyFrame();
}

void Animator::SetGoing(bool moving)
{
    if (moving)
    {
        startPos = thisObject->transform.LocalPosition();
        startRot = thisObject->transform.LocalRotation();
    }
    isGoing = moving;
}


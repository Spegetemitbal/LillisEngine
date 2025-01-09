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
    if (animTime >= currentAnim->getKeyFrame(currentKeyFrame).frameDuration)
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
        thisObject->transform.localPosition += kf.ftd.objPos;
        thisObject->transform.localRotation += kf.ftd.objRot;
        thisObject->transform.localScale += kf.ftd.objScale;
    }

    if (stateObject != nullptr)
    {
        //Not implemented yet
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
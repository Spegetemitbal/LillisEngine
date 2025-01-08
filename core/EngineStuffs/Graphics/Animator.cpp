//
// Created by Somed on 1/7/2025.
//

#include "Animator.h"
#include "../GameObject.h"
#include "Sprite.h"
#include "../Physics/RectangleCollider.h"

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

    animTime += deltaTime;
    if (animTime >= mAnim->getKeyFrame(currentKeyFrame).frameDuration)
    {
        if (goingForward)
        {
            currentKeyFrame++;
            if (mAnim->getKeyFrameCount() == currentKeyFrame)
            {
                switch (mAnim->getRepeatType())
                {
                    case REPEAT_STOP:
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
    //currentState = stateObject->GetCurrentState(stateTime);
}

void Animator::SwapKeyFrame()
{
    KeyFrame kf = mAnim->getKeyFrame(currentKeyFrame);
    if (kf.hasSpriteData)
    {
        if (sprt.Exists())
        {
            sprt->frame = kf.fsd.sprFrame;
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

}


bool Animator::ChangeState(const std::string &newState)
{
    return true;
}
//
// Created by Somed on 12/23/2024.
//

#include "SceneGraph.h"
#include <queue>

SceneGraph::SceneGraph(GameObjPool* gameObjPool)
{
    mObjPool = gameObjPool;
}


void SceneGraph::SetParent(LilObj<GameObject> parent, LilObj<GameObject> child)
{
    unsigned int parentID = parent.GetID();
    unsigned int childID = child.GetID();

    unsigned int numToMove = 0;
    unsigned int numToAdd = 0;
    unsigned int startIndex = mObjPool->FindObjectIndex(mObjPool->GetObjByID<GameObject>(childID));
    unsigned int endIndex = startIndex;
    unsigned int toIndex;

    //If child is a parent, make sure no circular hierarchy is established, otherwise prep to move children.
    if (parentMap.contains(childID))
    {
        //Prep to move children
        std::queue<unsigned int> nextScan;
        nextScan.push(childID);
        while (!nextScan.empty())
        {
            if (parentMap.contains(nextScan.front()))
            {
                std::pair<std::multimap<unsigned int, unsigned int>::iterator, std::multimap<unsigned int, unsigned int>::iterator> ret;
                ret = parentMap.equal_range(nextScan.front());
                std::multimap<unsigned int, unsigned int>::iterator iter = ret.first;
                for (; iter != ret.second; ++iter)
                {
                    //Checks for circular hierarchy
                    if (parentID == iter->second)
                    {
                        std::cerr << "Circular parent detected!" << '\n';
                        return;
                    }
                    numToMove++;
                    nextScan.push(iter->second);
                }
            }
            nextScan.pop();
        }
    }

    if (childMap.contains(childID))
    {
        childMap.erase(childID);
    }

    if (!childMap.contains(childID) && !parentMap.contains(childID))
    {
        numToAdd++;
    }

    endIndex = startIndex + numToMove;

    //If parent is already in the roster
    if (parentMap.contains(parentID) || childMap.contains(parentID))
    {
        toIndex = mObjPool->FindObjectIndex(mObjPool->GetObjByID<GameObject>(parentID)) + 1;
    } else
    {
        toIndex = numInheritedObjects + 1;
        mObjPool->SwapObjects(mObjPool->poolDir[numInheritedObjects], mObjPool->GetObjByID<GameObject>(parentID));
        numToAdd++;
    }

    //Scoot all objects to their correct position
    if (numToMove > 0)
    {
        bool shiftLeft = toIndex < startIndex;
        if (shiftLeft)
        {
            while (startIndex < endIndex)
            {
                unsigned int currentIndex = startIndex;

                while (currentIndex > toIndex)
                {
                    mObjPool->SwapObjects(mObjPool->poolDir[currentIndex], mObjPool->poolDir[currentIndex - 1]);
                    currentIndex--;
                }
                startIndex++;
                toIndex++;
            }
        } else
        {
            while (startIndex < endIndex)
            {
                unsigned int currentIndex = startIndex;

                while (currentIndex < toIndex)
                {
                    mObjPool->SwapObjects(mObjPool->poolDir[currentIndex], mObjPool->poolDir[currentIndex + 1]);
                    currentIndex++;
                }
                startIndex++;
                toIndex++;
            }
        }
    } else
    {
        mObjPool->SwapObjects(mObjPool->poolDir[startIndex], mObjPool->poolDir[toIndex]);
    }

    parentMap.emplace(parentID, childID);
    childMap.emplace(childID, parentID);
    numInheritedObjects += numToAdd;
}

void SceneGraph::RemoveParent(LilObj<GameObject> child, ObjectRemovalFlag removalFlag)
{
    if (!childMap.contains(child.GetID()))
    {
        return;
    }

    size_t numberOfRemoved = 1;
    bool removeParent = false;

    if (parentMap.count(childMap[child.GetID()]) == 1 && !childMap.contains(GetParent(child).GetID()))
    {
        removeParent = true;
        parentMap.erase(childMap[child.GetID()]);
        numberOfRemoved++;
    }

    std::queue<unsigned int> nextScan;
    nextScan.push(child.GetID());
    while (!nextScan.empty())
    {
        if (parentMap.contains(nextScan.front()))
        {
            std::pair<std::multimap<unsigned int, unsigned int>::iterator, std::multimap<unsigned int, unsigned int>::iterator> ret;
            ret = parentMap.equal_range(nextScan.front());
            std::multimap<unsigned int, unsigned int>::iterator iter = ret.first;
            for (; iter != ret.second; ++iter)
            {
                numberOfRemoved++;
                nextScan.push(iter->second);
            }
            parentMap.erase(nextScan.front());
        }
        childMap.erase(nextScan.front());
        if (removalFlag == OBJECTREMOVAL_DESTROY)
        {
            mObjPool->GetObjByID<GameObject>(nextScan.front())->SetActive(false);
        }
        nextScan.pop();
    }

    if (numberOfRemoved == numInheritedObjects)
    {
        numInheritedObjects = 0;
        return;
    }

    //Move everything left
    unsigned int baseIndex = mObjPool->FindObjectIndex(mObjPool->GetObjByID<GameObject>(child.GetID()));
    if (removeParent)
    {
        baseIndex--;
    }

    unsigned int startIndex = baseIndex + numberOfRemoved;
    while (startIndex < baseIndex)
    {
        unsigned int currentIndex = startIndex;

        while (currentIndex < numInheritedObjects - 1)
        {
            mObjPool->SwapObjects(mObjPool->poolDir[currentIndex], mObjPool->poolDir[currentIndex + 1]);
            currentIndex++;
        }
        startIndex++;
        numInheritedObjects--;
    }

}


LilObj<GameObject> SceneGraph::GetParent(LilObj<GameObject> child)
{
    if (childMap.find(child.GetID()) != childMap.end())
    {
        return {mObjPool,childMap[child.GetID()]};
    }
    return {};
}

//Parents are always to the left of children!
std::unordered_set<unsigned int> SceneGraph::DoForwardKinematics()
{
    std::unordered_set<unsigned int> result;

    for (int i = 0; i < numInheritedObjects; i++)
    {
        GameObject* obj = mObjPool->poolDir[i];
        if (obj->transform.toUpdate)
        {
            if (childMap.contains(obj->GetID()))
            {
                GameObject* parentObj = mObjPool->GetObjByID<GameObject>(childMap[obj->GetID()]);
                obj->transform.globalPosition = parentObj->transform.globalPosition + obj->transform.localPosition;
                obj->transform.globalRotation = parentObj->transform.globalRotation + obj->transform.localRotation;
                obj->transform.globalScale = parentObj->transform.globalScale * obj->transform.localScale;
            } else
            {
                obj->transform.globalPosition = obj->transform.localPosition;
                obj->transform.globalRotation = obj->transform.localRotation;
                obj->transform.globalScale = obj->transform.localScale;
            }
            if (obj->getSprite().Exists())
            {
                result.insert(obj->getSprite().GetID());
            }
            obj->transform.toUpdate = false;
        }
    }

    for (unsigned int i = numInheritedObjects; i < mObjPool->activeLine; i++)
    {
        GameObject* obj = mObjPool->poolDir[i];
        if (obj->transform.toUpdate)
        {
            obj->transform.globalPosition = obj->transform.localPosition;
            obj->transform.globalRotation = obj->transform.localRotation;
            obj->transform.globalScale = obj->transform.localScale;
            if (obj->getSprite().Exists())
            {
                result.insert(obj->getSprite().GetID());
            }
            obj->transform.toUpdate = false;
        }
    }
    return result;
}

void SceneGraph::ClearHierarchy()
{
    numInheritedObjects = 0;
    childMap.clear();
    parentMap.clear();
}


//
// Created by Somed on 12/23/2024.
//

#include "SceneGraph.h"
#include <queue>

SceneGraph::SceneGraph(ComponentPool<Transform>* gameObjPool)
{
    mObjPool = gameObjPool;
}


void SceneGraph::SetParent(LilObj<Transform> parent, LilObj<Transform> child)
{
    unsigned int parentID = parent.GetID();
    unsigned int childID = child.GetID();

    unsigned int numToMove = 0;
    unsigned int numToAdd = 0;
    unsigned int startIndex = mObjPool->FindObjectIndex(mObjPool->GetObjByID<Transform>(childID));
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
        toIndex = mObjPool->FindObjectIndex(mObjPool->GetObjByID<Transform>(parentID)) + 1;
    } else
    {
        toIndex = numInheritedObjects + 1;
        mObjPool->SwapObjects(mObjPool->poolDir[numInheritedObjects], mObjPool->GetObjByID<Transform>(parentID));
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
    child->isChild = true;
}

void SceneGraph::RemoveParent(LilObj<Transform> child, ObjectRemovalFlag removalFlag)
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
            mObjPool->GetObjByID<Transform>(nextScan.front())->SetActive(false);
        }
        nextScan.pop();
    }

    if (numberOfRemoved == numInheritedObjects)
    {
        numInheritedObjects = 0;
        return;
    }

    //Move everything left
    unsigned int baseIndex = mObjPool->FindObjectIndex(mObjPool->GetObjByID<Transform>(child.GetID()));
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

    child->isChild = false;

}


LilObj<Transform> SceneGraph::GetParent(LilObj<Transform> child)
{
    if (childMap.find(child.GetID()) != childMap.end())
    {
        return {mObjPool,childMap[child.GetID()]};
    }
    return {};
}

std::vector<LilObj<Transform>> SceneGraph::GetImmediateChildren(LilObj<Transform> child)
{
    if (parentMap.count(child.GetID()) > 0)
    {
        std::vector<LilObj<Transform>> v = std::vector<LilObj<Transform>>(parentMap.count(child.GetID()));
        auto range = parentMap.equal_range(child.GetID());
        for (auto it = range.first; it != range.second; ++it) {
            v.emplace_back(mObjPool, it->second);
        }
        return v;
    }
    return {};
}


//Parents are always to the left of children!
void SceneGraph::DoForwardKinematics(bool noFlagUpdate)
{
    //Do active check here
    int numInactive = 0;

    for (int i = 0; i < numInheritedObjects; i++)
    {
        Transform* obj = mObjPool->poolDir[i];
        if (obj->GetActive())
        {
            if (obj->toUpdate)
            {
                if (parentMap.contains(obj->GetID()))
                {
                    auto range = parentMap.equal_range(obj->GetID());
                    for (auto it = range.first; it != range.second; ++it) {
                        mObjPool->GetObjByID<Transform>(it->second)->toUpdate = true;
                    }
                }

                if (childMap.contains(obj->GetID()))
                {
                    auto* parentObj = mObjPool->GetObjByID<Transform>(childMap[obj->GetID()]);
                    obj->globalPosition = parentObj->globalPosition + obj->localPosition;
                    obj->globalRotation = parentObj->globalRotation + obj->localRotation;
                    obj->globalScale = parentObj->globalScale * obj->localScale;
                } else
                {
                    obj->globalPosition = obj->localPosition;
                    obj->globalRotation = obj->localRotation;
                    obj->globalScale = obj->localScale;
                }

                if (!noFlagUpdate)
                {
                    obj->toUpdate = false;
                }
            }
        } else
        {
            numInactive++;
        }
    }

    for (unsigned int i = numInheritedObjects; i < mObjPool->activeLine; i++)
    {
        Transform* obj = mObjPool->poolDir[i];
        if (obj->GetActive())
        {
            //if (obj->toUpdate)
            //{
            //    obj->globalPosition = obj->localPosition;
            //    obj->globalRotation = obj->localRotation;
            //    obj->globalScale = obj->localScale;

            //    obj->toUpdate = false;
            //}
        } else
        {
            numInactive++;
        }
    }

    mObjPool->CompactPool(numInactive);
}

void SceneGraph::ClearHierarchy()
{
    numInheritedObjects = 0;
    childMap.clear();
    parentMap.clear();
}


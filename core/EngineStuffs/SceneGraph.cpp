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
    //If already a child
    if (childMap.contains(childID) || parentMap.contains(childID))
    {
        std::cout << "Moving existing hierarchy objects not Implemented" << '\n';
        return;
    }

    //If parent is already in the roster
    if (parentMap.contains(parentID))
    {
        //Scoot everything to the right, then
        unsigned int startIndex = mObjPool->FindObjectIndex(mObjPool->GetObjByID<GameObject>(parentID)) + 1;
        unsigned int lMove = numInheritedObjects - 1;
        unsigned int rMove = numInheritedObjects;

        while (lMove >= startIndex)
        {
            mObjPool->SwapObjects(mObjPool->poolDir[lMove], mObjPool->poolDir[rMove]);
            lMove--;
            rMove--;
        }

        mObjPool->SwapObjects(mObjPool->poolDir[startIndex], mObjPool->GetObjByID<GameObject>(childID));
        numInheritedObjects++;

    } else
    {
        mObjPool->SwapObjects(mObjPool->GetObjByID<GameObject>(parentID), mObjPool->poolDir[numInheritedObjects]);
        numInheritedObjects++;
        mObjPool->SwapObjects(mObjPool->GetObjByID<GameObject>(childID), mObjPool->poolDir[numInheritedObjects]);
        numInheritedObjects++;
    }
    parentMap.emplace(parentID, childID);
    childMap.emplace(childID, parentID);
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


void SceneGraph::DoForwardKinematics()
{
    for (int i = 0; i < numInheritedObjects; i++)
    {
        GameObject* obj = mObjPool->poolDir[i];
        if (childMap.contains(obj->GetID()))
        {
            GameObject* parentObj = mObjPool->GetObjByID<GameObject>(childMap[obj->GetID()]);
            obj->transform.globalPosition = parentObj->transform.globalPosition + obj->transform.localPosition;
            obj->transform.globalRotation = parentObj->transform.globalRotation + obj->transform.localRotation;
            obj->transform.globalScale = parentObj->transform.globalScale + obj->transform.localScale;
        } else
        {
            obj->transform.globalPosition = obj->transform.localPosition;
            obj->transform.globalRotation = obj->transform.localRotation;
            obj->transform.globalScale = obj->transform.localScale;
        }
    }

    for (unsigned int i = numInheritedObjects; i < mObjPool->poolDir.size(); i++)
    {
        GameObject* obj = mObjPool->poolDir[i];
        obj->transform.globalPosition = obj->transform.localPosition;
        obj->transform.globalRotation = obj->transform.localRotation;
        obj->transform.globalScale = obj->transform.localScale;
    }
}

void SceneGraph::ClearHierarchy()
{
    numInheritedObjects = 0;
    childMap.clear();
    parentMap.clear();
}


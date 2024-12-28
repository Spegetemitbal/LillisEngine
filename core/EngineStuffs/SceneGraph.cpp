//
// Created by Somed on 12/23/2024.
//

#include "SceneGraph.h"

#include "Utils/Events/ObjectAliveEvent.h"

void SceneGraph::handleEvent(const Event& theEvent)
{
    if (theEvent.getType() == OBJECT_ALIVE_EVENT)
    {
        const ObjectAliveEvent& alvEvent = dynamic_cast<const ObjectAliveEvent&>(theEvent);
        if (parentMap.contains(alvEvent.getID()) || childMap.contains(alvEvent.getID()))
        {
            if (alvEvent.getDeleted() && !alvEvent.getEnabled())
            {

            } else if (alvEvent.getEnabled())
            {

            }
        }
    }
}


SceneGraph::SceneGraph(GameObjPool* gameObjPool)
{
    mObjPool = gameObjPool;
    EventSystem::getInstance()->addListener((EventType)OBJECT_ALIVE_EVENT,this);
}

SceneGraph::~SceneGraph()
{
    EventSystem::getInstance()->removeListenerFromAllEvents(this);
}


void SceneGraph::SetParent(LilObj<GameObject> parent, LilObj<GameObject> child)
{
    unsigned int parentID = parent.GetID();
    unsigned int childID = child.GetID();
    //If already a child
    if (childMap.contains(childID))
    {
        std::cout << "Swapping parents not implemented" << '\n';
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

void SceneGraph::RemoveParent(LilObj<GameObject> child)
{
    //objectsWithInheritence.erase(child);

    //Update roots
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
        } else
        {
            obj->transform.globalPosition = obj->transform.localPosition;
        }
    }

    for (int i = numInheritedObjects; i < mObjPool->poolDir.size(); i++)
    {
        GameObject* obj = mObjPool->poolDir[i];
        obj->transform.globalPosition = obj->transform.localPosition;
    }
}

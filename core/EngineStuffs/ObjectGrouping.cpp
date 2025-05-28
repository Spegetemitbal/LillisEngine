//
// Created by Somed on 5/22/2025.
//

#include "ObjectGrouping.h"


void ObjectGrouping::SetParent(LilObj<GameObject> parent, LilObj<GameObject> child)
{
    unsigned int parentID = parent.GetID();
    unsigned int childID = child.GetID();

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
                std::pair<std::multimap<unsigned int, LilObj<GameObject>>::iterator, std::multimap<unsigned int, LilObj<GameObject>>::iterator> ret;
                ret = parentMap.equal_range(nextScan.front());
                std::multimap<unsigned int, LilObj<GameObject>>::iterator iter = ret.first;
                for (; iter != ret.second; ++iter)
                {
                    //Checks for circular hierarchy
                    if (parentID == iter->second->GetID())
                    {
                        std::cerr << "Circular parent detected!" << '\n';
                        return;
                    }
                    nextScan.push(iter->second->GetID());
                }
            }
            nextScan.pop();
        }
    }

    if (childMap.contains(childID))
    {
        childMap.erase(childID);
    }

    parentMap.emplace(parentID, child);
    childMap.emplace(childID, parent);
}

void ObjectGrouping::RemoveParent(LilObj<GameObject> obj, ObjectRemovalFlag removalFlag)
{
    if (!childMap.contains(obj.GetID()))
    {
        return;
    }
    LilObj<GameObject> parent = childMap.at(obj.GetID());
    if (!parentMap.contains(parent.GetID()))
    {
        return;
    }
    childMap.erase(obj.GetID());

    auto range = parentMap.equal_range(obj.GetID());
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second->GetID() == obj->GetID())
        {
            parentMap.erase(it);
            return;
        }
    }

    if (removalFlag == OBJECTREMOVAL_DESTROY || removalFlag == OBJECTREMOVAL_DISABLE)
    {
        std::vector<LilObj<GameObject>> children = GetImmediateChildren(obj);
        for (auto it : children)
        {
            if (removalFlag == OBJECTREMOVAL_DESTROY)
            {
                it->SetActive(false);
            } else
            {
                RemoveParent(it, removalFlag);
            }
        }
    }
}


LilObj<GameObject> ObjectGrouping::GetParent(LilObj<GameObject> child)
{
    if (childMap.contains(child.GetID()))
    {
        return childMap.at(child.GetID());
    }
    std::cout << "Object is not a parent." << std::endl;
    return {};
}

std::vector<LilObj<GameObject>> ObjectGrouping::GetImmediateChildren(LilObj<GameObject> parent)
{
    if (parentMap.count(parent.GetID()) > 0)
    {
        std::vector<LilObj<GameObject>> v = std::vector<LilObj<GameObject>>(parentMap.count(parent.GetID()));
        auto range = parentMap.equal_range(parent.GetID());
        for (auto it = range.first; it != range.second; ++it) {
            v.push_back(it->second);
        }
        return v;
    }
    return {};
}

std::vector<LilObj<GameObject> > ObjectGrouping::GetAllChildren(LilObj<GameObject> parent)
{
    if (parentMap.count(parent.GetID()) > 0)
    {
        std::vector<LilObj<GameObject>> AllChildren = std::vector<LilObj<GameObject>>(parentMap.count(parent.GetID()));
        std::queue<LilObj<GameObject>> q = std::queue<LilObj<GameObject>>();
        q.push(parent);

        while (!q.empty())
        {
            LilObj<GameObject> par = q.front();
            q.pop();

            if (parentMap.count(par.GetID()) > 0)
            {
                auto range = parentMap.equal_range(par.GetID());
                for (auto it = range.first; it != range.second; ++it) {
                    q.push(it->second);
                    AllChildren.push_back(it->second);
                }
            }
        }
        return AllChildren;
    }
    return {};
}

std::vector<LilObj<Animator> > ObjectGrouping::GetAnimatorsInChildren(LilObj<GameObject> parent)
{
    std::vector<LilObj<GameObject>> children = GetAllChildren(parent);
    std::vector<LilObj<Animator>> animators;
    for (auto it : children)
    {
        LilObj<Animator> comp = it->getAnimator();
        if (comp.Exists())
        {
            animators.push_back(comp);
        }
    }
    return animators;
}

std::vector<LilObj<Sprite> > ObjectGrouping::GetSpritesInChildren(LilObj<GameObject> parent)
{
    std::vector<LilObj<GameObject>> children = GetAllChildren(parent);
    std::vector<LilObj<Sprite>> sprites;
    for (auto it : children)
    {
        LilObj<Sprite> comp = it->getSprite();
        if (comp.Exists())
        {
            sprites.push_back(comp);
        }
    }
    return sprites;
}

std::vector<LilObj<RigidBody> > ObjectGrouping::GetRigidBodiesInChildren(LilObj<GameObject> parent)
{
    std::vector<LilObj<GameObject>> children = GetAllChildren(parent);
    std::vector<LilObj<RigidBody>> rbs;
    for (auto it : children)
    {
        LilObj<RigidBody> comp = it->getRigidBody();
        if (comp.Exists())
        {
            rbs.push_back(comp);
        }
    }
    return rbs;
}

void ObjectGrouping::DeleteAllChildren(LilObj<GameObject> parent)
{
    std::vector<LilObj<GameObject>> children = GetAllChildren(parent);
    for (auto it : children)
    {
        it->SetActive(false);
    }
}

void ObjectGrouping::DisableEnableAllChildren(LilObj<GameObject> parent, bool able)
{
    std::vector<LilObj<GameObject>> children = GetAllChildren(parent);
    for (auto it : children)
    {
        it->SetEnabled(able);
    }
}



void ObjectGrouping::ClearHierarchy()
{
    childMap.clear();
    parentMap.clear();
}
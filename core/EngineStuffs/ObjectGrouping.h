//
// Created by Somed on 5/22/2025.
//

#ifndef OBJECTGROUPING_H
#define OBJECTGROUPING_H

#include "GameObject.h"

class ObjectGrouping {
public:
    ObjectGrouping() = default;
    ~ObjectGrouping() = default;

    void SetParent(LilObj<GameObject> parent, LilObj<GameObject> child);
    //Remove object from hierarchy, if removalflag is disable, removes all children from hierarchy too.
    //If removalflag is destroy, deletes all involved objects.
    void RemoveParent(LilObj<GameObject> obj, ObjectRemovalFlag removalFlag);
    LilObj<GameObject> GetParent(LilObj<GameObject> child);
    std::vector<LilObj<GameObject>> GetImmediateChildren(LilObj<GameObject> parent);
    std::vector<LilObj<GameObject>> GetAllChildren(LilObj<GameObject> parent);
    void ClearHierarchy();

    std::vector<LilObj<Sprite>> GetSpritesInChildren(LilObj<GameObject> parent);
    std::vector<LilObj<RigidBody>> GetRigidBodiesInChildren(LilObj<GameObject> parent);
    std::vector<LilObj<Animator>> GetAnimatorsInChildren(LilObj<GameObject> parent);

    void DeleteAllChildren(LilObj<GameObject> parent);
    void DisableEnableAllChildren(LilObj<GameObject> parent, bool able);

    template<typename T>
    std::vector<LilObj<T>> GetBehaviorInChildren(LilObj<GameObject> parent, std::string behvName)
    {
        std::vector<LilObj<GameObject>> children = GetAllChildren(parent);
        std::vector<LilObj<T>> behaviors;
        for (auto it : children)
        {
            LilObj<T> comp = it->GetBehavior<T>(behvName);
            if (comp.Exists())
            {
                behaviors.push_back(comp);
            }
        }
        return behaviors;
    }

private:

    //child first, parent second
    std::map<unsigned int, LilObj<GameObject>> childMap;
    //parent first, child second
    std::multimap<unsigned int, LilObj<GameObject>> parentMap;

    const int MAX_GRAPH_DEPTH = 5;
};



#endif //OBJECTGROUPING_H

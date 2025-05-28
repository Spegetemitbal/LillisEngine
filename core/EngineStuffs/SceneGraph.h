//
// Created by Somed on 12/23/2024.
//

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "MemoryManagement/ComponentPool.h"
#include "Transform.h"
#include <unordered_set>

class SceneGraph
{
public:
    SceneGraph() = delete;
    ~SceneGraph() = default;
    SceneGraph(ComponentPool<Transform>* gameObjPool);

    void SetParent(LilObj<Transform> parent, LilObj<Transform> child);
    void RemoveParent(LilObj<Transform> child, ObjectRemovalFlag removalFlag);
    LilObj<Transform> GetParent(LilObj<Transform> child);
    std::vector<LilObj<Transform>> GetImmediateChildren(LilObj<Transform> parent);
    void DoForwardKinematics(bool noFlagUpdate = false);
    void ClearHierarchy();

private:
    ComponentPool<Transform>* mObjPool;

    //child first, parent second
    std::map<unsigned int, unsigned int> childMap;
    //parent first, child second
    std::multimap<unsigned int, unsigned int> parentMap;
    unsigned int numInheritedObjects = 0;

    const int MAX_GRAPH_DEPTH = 5;
};



#endif //SCENEGRAPH_H

//
// Created by Somed on 12/23/2024.
//

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "MemoryManagement/GameObjPool.h"

enum ObjectRemovalFlag
{
    OBJECTREMOVAL_NONE,
    OBJECTREMOVAL_DESTROY,
    OBJECTREMOVAL_DISABLE
};

class SceneGraph
{
public:
    SceneGraph() = delete;
    ~SceneGraph() = default;
    SceneGraph(GameObjPool* gameObjPool);

    void SetParent(LilObj<GameObject> parent, LilObj<GameObject> child);
    void RemoveParent(LilObj<GameObject> child, ObjectRemovalFlag removalFlag);
    LilObj<GameObject> GetParent(LilObj<GameObject> child);
    std::vector<LilObj<GameObject>> GetImmediateChildren(LilObj<GameObject> child);
    std::unordered_set<unsigned int> DoForwardKinematics();
    void ClearHierarchy();

private:
    GameObjPool* mObjPool;

    //child first, parent second
    std::map<unsigned int, unsigned int> childMap;
    //parent first, child second
    std::multimap<unsigned int, unsigned int> parentMap;
    unsigned int numInheritedObjects = 0;

    const int MAX_GRAPH_DEPTH = 5;
};



#endif //SCENEGRAPH_H

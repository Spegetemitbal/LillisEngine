//
// Created by Somed on 12/23/2024.
//

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "MemoryManagement/GameObjPool.h"
#include "Utils/Events/EventListener.h"

class SceneGraph : public EventListener
{
public:
    SceneGraph() = delete;
    ~SceneGraph();
    SceneGraph(GameObjPool* gameObjPool);

    void SetParent(LilObj<GameObject> parent, LilObj<GameObject> child);
    void RemoveParent(LilObj<GameObject> child);
    LilObj<GameObject> GetParent(LilObj<GameObject> child);
    void DoForwardKinematics();
private:
    void handleEvent(const Event& theEvent) override;
    GameObjPool* mObjPool;

    //child first, parent second
    std::map<unsigned int, unsigned int> childMap;
    //parent first, child second
    std::multimap<unsigned int, unsigned int> parentMap;
    unsigned int numInheritedObjects = 0;
};



#endif //SCENEGRAPH_H

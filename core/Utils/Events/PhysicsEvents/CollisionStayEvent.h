//
// Created by Somed on 5/24/2025.
//

#ifndef COLLISIONSTAYEVENT_H
#define COLLISIONSTAYEVENT_H
#include <glm/vec2.hpp>

#include "Utils/Events/GameEvent.h"


class CollisionStayEvent : public GameEvent
{
public:
    CollisionStayEvent(int thisID, int otherID, int thisTag, int otherTag, glm::vec2 normal, float depth, int contactCount,
        glm::vec2 contact1, glm::vec2 contact2);
    ~CollisionStayEvent() {};
    int getThisCollider() const { return mFirst; };
    int getThatCollider() const { return mSecond; };
    glm::vec2 getNormal() const {return Normal; };
    float getDepth() const {return Depth; };
    int getContactCount() const {return ContactCount; };
    glm::vec2 getContact1() const {return Contact1; };
    glm::vec2 getContact2() const {return Contact2; };
    int getThisTag() const { return mFirstColTag; };
    int getThatTag() const { return mSecondColTag; };
    bool containsID(unsigned int ID) const override
    {
        return ID == mFirst || ID == mSecond;
    };
private:
    int mFirst = -1;
    int mSecond = -1;
    glm::vec2 Normal = {};
    float Depth;
    glm::vec2 Contact1 = {}, Contact2 = {};
    int ContactCount;
    int mFirstColTag = 0;
    int mSecondColTag = 0;
};



#endif //COLLISIONSTAYEVENT_H

//
// Created by Somed on 5/24/2025.
//

#include "CollisionStayEvent.h"

CollisionStayEvent::CollisionStayEvent(int thisID, int otherID, int thisTag, int otherTag, glm::vec2 normal,
    float depth, int contactCount, glm::vec2 contact1, glm::vec2 contact2) : GameEvent(COLLISION_STAY_EVENT)
{
    mFirst = thisID;
    mSecond = otherID;
    Normal = normal;
    Depth = depth;
    ContactCount = contactCount;
    Contact1 = contact1;
    Contact2 = contact2;
    mFirstColTag = thisTag;
    mSecondColTag = otherTag;
}
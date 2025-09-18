//
// Created by Somed on 2/15/2025.
//

#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H
#include <glm/vec2.hpp>

#include "AABB.h"

class RigidBody;
class TileCollider;

class CollisionChecker {
public:

    //If 2 points are closer than this, they are the same point.
    static inline const float NearlyEqual = 0.0005f;

    static void FindContactPoints(RigidBody* bodyA, const TileCollider* bodyB, glm::vec2& contact1, glm::vec2& contact2, int& contactCount);
    static void FindContactPoints(RigidBody* bodyA, RigidBody* bodyB, glm::vec2& contact1, glm::vec2& contact2, int& contactCount);

    static bool IntersectAABBs(AABB a, AABB b);
    static bool GetNearlyEqual(glm::vec2 a, glm::vec2 b);
    static bool GetNearlyEqual(float a, float b);

    //Circle stuff
    static bool IntersectCircles(glm::vec2 centerA, glm::vec2 centerB, float radA, float radB, float& depth, glm::vec2& normal);
    static bool IntersectCirclePolygon(glm::vec2 circleCenter, float circleRad, glm::vec2 polyCenter, glm::vec2 *vertices, int len, glm::vec2 &normal, float& depth);
    //Polygon stuff
    static bool IntersectPolygons(glm::vec2 *verticesA, int aLen, glm::vec2 centerA, glm::vec2 *verticesB, int bLen, glm::vec2 centerB, glm::vec2 &normal, float &depth);

    //Collision via SAT
    static bool CollideCheck(RigidBody bodyA, RigidBody bodyB, glm::vec2 &normal, float &depth);
    static bool CollideCheck(RigidBody bodyA, TileCollider bodyB, glm::vec2 &normal, float &depth);

    //Raycasting
    static bool RayCastCheck(RigidBody body, glm::vec2 *lineData);
    static bool RayCastCheck(TileCollider body, glm::vec2 *lineData);
    static void GetRayContacts(RigidBody* body, glm::vec2 *lineData, glm::vec2& contact1, glm::vec2& contact2, int& contactCount);
    static void GetRayContacts(const TileCollider* body, glm::vec2 *lineData, glm::vec2& contact1, glm::vec2& contact2, int& contactCount);

    //Resolve
    static void ResolveCollision(const RigidBody& rb1, const RigidBody& rb2, float depth, glm::vec2 normal);

    //Find the closest point to point P.
    static void PointSegmentDistance(glm::vec2 p, glm::vec2 a, glm::vec2 b, float& distanceSquared, glm::vec2& contact);
    static int FindClosestPointOnPolygon(glm::vec2 circleCenter, glm::vec2 *vertices, int len);

private:


    static void ProjectCircle(glm::vec2 center, float radius, glm::vec2 axis, float &min, float &max);
    static void ProjectVertices(glm::vec2 *vertices, int len, glm::vec2 axis, float &min, float &max);

    //Circle-Circle
    static void FindContactPoint(glm::vec2 centerA, glm::vec2 centerB, float radA, glm::vec2& contact);
    //Circle-Polygon
    static void FindContactPoint(glm::vec2 centerA, float radA, glm::vec2 polyCenter, glm::vec2 *vertices, int len, glm::vec2& contact);
    //Polygon-Polygon
    static void FindContactPoint(glm::vec2 *verticesA, int aLen, glm::vec2 *verticesB, int bLen, glm::vec2& contact1, glm::vec2& contact2, int& contactCount);
    //Line-Circle
    static void FindContactPoint(glm::vec2 center, float rad, glm::vec2 *lineData, glm::vec2& contact1, glm::vec2& contact2, int& contactCount);
    //Line-Polygon
    static void FindContactPoint(glm::vec2 *vertices, int numVerts, glm::vec2 *lineData, glm::vec2& contact1, glm::vec2& contact2, int& contactCount);
};



#endif //COLLISIONCHECKER_H

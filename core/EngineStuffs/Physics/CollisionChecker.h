//
// Created by Somed on 2/15/2025.
//

#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H
#include <glm/vec2.hpp>

class RigidBody;

class CollisionChecker {
public:
    //Circle stuff
    static bool IntersectCircles(glm::vec2 centerA, glm::vec2 centerB, float radA, float radB, float& depth, glm::vec2& normal);
    static bool IntersectCirclePolygon(glm::vec2 circleCenter, float circleRad, glm::vec2 *vertices, glm::vec2 &normal, float& depth);
    //Polygon stuff
    static bool IntersectPolygons(glm::vec2 *verticesA, glm::vec2 *verticesB, glm::vec2 &normal, float &depth);
    //Resolve
    static void ResolveCollision(const RigidBody& rb1, const RigidBody& rb2, float depth, glm::vec2 normal);
private:
    static glm::vec2 FindArithmeticMean(glm::vec2 *vertices, int len);
    static void ProjectCircle(glm::vec2 center, float radius, glm::vec2 axis, float &min, float &max);
    static int FindClosestPointOnPolygon(glm::vec2 circleCenter, glm::vec2 *vertices, int len);
    static void ProjectVertices(glm::vec2 *vertices, int len, glm::vec2 axis, float &min, float &max);
};



#endif //COLLISIONCHECKER_H

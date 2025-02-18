//
// Created by Somed on 2/15/2025.
//

#include "CollisionChecker.h"

//#include <glm/detail/func_geometric.inl>

#include "RigidBody.h"
#include "EngineStuffs/GameObject.h"
#include "EngineStuffs/Transform.h"

bool CollisionChecker::IntersectCirclePolygon(glm::vec2 circleCenter, float circleRad, glm::vec2 *vertices, glm::vec2 &normal, float &depth)
{
    normal = glm::vec2(0);
    depth = std::numeric_limits<float>::max();

    int len = sizeof(vertices) / sizeof(glm::vec2);

    glm::vec2 axis = glm::vec2(0);
    float minA, minB, maxA, maxB, axisDepth;

    for (int i = 0; i < len; i++)
    {
        glm::vec2 va = vertices[i];
        glm::vec2 vb = vertices[(i + 1) % len];
        glm::vec2 edge = vb - va;
        //Make sure this is clockwise in winding order
        axis = {-edge.y, edge.x};

        ProjectVertices(vertices, len, axis, minA, maxA);
        ProjectCircle(circleCenter, circleRad, axis, minB, maxB);

        if (minA >= maxB || minB >= maxA)
        {
            //There be a gap
            return false;
        }

        //Find the smallest possible correction needed
        axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < depth)
        {
            depth = axisDepth;
            normal = axis;
        }
    }

    int clpIndex = FindClosestPointOnPolygon(circleCenter, vertices, len);
    glm::vec2 closestPoint = vertices[clpIndex];
    axis = closestPoint - circleCenter;

    ProjectVertices(vertices, len, axis, minA, maxA);
    ProjectCircle(circleCenter, circleRad, axis, minB, maxB);

    if (minA >= maxB || minB >= maxA)
    {
        return false;
    }

    axisDepth = std::min(maxB - minA, maxA - minB);

    if (axisDepth < depth)
    {
        depth = axisDepth;
        normal = axis;
    }

    depth /= glm::length(normal);
    normal = glm::normalize(normal);

    //Making sure it's facing the right way
    glm::vec2 polygonCenter = FindArithmeticMean(vertices, len);
    glm::vec2 dir = polygonCenter - circleCenter;

    if (glm::dot(dir, normal) < 0.0f)
    {
        normal = -normal;
    }

    return true;
}


bool CollisionChecker::IntersectCircles(glm::vec2 centerA, glm::vec2 centerB, float radA, float radB, float& depth, glm::vec2& normal)
{
    float dist = glm::distance(centerA, centerB);
    float radii = radA + radB;

    if (dist >= radii)
    {
        return false;
    }

    normal = glm::normalize(centerB - centerA);
    depth = radii - dist;
    return true;
}

void CollisionChecker::ResolveCollision(const RigidBody& rb1, const RigidBody& rb2, float depth, glm::vec2 normal)
{
    LilObj<Transform> t1 = rb1.transform;
    LilObj<Transform> t2 = rb2.transform;

    glm::vec2 dir = normal;
    dir *= (depth / 2.0f);

    t1->Translate(-dir);
    t2->Translate(dir);
}


bool CollisionChecker::IntersectPolygons(glm::vec2 *verticesA, glm::vec2 *verticesB, glm::vec2 &normal, float &depth)
{
    normal = glm::vec2(0);
    depth = std::numeric_limits<float>::max();

    int aLen = sizeof(verticesA) / sizeof(glm::vec2);
    int bLen = sizeof(verticesB) / sizeof(glm::vec2);

    for (int i = 0; i < aLen; i++)
    {
        glm::vec2 va = verticesA[i];
        glm::vec2 vb = verticesA[(i + 1) % aLen];
        glm::vec2 edge = vb - va;
        //Make sure this is clockwise in winding order
        glm::vec2 axis = {-edge.y, edge.x};

        float minA, minB, maxA, maxB;

        ProjectVertices(verticesA, aLen, axis, minA, maxA);
        ProjectVertices(verticesB, bLen, axis, minB, maxB);

        if (minA >= maxB || minB >= maxA)
        {
            //There be a gap
            return false;
        }

        //Find the smallest possible correction needed
        float axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < depth)
        {
            depth = axisDepth;
            normal = axis;
        }
    }

    for (int i = 0; i < bLen; i++)
    {
        glm::vec2 va = verticesB[i];
        glm::vec2 vb = verticesB[(i + 1) % aLen];
        glm::vec2 edge = vb - va;
        //Make sure this is clockwise in winding order
        glm::vec2 axis = {-edge.y, edge.x};

        float minA, minB, maxA, maxB;

        ProjectVertices(verticesA, aLen, axis, minA, maxA);
        ProjectVertices(verticesB, bLen, axis, minB, maxB);

        if (minA >= maxB || minB >= maxA)
        {
            //There be a gap
            return false;
        }

        //Find the smallest possible correction needed
        float axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < depth)
        {
            depth = axisDepth;
            normal = axis;
        }
    }

    depth /= glm::length(normal);
    normal = glm::normalize(normal);

    //Making sure it's facing the right way
    glm::vec2 centerA = FindArithmeticMean(verticesA, aLen);
    glm::vec2 centerB = FindArithmeticMean(verticesB, bLen);
    glm::vec2 dir = centerB - centerA;

    if (glm::dot(dir, normal) < 0.0f)
    {
        normal = -normal;
    }

    return true;
}

glm::vec2 CollisionChecker::FindArithmeticMean(glm::vec2 *vertices, int len)
{
    float sumX = 0, sumY = 0;

    for (int i = 0; i < len; i++)
    {
        glm::vec2 v = vertices[i];
        sumX += v.x;
        sumY += v.y;
    }

    return {sumX / (float)len, sumY / (float)len};
}

void CollisionChecker::ProjectCircle(glm::vec2 center, float radius, glm::vec2 axis, float &min, float &max)
{
    glm::vec2 dir = glm::normalize(axis);
    glm::vec2 dirAndRad = {dir.x * radius, dir.y * radius};

    glm::vec2 p1 = center + dirAndRad;
    glm::vec2 p2 = center - dirAndRad;

    min = glm::dot(p1, axis);
    max = glm::dot(p2, axis);

    if (min > max)
    {
        //Swapsies
        float f = min;
        min = max;
        max = f;
    }
}

void CollisionChecker::ProjectVertices(glm::vec2 *vertices, int len, glm::vec2 axis, float &min, float &max)
{
    min = std::numeric_limits<float>::max();
    max = std::numeric_limits<float>::min();

    for (int i = 0; i < len; i++)
    {
        glm::vec2 v = vertices[i];
        float proj = glm::dot(v, axis);

        if (proj < min) {min = proj;}
        if (proj > max) {max = proj;}
    }
}

int CollisionChecker::FindClosestPointOnPolygon(glm::vec2 circleCenter, glm::vec2 *vertices, int len)
{
    int result = -1;
    float minDist = std::numeric_limits<float>::max();

    for (int i = 0; i < len; i++)
    {
        float dist = glm::distance(vertices[i], circleCenter);

        if (dist < minDist)
        {
            minDist = dist;
        }
    }

    return result;
}




//
// Created by Somed on 2/15/2025.
//

#include "CollisionChecker.h"

//#include <glm/detail/func_geometric.inl>

#include "RigidBody.h"
#include "EngineStuffs/GameObject.h"
#include "EngineStuffs/Transform.h"
#include "EngineStuffs/Tilemaps/TileCollider.h"

bool CollisionChecker::IntersectCirclePolygon(glm::vec2 circleCenter, float circleRad, glm::vec2 polyCenter, glm::vec2 *vertices, int len, glm::vec2 &normal, float &depth)
{
    normal = glm::vec2(0);
    depth = std::numeric_limits<float>::infinity();

    glm::vec2 axis = glm::vec2(0);
    float minA, minB, maxA, maxB, axisDepth;

    for (int i = 0; i < len; i++)
    {
        glm::vec2 va = vertices[i];
        glm::vec2 vb = vertices[(i + 1) % len];
        glm::vec2 edge = vb - va;
        //Make sure this is clockwise in winding order
        axis = {-edge.y, edge.x};
        axis = glm::normalize(axis);

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
    axis = glm::normalize(axis);

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

    //Making sure it's facing the right way
    glm::vec2 dir = polyCenter - circleCenter;

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

bool CollisionChecker::IntersectPolygons(glm::vec2 *verticesA, int aLen, glm::vec2 centerA, glm::vec2 *verticesB, int bLen, glm::vec2 centerB, glm::vec2 &normal, float &depth)
{
    normal = glm::vec2(0);
    depth = std::numeric_limits<float>::infinity();

    for (int i = 0; i < aLen; i++)
    {
        glm::vec2 va = verticesA[i];
        glm::vec2 vb = verticesA[(i + 1) % aLen];
        glm::vec2 edge = vb - va;
        //Make sure this is clockwise in winding order
        glm::vec2 axis = {-edge.y, edge.x};
        axis = glm::normalize(axis);

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
        glm::vec2 vb = verticesB[(i + 1) % bLen];
        glm::vec2 edge = vb - va;
        //Make sure this is clockwise in winding order
        glm::vec2 axis = {-edge.y, edge.x};
        axis = glm::normalize(axis);

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

    glm::vec2 dir = centerB - centerA;

    if (glm::dot(dir, normal) < 0.0f)
    {
        normal = -normal;
    }

    return true;
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
    min = std::numeric_limits<float>::infinity();
    max = -std::numeric_limits<float>::infinity();

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
    float minDist = std::numeric_limits<float>::infinity();

    for (int i = 0; i < len; i++)
    {
        float dist = glm::distance(vertices[i], circleCenter);

        if (dist < minDist)
        {
            minDist = dist;
            result = i;
        }
    }

    return result;
}

void CollisionChecker::FindContactPoints(RigidBody *bodyA, RigidBody *bodyB, glm::vec2 &contact1, glm::vec2 &contact2, int &contactCount)
{
    contact1 = {};
    contact2 = {};
    contactCount = 0;

    RigidBodyShape typeA = bodyA->bodyShape;
    RigidBodyShape typeB = bodyB->bodyShape;

    int aLen = bodyA->GetNumVertices();
    int bLen = bodyB->GetNumVertices();

    if (typeA == RigidBodyShape::RB_BOX)
    {
        if (typeA == typeB)
        {
            FindContactPoint(bodyA->GetTransformedVertices(), aLen, bodyB->GetTransformedVertices(), bLen, contact1, contact2, contactCount);
        } else if (typeB == RigidBodyShape::RB_CIRCLE)
        {
            FindContactPoint(bodyB->transform->GlobalPosition(), bodyB->GetRadius(),
                bodyA->transform->GlobalPosition(), bodyA->GetTransformedVertices(), aLen, contact1);
            contactCount = 1;
        }
    } else if (typeA == RigidBodyShape::RB_CIRCLE)
    {
        if (typeA == typeB)
        {
            glm::vec2 cont = {};
            FindContactPoint(bodyA->transform->GlobalPosition(), bodyB->transform->GlobalPosition(),bodyA->Radius(), cont);
            contactCount = 1;
        } else if (typeB == RigidBodyShape::RB_BOX)
        {
            FindContactPoint(bodyA->transform->GlobalPosition(), bodyA->GetRadius(),
                bodyB->transform->GlobalPosition(), bodyB->GetTransformedVertices(), bLen, contact1);
            contactCount = 1;
        }
    }
}

void CollisionChecker::FindContactPoints(RigidBody *bodyA, const TileCollider *bodyB, glm::vec2 &contact1, glm::vec2 &contact2, int &contactCount)
{
    contact1 = {};
    contact2 = {};
    contactCount = 0;

    RigidBodyShape typeA = bodyA->bodyShape;

    int aLen = bodyA->GetNumVertices();
    glm::vec2 verts[4];
    for (int i = 0; i < 4; i++)
    {
        verts[i] = bodyB->vertices[i];
    }

    if (typeA == RigidBodyShape::RB_BOX)
    {
        FindContactPoint(bodyA->GetTransformedVertices(), aLen,
            verts, 4, contact1, contact2, contactCount);
    } else if (typeA == RigidBodyShape::RB_CIRCLE)
    {
        FindContactPoint(bodyA->transform->GlobalPosition(), bodyA->GetRadius(),
                bodyB->center, verts, 4, contact1);
        contactCount = 1;
    }
}


void CollisionChecker::PointSegmentDistance(glm::vec2 p, glm::vec2 a, glm::vec2 b, float &distanceSquared, glm::vec2 &contact)
{
    glm::vec2 ab = b - a;
    glm::vec2 ap = p - a;

    float proj = glm::dot(ap, ab);
    float abLenSq = ab.x * ab.x + ab.y * ab.y;
    float d = proj / abLenSq;

    if (d <= 0.0f)
    {
        contact = a;
    } else if (d >= 1.0f)
    {
        contact = b;
    } else
    {
        contact = a + ab * d;
    }

    float dx = p.x - contact.x;
    float dy = p.y - contact.y;
    distanceSquared = dx * dx + dy * dy;
}


//Circle circle
void CollisionChecker::FindContactPoint(glm::vec2 centerA, glm::vec2 centerB, float radA, glm::vec2 &contact)
{
    glm::vec2 ab = glm::normalize(centerB - centerA);
    contact = centerA + ab * radA;
}

//Circle polygon
void CollisionChecker::FindContactPoint(glm::vec2 centerA, float radA, glm::vec2 polyCenter, glm::vec2 *vertices, int len, glm::vec2 &contact)
{
    float minDistSq = std::numeric_limits<float>::infinity();

    for (int i = 0; i < len; i++)
    {
        glm::vec2 vA = vertices[i];
        glm::vec2 vB = vertices[(i + 1) % len];

        float distanceSquared;
        glm::vec2 testContact;
        PointSegmentDistance(centerA, vA, vB, distanceSquared, testContact);

        if (distanceSquared < minDistSq)
        {
            minDistSq = distanceSquared;
            contact = testContact;
        }
    }
}

//Polygon polygon
void CollisionChecker::FindContactPoint(glm::vec2 *verticesA, int aLen, glm::vec2 *verticesB, int bLen, glm::vec2 &contact1, glm::vec2 &contact2, int &contactCount)
{
    float minDistSq = std::numeric_limits<float>::infinity();

    //Check for one
    for (int i = 0; i < aLen; i++)
    {
        glm::vec2 p = verticesA[i];

        for (int j = 0; j < bLen; j++)
        {
            glm::vec2 va = verticesB[j];
            glm::vec2 vb = verticesB[(j + 1) % bLen];

            float distanceSquared;
            glm::vec2 testContact;
            PointSegmentDistance(p, va, vb, distanceSquared, testContact);

            if (GetNearlyEqual(distanceSquared, minDistSq))
            {
                if (!GetNearlyEqual(testContact, contact1))
                {
                    contact2 = testContact;
                    contactCount = 2;
                }
            } else if (distanceSquared < minDistSq)
            {
                minDistSq = distanceSquared;
                contactCount = 1;
                contact1 = testContact;
            }
        }
    }

    //Check for other.
    for (int i = 0; i < bLen; i++)
    {
        glm::vec2 p = verticesB[i];

        for (int j = 0; j < aLen; j++)
        {
            glm::vec2 va = verticesA[j];
            glm::vec2 vb = verticesA[(j + 1) % aLen];

            float distanceSquared;
            glm::vec2 testContact;
            PointSegmentDistance(p, va, vb, distanceSquared, testContact);

            if (GetNearlyEqual(distanceSquared, minDistSq))
            {
                if (!GetNearlyEqual(testContact, contact1))
                {
                    contact2 = testContact;
                    contactCount = 2;
                }
            } else if (distanceSquared < minDistSq)
            {
                minDistSq = distanceSquared;
                contactCount = 1;
                contact1 = testContact;
            }
        }
    }
}

bool CollisionChecker::CollideCheck(RigidBody bodyA, TileCollider bodyB, glm::vec2 &normal, float &depth)
{
    normal = glm::vec2(0);
    depth = 0;

    RigidBodyShape typeA = bodyA.bodyShape;

    if (typeA == RigidBodyShape::RB_BOX)
    {
        return CollisionChecker::IntersectPolygons
            (bodyA.GetTransformedVertices(), bodyA.GetNumVertices(), bodyA.transform->GlobalPosition(),
                bodyB.vertices, 4, bodyB.center, normal, depth);
    } else if (typeA == RigidBodyShape::RB_CIRCLE)
    {
        bool result = CollisionChecker::IntersectCirclePolygon
            (bodyA.transform->GlobalPosition(), bodyA.GetRadius(),
                bodyB.center, bodyB.vertices, 4, normal, depth);

        normal = -normal;
        return result;
    }

    return false;
}


bool CollisionChecker::CollideCheck(RigidBody bodyA, RigidBody bodyB, glm::vec2 &normal, float &depth)
{
    normal = glm::vec2(0);
    depth = 0;

    RigidBodyShape typeA = bodyA.bodyShape;
    RigidBodyShape typeB = bodyB.bodyShape;

    if (typeA == RigidBodyShape::RB_BOX)
    {
        if (typeA == typeB)
        {
            return CollisionChecker::IntersectPolygons
            (bodyA.GetTransformedVertices(), bodyA.GetNumVertices(), bodyA.transform->GlobalPosition(),
                bodyB.GetTransformedVertices(), bodyB.GetNumVertices(), bodyB.transform->GlobalPosition(), normal, depth);
        } else if (typeB == RigidBodyShape::RB_CIRCLE)
        {
            bool result = CollisionChecker::IntersectCirclePolygon
            (bodyB.transform->GlobalPosition(), bodyB.GetRadius(),
                bodyA.transform->GlobalPosition(), bodyA.GetTransformedVertices(), bodyA.GetNumVertices(), normal, depth);
            normal = -normal;
            return result;
        }
    } else if (typeA == RigidBodyShape::RB_CIRCLE)
    {
        if (typeA == typeB)
        {
            CollisionChecker::IntersectCircles
            (bodyA.transform->GlobalPosition(), bodyB.transform->GlobalPosition(),bodyA.GetRadius(), bodyB.GetRadius(), depth, normal);
        } else if (typeB == RigidBodyShape::RB_BOX)
        {
            bool result = CollisionChecker::IntersectCirclePolygon
            (bodyA.transform->GlobalPosition(), bodyA.GetRadius(),
                bodyB.transform->GlobalPosition(), bodyB.GetTransformedVertices(), bodyB.GetNumVertices(), normal, depth);

            normal = -normal;
            return result;
        }
    }

    return false;
}

bool CollisionChecker::IntersectAABBs(AABB a, AABB b)
{
    if (a.max.x <= b.min.x || b.max.x <= a.min.x
        || a.max.y <= b.min.y || b.max.y <= a.min.y)
    {
        return false;
    }

    return true;
}

bool CollisionChecker::GetNearlyEqual(glm::vec2 a, glm::vec2 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float sqrDist = std::sqrt(dx * dx + dy * dy);
    //return FlatMath.DistanceSquared(a, b) < FlatMath.VerySmallAmount * FlatMath.VerySmallAmount;
    return sqrDist < NearlyEqual * NearlyEqual;
}

bool CollisionChecker::GetNearlyEqual(float a, float b)
{
    return std::abs(a - b) < NearlyEqual;
}

bool CollisionChecker::RayCastCheck(RigidBody body, glm::vec2 *lineData)
{
    RigidBodyShape shape = body.bodyShape;
    glm::vec2 fakeNormal;
    float fakeDepth;

    if (shape == RigidBodyShape::RB_BOX)
    {
        return CollisionChecker::IntersectPolygons
            (body.GetTransformedVertices(), body.GetNumVertices(), body.transform->GlobalPosition(),
                lineData, 2, glm::vec2(0), fakeNormal, fakeDepth);
    } else if (shape == RigidBodyShape::RB_CIRCLE)
    {
        bool result = CollisionChecker::IntersectCirclePolygon
            (body.transform->GlobalPosition(), body.GetRadius(),
                glm::vec2(0), lineData, 2, fakeNormal, fakeDepth);
        return result;
    }

    return false;
}

bool CollisionChecker::RayCastCheck(TileCollider body, glm::vec2 *lineData)
{
    glm::vec2 fakeNormal;
    float fakeDepth;

    return CollisionChecker::IntersectPolygons
            (lineData, 2, glm::vec2(0),
                body.vertices, 4, body.center, fakeNormal, fakeDepth);
}

void CollisionChecker::GetRayContacts(const TileCollider *body, glm::vec2 *lineData, glm::vec2 &contact1, glm::vec2 &contact2, int &contactCount)
{
    FindContactPoint(*body->vertices, 4, lineData, contact1, contact2, contactCount);
}

void CollisionChecker::GetRayContacts(RigidBody *body, glm::vec2 *lineData, glm::vec2 &contact1, glm::vec2 &contact2, int &contactCount)
{
    if (body->bodyShape == RigidBodyShape::RB_BOX)
    {
        FindContactPoint(body->GetTransformedVertices(), body->GetNumVertices(), lineData, contact1, contact2, contactCount);
    } else if (body->bodyShape == RigidBodyShape::RB_CIRCLE)
    {
        FindContactPoint(body->transform->GlobalPosition(), body->GetRadius(), lineData, contact1, contact2, contactCount);
    }
}

//Get contacts against circle
void CollisionChecker::FindContactPoint(glm::vec2 center, float rad, glm::vec2 *lineData, glm::vec2 &contact1, glm::vec2 &contact2, int &contactCount)
{
    float minDistSq = std::numeric_limits<float>::infinity();

    for (int i = 0; i < 2; i++)
    {
        glm::vec2 vA = lineData[i];
        glm::vec2 vB = lineData[(i + 1) % 2];

        float distanceSquared;
        glm::vec2 testContact;
        PointSegmentDistance(center, vA, vB, distanceSquared, testContact);

        if (distanceSquared < minDistSq)
        {
            minDistSq = distanceSquared;
            contact1 = testContact;
        }
    }
}

//Get contacts against polys
void CollisionChecker::FindContactPoint(glm::vec2 *vertices, int numVerts, glm::vec2 *lineData, glm::vec2 &contact1, glm::vec2 &contact2, int &contactCount)
{
    float minDistSq = std::numeric_limits<float>::infinity();

    //Check for one
    for (int i = 0; i < numVerts; i++)
    {
        glm::vec2 p = vertices[i];

        for (int j = 0; j < 2; j++)
        {
            glm::vec2 va = lineData[j];
            glm::vec2 vb = lineData[(j + 1) % 2];

            float distanceSquared;
            glm::vec2 testContact;
            PointSegmentDistance(p, va, vb, distanceSquared, testContact);

            if (GetNearlyEqual(distanceSquared, minDistSq))
            {
                if (!GetNearlyEqual(testContact, contact1))
                {
                    contact2 = testContact;
                    contactCount = 2;
                }
            } else if (distanceSquared < minDistSq)
            {
                minDistSq = distanceSquared;
                contactCount = 1;
                contact1 = testContact;
            }
        }
    }

    //Check for other.
    for (int i = 0; i < 2; i++)
    {
        glm::vec2 p = lineData[i];

        for (int j = 0; j < numVerts; j++)
        {
            glm::vec2 va = vertices[j];
            glm::vec2 vb = vertices[(j + 1) % numVerts];

            float distanceSquared;
            glm::vec2 testContact;
            PointSegmentDistance(p, va, vb, distanceSquared, testContact);

            if (GetNearlyEqual(distanceSquared, minDistSq))
            {
                if (!GetNearlyEqual(testContact, contact1))
                {
                    contact2 = testContact;
                    contactCount = 2;
                }
            } else if (distanceSquared < minDistSq)
            {
                minDistSq = distanceSquared;
                contactCount = 1;
                contact1 = testContact;
            }
        }
    }
}









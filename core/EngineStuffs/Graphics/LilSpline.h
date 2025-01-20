//
// Created by Somed on 1/18/2025.
//

#ifndef LILSPLINE_H
#define LILSPLINE_H
#include <glm/vec2.hpp>

struct SplineSegment
{
    glm::vec2 a;
    glm::vec2 b;
    glm::vec2 c;
    glm::vec2 d;
};

struct SplineInfo
{
    SplineInfo(glm::vec2 pos, glm::vec2 dir)
    {
        position = pos;
        direction = dir;
    }
    glm::vec2 position;
    glm::vec2 direction;
};

//A Catmull-Rom spline with arc length parameterization
class LilSpline {
public:
    LilSpline() = default;
    LilSpline(int numPoints);
    ~LilSpline() = default;

    float getTension() const {return tension;}
    void setTension(float tens);

    bool getInitted() const {return isInitted;}

    void drawSpline() {};
    SplineInfo getPosition(float t);
    void setPoint(int pointNum, glm::vec2 point);
    void initPoints(std::vector<glm::vec2> points);
private:
    std::vector<glm::vec2> mPoints;
    glm::vec2 secretPoints[2];
    int pointCount;
    bool isClosed = false;
    bool isInitted = false;
    float tension = 0;
};



#endif //LILSPLINE_H

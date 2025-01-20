//
// Created by Somed on 1/18/2025.
//

#include "LilSpline.h"
#include <glm/detail/func_geometric.inl>

LilSpline::LilSpline(int numPoints)
{
    int pts = numPoints;
    if (numPoints < 2)
    {
        pts = 2;
    }

    pointCount = pts;
    mPoints.reserve(pts);
}

void LilSpline::initPoints(std::vector<glm::vec2> points)
{
    if (points.size() != pointCount)
    {
        std::cout << "Invalid number of points, please try again" << '\n';
        return;
    }
    for (int i = 0; i < points.size(); i++)
    {
        mPoints.emplace_back(points[i]);
    }

    if (mPoints.front() == points.back() && pointCount > 3)
    {
        isClosed = true;
    }

    if (!isClosed)
    {
        secretPoints[0] = mPoints[0] - mPoints[1];
        secretPoints[1] = mPoints[pointCount - 1] - mPoints[pointCount - 2];
    }

    isInitted = true;
}


void LilSpline::setPoint(int pointNum, glm::vec2 point)
{
    if (!isInitted || pointNum >= pointCount || pointNum < 0)
    {
        return;
    }

    if (pointCount < 4)
    {
        if (pointNum == 0 && point == mPoints.back())
        {
            return;
        }
        if (pointNum == mPoints.size() - 1 && point == mPoints.front())
        {
            return;
        }
    }


    mPoints[pointNum] = point;

    //Closes loop if identicals appear
    if (pointNum == pointCount - 1)
    {
        if (point == mPoints.front())
        {
            isClosed = true;
        } else
        {
            isClosed = false;
        }
    }
    if (pointNum == 0)
    {
        if (point == mPoints.back())
        {
            isClosed = true;
        } else
        {
            isClosed = false;
        }
    }

    if (pointNum < 2 || !isClosed)
    {
        secretPoints[0] = mPoints[0] - mPoints[1];
    }
    if (pointNum >= pointCount - 2 && !isClosed)
    {
        secretPoints[1] = mPoints[pointCount - 1] - mPoints[pointCount - 2];
    }
}

void LilSpline::setTension(float tens)
{
    if (tens > 1)
    {
        tension = 1;
    } else if (tens < 0)
    {
        tension = 0;
    } else
    {
        tension = tens;
    }
}

SplineInfo LilSpline::getPosition(float t)
{
    if (!isInitted)
    {
        return {{},{}};
    }

    int segment = std::floor(t);

    float trueT = t - segment;

    if (t < 0)
    {
        return {{},{}};
    } else if (t >= pointCount - 1)
    {
        if (isClosed)
        {
            return {mPoints.front(), glm::normalize(mPoints[1] - mPoints[0])};
        } else
        {
            return {mPoints.back(), glm::normalize(secretPoints[1] - mPoints[pointCount - 1])};
        }
    }
    else if (t == 0)
    {
        return {mPoints[0], glm::normalize(mPoints[1] - mPoints[0])};
    }

    glm::vec2 segPoints[4];
    //First Segment
    if (segment == 0)
    {
        segPoints[0] = secretPoints[0];
        segPoints[1] = mPoints[0];
        segPoints[2] = mPoints[1];

        //Special case for 2 point linear spline
        if (segment == pointCount - 2)
        {
            segPoints[3] = secretPoints[1];
        } else
        {
            segPoints[3] = mPoints[2];
        }

        if (isClosed)
        {
            segPoints[0] = mPoints[pointCount - 2];
        }
        //Is last
    } else if (segment == pointCount - 2)
    {
        segPoints[0] = mPoints[segment - 1];
        segPoints[1] = mPoints[segment];
        segPoints[2] = mPoints[segment + 1];
        segPoints[3] = secretPoints[1];
        if (isClosed)
        {
            segPoints[3] = mPoints[1];
        }
    } else
    {
        //Everything else
        segPoints[0] = mPoints[segment - 1];
        segPoints[1] = mPoints[segment];
        segPoints[2] = mPoints[segment + 1];
        segPoints[3] = mPoints[segment + 2];
    }

    const float ALPHA = 0.5;

    //Calculation time
    //Borrowed from https://qroph.github.io/2018/07/30/smooth-paths-using-catmull-rom-splines.html
    //Perhaps swap out with a matrix later for efficiency
    float t01 = pow(glm::distance(segPoints[0], segPoints[1]), ALPHA);
    float t12 = pow(glm::distance(segPoints[1], segPoints[2]), ALPHA);
    float t23 = pow(glm::distance(segPoints[2], segPoints[3]), ALPHA);

    glm::vec2 m1 = (1.0f - tension) *
        (segPoints[2] - segPoints[1] + t12 * ((segPoints[1] - segPoints[0]) / t01 - (segPoints[2] - segPoints[0]) / (t01 + t12)));
    glm::vec2 m2 = (1.0f - tension) *
        (segPoints[2] - segPoints[1] + t12 * ((segPoints[3] - segPoints[2]) / t23 - (segPoints[3] - segPoints[1]) / (t12 + t23)));

    SplineSegment splSegment;
    splSegment.a = 2.0f * (segPoints[1] - segPoints[2]) + m1 + m2;
    splSegment.b = -3.0f * (segPoints[1] - segPoints[2]) - m1 - m1 - m2;
    splSegment.c = m1;
    splSegment.d = segPoints[1];

    glm::vec2 point = splSegment.a * trueT * trueT * trueT +
             splSegment.b * trueT * trueT +
             splSegment.c * trueT +
             splSegment.d;

    glm::vec2 dir = splSegment.a * trueT * trueT * 3.0f +
             splSegment.b * trueT * 2.0f +
             splSegment.c;

    return {point,glm::normalize(dir)};
}
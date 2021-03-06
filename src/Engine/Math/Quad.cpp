#include "Bang/Quad.h"

#include "Bang/Assert.h"
#include "Bang/Matrix4.tcc"
#include "Bang/Plane.h"
#include "Bang/Polygon.h"
#include "Bang/Triangle.h"

using namespace Bang;

Quad::Quad(const Vector3 &p0,
           const Vector3 &p1,
           const Vector3 &p2,
           const Vector3 &p3)
{
    SetPoint(0, p0);
    SetPoint(1, p1);
    SetPoint(2, p2);
    SetPoint(3, p3);
}

void Quad::SetPoint(int i, const Vector3 &p)
{
    (*this)[i] = p;
}

Vector3 Quad::GetNormal() const
{
    return Triangle(GetPoint(0), GetPoint(1), GetPoint(2)).GetNormal();
}

Plane Quad::GetPlane() const
{
    return Plane(GetPoint(0), GetNormal());
}

const Vector3 &Quad::GetPoint(int i) const
{
    return (*this)[i];
}

const std::array<Vector3, 4> &Quad::GetPoints() const
{
    return m_points;
}

Polygon Quad::ToPolygon() const
{
    Polygon poly;
    poly.AddPoint(GetPoint(0));
    poly.AddPoint(GetPoint(1));
    poly.AddPoint(GetPoint(2));
    poly.AddPoint(GetPoint(3));
    return poly;
}

void Quad::GetTriangles(Triangle *t0, Triangle *t1) const
{
    *t0 = Triangle(GetPoint(0), GetPoint(1), GetPoint(2));
    *t1 = Triangle(GetPoint(0), GetPoint(2), GetPoint(3));

    /*
    int indexToFurthestToQuadPoint0 = 1;
    if ( Vector3::SqDistance(GetPoint(0), GetPoint(2)) >
         Vector3::SqDistance(GetPoint(0), GetPoint(indexToFurthestToQuadPoint0))
    )
    {
        indexToFurthestToQuadPoint0 = 2;
    }

    if ( Vector3::SqDistance(GetPoint(0), GetPoint(3)) >
         Vector3::SqDistance(GetPoint(0), GetPoint(indexToFurthestToQuadPoint0))
    )
    {
        indexToFurthestToQuadPoint0 = 3;
    }

    switch (indexToFurthestToQuadPoint0)
    {
        case 0: ASSERT(false); return;
        case 1:
            *t0 = Triangle(GetPoint(0), GetPoint(2), GetPoint(3));
            *t1 = Triangle(GetPoint(1), GetPoint(2), GetPoint(3));
            return;
        case 2:
            *t0 = Triangle(GetPoint(0), GetPoint(1), GetPoint(3));
            *t1 = Triangle(GetPoint(2), GetPoint(1), GetPoint(3));
            return;
        case 3:
            *t0 = Triangle(GetPoint(0), GetPoint(1), GetPoint(2));
            *t1 = Triangle(GetPoint(3), GetPoint(1), GetPoint(2));
            return;
    }
    */
}

Vector3 &Quad::operator[](std::size_t i)
{
    ASSERT(i >= 0 && i <= 3);
    return m_points[i];
}

const Vector3 &Quad::operator[](std::size_t i) const
{
    ASSERT(i >= 0 && i <= 3);
    return m_points[i];
}

namespace Bang
{
Quad operator*(const Matrix4 &m, const Quad &q)
{
    return Quad(m.TransformedPoint(q[0]),
                m.TransformedPoint(q[1]),
                m.TransformedPoint(q[2]),
                m.TransformedPoint(q[3]));
}
}

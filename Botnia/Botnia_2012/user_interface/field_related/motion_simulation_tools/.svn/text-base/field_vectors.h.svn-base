/*========================================================================
    Vector.h : Simple vector class for 2D and 3D vectors
  ------------------------------------------------------------------------
    Copyright (C) 1999-2002  James R. Bruce
    School of Computer Science, Carnegie Mellon University
  ------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ========================================================================*/
#ifndef FIELD_VECTORS_H
#define FIELD_VECTORS_H

#include <math.h>
#include "field_util.h"

#define V3COMP(p) p.x,p.y,p.z
#define V2COMP(p) p.x,p.y

namespace MyVector
{

//#define point3d Vector3d
//#define point2d Vector2d

/*
inline int sqrt(int n){
  return((int)sqrt((double)n));
}
*/

//=====================================================================//
//  Vector3D Class
//=====================================================================//

#define EPSILON (1.0E-10)

template <class num>
class Vector3d
{
public:
        num x,y,z;

        Vector3d()
        {}
        Vector3d(num nx,num ny,num nz)
        {
                x=nx;
                y=ny;
                z=nz;
        }

        void set(num nx,num ny,num nz)
        {
                x=nx;
                y=ny;
                z=nz;
        }
        void set(Vector3d<num> p)
        {
                x=p.x;
                y=p.y;
                z=p.z;
        }

        Vector3d<num> &operator=(const Vector3d<num> p)
        {
                set(p);
                return(*this);
        }

        num length() const;
        num sqlength() const;
        Vector3d<num> norm() const;
        void normalize();

        num dot(const Vector3d<num> p) const;
        Vector3d<num> cross(const Vector3d<num> p) const;

        Vector3d<num> &operator+=(const Vector3d<num> p);
        Vector3d<num> &operator-=(const Vector3d<num> p);
        Vector3d<num> &operator*=(const Vector3d<num> p);
        Vector3d<num> &operator/=(const Vector3d<num> p);

        Vector3d<num> operator+(const Vector3d<num> p) const;
        Vector3d<num> operator-(const Vector3d<num> p) const;
        Vector3d<num> operator*(const Vector3d<num> p) const;
        Vector3d<num> operator/(const Vector3d<num> p) const;

        Vector3d<num> operator*(num f) const;
        Vector3d<num> operator/(num f) const;
        Vector3d<num> &operator*=(num f);
        Vector3d<num> &operator/=(num f);

        Vector3d<num> operator-() const;

        bool operator==(const Vector3d<num> p) const;
        bool operator!=(const Vector3d<num> p) const;
        bool operator< (const Vector3d<num> p) const;
        bool operator> (const Vector3d<num> p) const;
        bool operator<=(const Vector3d<num> p) const;
        bool operator>=(const Vector3d<num> p) const;

        Vector3d<num> rotate_x(const double a) const;
        Vector3d<num> rotate_y(const double a) const;
        Vector3d<num> rotate_z(const double a) const;
};

template <class num>
num Vector3d<num>::length() const
{
        return(sqrt(x*x + y*y + z*z));
}

template <class num>
num Vector3d<num>::sqlength() const
{
        return(x*x + y*y + z*z);
}

template <class num>
Vector3d<num> Vector3d<num>::norm() const
{
        Vector3d<num> p;
        num l;
        l = sqrt(x*x + y*y + z*z);
        p.x = x / l;
        p.y = y / l;
        p.z = z / l;
        return(p);
}

template <class num>
void Vector3d<num>::normalize()
{
        num l;
        l = sqrt(x*x + y*y + z*z);
        x /= l;
        y /= l;
        z /= l;
}

template <class num>
num Vector3d<num>::dot(const Vector3d<num> p) const
{
        return(x*p.x + y*p.y + z*p.z);
}

template <class num>
num dot(const Vector3d<num> a,const Vector3d<num> b)
{
        return(a.x*b.x + a.y*b.y + a.z*b.z);
}

template <class num>
Vector3d<num> Vector3d<num>::cross(const Vector3d<num> p) const
{
        Vector3d<num> r;
        // right handed
        r.x = y*p.z - z*p.y;
        r.y = z*p.x - x*p.z;
        r.z = x*p.y - y*p.x;
        return(r);
}

template <class num>
Vector3d<num> cross(const Vector3d<num> a,const Vector3d<num> b)
{
        Vector3d<num> r;
        // right handed
        r.x = a.y*b.z - a.z*b.y;
        r.y = a.z*b.x - a.x*b.z;
        r.z = a.x*b.y - a.y*b.x;
        return(r);
}

#define VECTOR3D_EQUAL_BINARY_OPERATOR(opr) \
  template <class num> \
  Vector3d<num> &Vector3d<num>::operator opr (const Vector3d<num> p) \
  {                  \
    x = x opr p.x;   \
    y = y opr p.y;   \
    z = z opr p.z;   \
    return(*this);   \
  }

VECTOR3D_EQUAL_BINARY_OPERATOR(+=)
VECTOR3D_EQUAL_BINARY_OPERATOR(-=)
VECTOR3D_EQUAL_BINARY_OPERATOR(*=)
VECTOR3D_EQUAL_BINARY_OPERATOR(/=)

#define VECTOR3D_BINARY_OPERATOR(opr) \
  template <class num> \
  Vector3d<num> Vector3d<num>::operator opr (const Vector3d<num> p) const \
  {                  \
    Vector3d<num> r; \
    r.x = x opr p.x; \
    r.y = y opr p.y; \
    r.z = z opr p.z; \
    return(r);       \
  }

VECTOR3D_BINARY_OPERATOR(+)
VECTOR3D_BINARY_OPERATOR(-)
VECTOR3D_BINARY_OPERATOR(*)
VECTOR3D_BINARY_OPERATOR(/)

#define VECTOR3D_SCALAR_OPERATOR(opr) \
  template <class num> \
  Vector3d<num> Vector3d<num>::operator opr (const num f) const \
  {                  \
    Vector3d<num> r; \
    r.x = x opr f;   \
    r.y = y opr f;   \
    r.z = z opr f;   \
    return(r);       \
  }

VECTOR3D_SCALAR_OPERATOR(*)
VECTOR3D_SCALAR_OPERATOR(/)

#define VECTOR3D_EQUAL_SCALAR_OPERATOR(opr) \
  template <class num> \
  Vector3d<num> &Vector3d<num>::operator opr (num f) \
  {                \
    x = x opr f;   \
    y = y opr f;   \
    z = z opr f;   \
    return(*this); \
  }

VECTOR3D_EQUAL_SCALAR_OPERATOR(*=)
VECTOR3D_EQUAL_SCALAR_OPERATOR(/=)

#define VECTOR3D_LOGIC_OPERATOR(opr,combine) \
  template <class num> \
  bool Vector3d<num>::operator opr (const Vector3d<num> p) const \
  {                            \
    return((x opr p.x) combine \
           (y opr p.y) combine \
           (z opr p.z));       \
  }

VECTOR3D_LOGIC_OPERATOR(==,&&)
VECTOR3D_LOGIC_OPERATOR(!=,||)

VECTOR3D_LOGIC_OPERATOR(< ,&&)
VECTOR3D_LOGIC_OPERATOR(> ,&&)
VECTOR3D_LOGIC_OPERATOR(<=,&&)
VECTOR3D_LOGIC_OPERATOR(>=,&&)

template <class num>
Vector3d<num> Vector3d<num>::operator-() const
{
        Vector3d<num> r;
        r.x = -x;
        r.y = -y;
        r.z = -z;
        return(r);
}

template <class num>
inline Vector3d<num> abs(Vector3d<num> a)
{
        a.x = ::fabs(a.x);
        a.y = ::fabs(a.y);
        a.z = ::fabs(a.z);
        return(a);
}

template <class num>
inline Vector3d<num> max1(Vector3d<num> a,Vector3d<num> b)
{
        Vector3d<num> v;
        v.x = ::max(a.x,b.x);
        v.y = ::max(a.y,b.y);
        v.z = ::max(a.z,b.z);
        return(v);
}

template <class num>
inline Vector3d<num> bound1(Vector3d<num> v,num low,num high)
{
        v.x = ::bound(v.x,low,high);
        v.y = ::bound(v.y,low,high);
        v.z = ::bound(v.z,low,high);
        return(v);
}

// returns point rotated around X axis by <a> radians (right handed)
template <class num>
Vector3d<num> Vector3d<num>::rotate_x(const double a) const
{
        Vector3d<num> q;
        double s,c;
        s = sin(a);
        c = cos(a);
        q.x = x;
        q.y = c*y + -s*z;
        q.z = s*y + c*z;
        return(q);
}

// returns point rotated around Y axis by <a> radians (right handed)
template <class num>
Vector3d<num> Vector3d<num>::rotate_y(const double a) const
{
        Vector3d<num> q;
        double s,c;
        s = sin(a);
        c = cos(a);
        q.x = c*x + s*z;
        q.y = y;
        q.z = -s*x + c*z;
        return(q);
}

// returns point rotated around Z axis by <a> radians (right handed)
template <class num>
Vector3d<num> Vector3d<num>::rotate_z(const double a) const
{
        Vector3d<num> q;
        double s,c;
        s = sin(a);
        c = cos(a);
        q.x = c*x + -s*y;
        q.y = s*x + c*y;
        q.z = z;
        return(q);
}

// returns distance between two points
template <class num>
num distance(const Vector3d<num> a,const Vector3d<num> b)
{
        num dx,dy,dz;
        dx = a.x - b.x;
        dy = a.y - b.y;
        dz = a.z - b.z;
        return(sqrt(dx*dx + dy*dy + dz*dz));
}

// returns square of distance between two points
template <class num>
num sqdistance(const Vector3d<num> a,const Vector3d<num> b)
{
        num dx,dy,dz;
        dx = a.x - b.x;
        dy = a.y - b.y;
        dz = a.z - b.z;
        return(dx*dx + dy*dy + dz*dz);
}

// returns distance from point p to line x0-x1
template <class num>
num distance_to_line(const Vector3d<num> x0,const Vector3d<num> x1,const Vector3d<num> p)
{
        Vector3d<num> x;
        num t;
        t = ((p.x - x0.x) + (p.y - x0.y) + (p.z - x0.z)) / (x1.x + x1.y + x1.z);
        x = x0 + (x1 - x0) * t;
        return(distance(x,p));
}


//=====================================================================//
//  Vector2D Class
//=====================================================================//

template <class num> class Vector2d;

template <class num>
struct vector2d_struct
{
        num x,y;

        void set(num nx, num ny)
        {
                x=nx;
                y=ny;
        }
        void set(Vector2d<num> p)
        {
                x=p.x;
                y=p.y;
        }
};

template <class num>
class Vector2d
{
public:
        num x,y;

        Vector2d()
        {}
        Vector2d(num nx,num ny)
        {
                x=nx;
                y=ny;
        }

        void set(num nx,num ny)
        {
                x=nx;
                y=ny;
        }
        void set(Vector2d<num> p)
        {
                x=p.x;
                y=p.y;
        }
        Vector2d<num> &operator=(Vector2d<num> p)
        {
                set(p);
                return(*this);
        }

        Vector2d<num> &operator=(vector2d_struct<num> p)
        {
                set(p.x, p.y);
                return(*this);
        }

        num length() const;
        num sqlength() const;
        num angle() const
        {
                return(atan2(y,x));
        }

        Vector2d<num> norm() const;
        Vector2d<num> norm(const num len) const;
        void normalize();
        Vector2d<num> bound(const num max_length) const;

        num dot(const Vector2d<num> p) const;
        num cross(const Vector2d<num> p) const;

        Vector2d<num> perp()
        {
                return(Vector2d(-y, x));
        }

        Vector2d<num> &operator+=(const Vector2d<num> p);
        Vector2d<num> &operator-=(const Vector2d<num> p);
        Vector2d<num> &operator*=(const Vector2d<num> p);
        Vector2d<num> &operator/=(const Vector2d<num> p);

        Vector2d<num> operator+(const Vector2d<num> p) const;
        Vector2d<num> operator-(const Vector2d<num> p) const;
        Vector2d<num> operator*(const Vector2d<num> p) const;
        Vector2d<num> operator/(const Vector2d<num> p) const;

        Vector2d<num> operator*(const num f) const;
        Vector2d<num> operator/(const num f) const;
        Vector2d<num> &operator*=(num f);
        Vector2d<num> &operator/=(num f);

        Vector2d<num> operator-() const;

        bool operator==(const Vector2d<num> p) const;
        bool operator!=(const Vector2d<num> p) const;
        bool operator< (const Vector2d<num> p) const;
        bool operator> (const Vector2d<num> p) const;
        bool operator<=(const Vector2d<num> p) const;
        bool operator>=(const Vector2d<num> p) const;

        Vector2d<num> rotate(const double a) const;
        Vector2d<num> perp() const;
        void heading(num angle)
          {x=cos(angle); y=sin(angle);}
};

template <class num>
num Vector2d<num>::length() const
{
        return(sqrt(x*x + y*y));
}

template <class num>
num Vector2d<num>::sqlength() const
{
        return(x*x + y*y);
}

template <class num>
Vector2d<num> Vector2d<num>::norm() const
{
        Vector2d<num> p;
        num l;
        l = sqrt(x*x + y*y);
        if (l<0.0001)
        {
                l=1;
        }
        p.x = x / l;
        p.y = y / l;
        return(p);
}

template <class num>
Vector2d<num> Vector2d<num>::norm(const num len) const
{
        Vector2d<num> p;
        num f;
        num l=sqrt(x*x + y*y);
        if (l<0.0001)
        {
                l=1;
        }
        f = len / l;
        p.x = x * f;
        p.y = y * f;
        return(p);
}

template <class num>
void Vector2d<num>::normalize()
{
        num l;
        l = sqrt(x*x + y*y);
        if (l<0.00001)
        {
                l=1;
        }
        x /= l;
        y /= l;
}

template <class num>
Vector2d<num> Vector2d<num>::bound(const num max_length) const
{
        Vector2d<num> p;
        num l,f;
        l = sqrt(x*x + y*y);
        if (max_length < l)
        {
                p.set(x,y);
        }
        else
        {
                f = max_length / l;
                p.set(f*x,f*y);
        }
        return(p);
}

template <class num>
num Vector2d<num>::dot(const Vector2d<num> p) const
{
        return(x*p.x + y*p.y);
}

template <class num>
num dot(const Vector2d<num> a,const Vector2d<num> b)
{
        return(a.x*b.x + a.y*b.y);
}

template <class num>
num cosine(const Vector2d<num> a,const Vector2d<num> b)
// equivalent to dot(a.norm(),b.norm())
{
        num l;
        l = sqrt(a.x*a.x + a.y*a.y) * sqrt(b.x*b.x + b.y*b.y);
        return((a.x*b.x + a.y*b.y) / l);
}

template <class num>
num Vector2d<num>::cross(const Vector2d<num> p) const
{
        // right handed
        return(x*p.y - p.x*y);
}

// returns point rotated by <a> radians
template <class num>
Vector2d<num> Vector2d<num>::rotate(const double a) const
{
        Vector2d<num> q;
        double s,c;
        s = sin(a);
        c = cos(a);
        q.x = c*x + -s*y;
        q.y = s*x + c*y;
        return(q);
}

template <class num>
Vector2d<num> Vector2d<num>::perp() const
{
        return Vector2d<num>(-y, x);
}


#define VECTOR2D_EQUAL_BINARY_OPERATOR(opr) \
  template <class num> \
  Vector2d<num> &Vector2d<num>::operator opr (const Vector2d<num> p) \
  {                  \
    x = x opr p.x;   \
    y = y opr p.y;   \
    return(*this);   \
  }

VECTOR2D_EQUAL_BINARY_OPERATOR(+=)
VECTOR2D_EQUAL_BINARY_OPERATOR(-=)
VECTOR2D_EQUAL_BINARY_OPERATOR(*=)
VECTOR2D_EQUAL_BINARY_OPERATOR(/=)

#define VECTOR2D_BINARY_OPERATOR(opr) \
  template <class num> \
  Vector2d<num> Vector2d<num>::operator opr (const Vector2d<num> p) const \
  {                  \
    Vector2d<num> r; \
    r.x = x opr p.x; \
    r.y = y opr p.y; \
    return(r);       \
  }

VECTOR2D_BINARY_OPERATOR(+)
VECTOR2D_BINARY_OPERATOR(-)
VECTOR2D_BINARY_OPERATOR(*)
VECTOR2D_BINARY_OPERATOR(/)

#define VECTOR2D_SCALAR_OPERATOR(opr) \
  template <class num> \
  Vector2d<num> Vector2d<num>::operator opr (const num f) const \
  {                  \
    Vector2d<num> r;  \
    r.x = x opr f;   \
    r.y = y opr f;   \
    return(r);       \
  }

VECTOR2D_SCALAR_OPERATOR(*)
VECTOR2D_SCALAR_OPERATOR(/)

#define VECTOR2D_EQUAL_SCALAR_OPERATOR(opr) \
  template <class num> \
  Vector2d<num> &Vector2d<num>::operator opr (num f) \
  {                \
    x = x opr f;   \
    y = y opr f;   \
    return(*this); \
  }

VECTOR2D_EQUAL_SCALAR_OPERATOR(*=)
VECTOR2D_EQUAL_SCALAR_OPERATOR(/=)

#define VECTOR2D_LOGIC_OPERATOR(opr,combine) \
  template <class num> \
  bool Vector2d<num>::operator opr (const Vector2d<num> p) const \
  {                            \
    return((x opr p.x) combine \
           (y opr p.y));       \
  }

VECTOR2D_LOGIC_OPERATOR(==,&&)
VECTOR2D_LOGIC_OPERATOR(!=,||)

VECTOR2D_LOGIC_OPERATOR(< ,&&)
VECTOR2D_LOGIC_OPERATOR(> ,&&)
VECTOR2D_LOGIC_OPERATOR(<=,&&)
VECTOR2D_LOGIC_OPERATOR(>=,&&)


template <class num>
Vector2d<num> Vector2d<num>::operator-() const
{
        Vector2d<num> r;
        r.x = -x;
        r.y = -y;
        return(r);
}

template <class num>
inline Vector2d<num> abs(Vector2d<num> a)
{
        a.x = ::fabs(a.x);
        a.y = ::fabs(a.y);
        return(a);
}

template <class num>
inline Vector2d<num> max1(Vector2d<num> a,Vector2d<num> b)
{
        Vector2d<num> v;
        v.x = ::max(a.x,b.x);
        v.y = ::max(a.y,b.y);
        return(v);
}

template <class num>
inline Vector2d<num> bound1(Vector2d<num> v,num low,num high)
{
        v.x = ::bound(v.x,low,high);
        v.y = ::bound(v.y,low,high);
        return(v);
}

template <class num>
num distance(const Vector2d<num> a,const Vector2d<num> b)
{
        num dx,dy;
        dx = a.x - b.x;
        dy = a.y - b.y;
        return(sqrt(dx*dx + dy*dy));
}

// returns square of distance between two points
template <class num>
num sqdistance(const Vector2d<num> a,const Vector2d<num> b)
{
        num dx,dy;
        dx = a.x - b.x;
        dy = a.y - b.y;
        return(dx*dx + dy*dy);
}

// returns distance from point p to line x0-x1
template <class num>
num distance_to_line(const Vector2d<num> x0,const Vector2d<num> x1,const Vector2d<num> p)
{
        Vector2d<num> x;
        num t;
        t = ((p.x - x0.x) + (p.y - x0.y)) / (x1.x + x1.y);
        x = x0 + (x1 - x0) * t;
        // printf("dist:(%f,%f)-(%f,%f)\n",x.x,x.y,p.x,p.y);
        return(distance(x,p));
}

// returns perpendicular offset from line x0-x1 to point p
//返回矢量p在直线x0-x1垂直方向的分量
template <class num>
num offset_to_line(const Vector2d<num> x0,const Vector2d<num> x1,const Vector2d<num> p)
{
        Vector2d<num> n;
        // get normal to line
        n = (x1 - x0).perp().norm();
        return(n.dot(p - x0));
}

// returns perpendicular offset from line x0-x1 to point p
//返回矢量p沿着直线x0-x1方向的分量
template <class num>
num offset_along_line(const Vector2d<num> x0,const Vector2d<num> x1,const Vector2d<num> p)
{
        Vector2d<num> n,v;
        // get normal to line
        n = x1 - x0;
        n.normalize();
        v = p - x0;
        return(n.dot(v));
}

// returns nearest point on segment a0-a1 to line b0-b1
template <class num>
Vector2d<num> segment_near_line(const Vector2d<num> a0,const Vector2d<num> a1,
                                const Vector2d<num> b0,const Vector2d<num> b1)
{
        Vector2d<num> v,n,p;
        double dn,t;
        v = a1-a0;
        n = (b1-b0).norm();
        n.set(-n.y,n.x);
        dn = dot(v,n);
        if (fabs(dn) < EPSILON) return(a0);
        t = -dot(a0-b0,n) / dn;
        // printf("t=%f dn=%f\n",t,dn);
        if (t < 0) t = 0;
        if (t > 1) t = 1;
        p = a0 + v*t;
        return(p);
}

//求直线(a1,a2)与直线(b1,b2)的交点
template <class num>
Vector2d<num> intersection(const Vector2d<num> a1, const Vector2d<num> a2,
                           const Vector2d<num> b1, const Vector2d<num> b2)
{
        Vector2d<num> a = a2 - a1;
        Vector2d<num> b1r = (b1 - a1).rotate(-a.angle());
        Vector2d<num> b2r = (b2 - a1).rotate(-a.angle());
        Vector2d<num> br = (b1r - b2r);
        return
            Vector2d<num>(b2r.x - b2r.y * (br.x / br.y), 0.0).rotate(a.angle()) + a1;
}

// gives counterclockwise angle from <a-b> to <c-b>
//求<a-b>到<c-b>直线的夹角
template <class num>
num vertex_angle(const Vector2d<num> a,
                 const Vector2d<num> b,
                 const Vector2d<num> c)
{
        return(angle_mod((a-b).angle() - (c-b).angle()));
}


//==== Generic functions =============================================//
// (work on 2d or 3d vectors)

// returns nearest point on line segment x0-x1 to point p
template <class vectortype>
//求直线<x0,x1>上离点p最近的点
vectortype point_on_segment(const vectortype x0,const vectortype x1,const vectortype p)
{
        vectortype sx,sp,r;
        double f,l;
        sx = x1 - x0;
        sp = p  - x0;
        f = dot(sx,sp);
        if (f <= 0.0) return(x0); // also handles x0=x1 case
        l = sx.sqlength();
        if (f >= l) return(x1);
        r = x0 + sx * (f / l);
        return(r);
}

template <class vectortype>
double closest_point_time(const vectortype x1,const vectortype v1,
                          const vectortype x2,const vectortype v2)
// returns time of closest point of approach of two points
// moving along constant velocity vectors.
{
        vectortype v  = v1 - v2;
        double sl = v.sqlength();
        double t;
        if (sl < EPSILON) return(0.0); // parallel tracks, any time is ok.
        t = -v.dot(x1 - x2) / sl;
        if (t < 0.0) return(0.0); // nearest time was in the past, now
        // is closest point from now on.
        return(t);
}

// Ported from: dist3D_Segment_to_Segment
//   from http://geometryalgorithms.com
// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose providing
// that this copyright notice is included with it.  SoftSurfer makes
// no warranty for this code, and cannot be held liable for any real
// or imagined damage resulting from its use.  Users of this code must
// verify correctness for their application.
template <class vectortype>
double distance_seg_to_seg(vectortype s1a,vectortype s1b,vectortype s2a,vectortype s2b)
// return distnace between segments s1a-s1b and s2a-s2b
{
        vectortype dp;
        vectortype u = s1b - s1a;
        vectortype v = s2b - s2a;
        vectortype w = s1a - s2a;
        double a = dot(u,u);        // always >= 0
        double b = dot(u,v);
        double c = dot(v,v);        // always >= 0
        double d = dot(u,w);
        double e = dot(v,w);
        double D = a*c - b*b;       // always >= 0
        double sc, sN, sD = D;      // sc = sN / sD, default sD = D >= 0
        double tc, tN, tD = D;      // tc = tN / tD, default tD = D >= 0
        if (false)
        {
                printf("SegDist (%f,%f)-(%f,%f) to (%f,%f)-(%f,%f)\n",
                       V2COMP(s1a),V2COMP(s1b),V2COMP(s2a),V2COMP(s2b));
        }
        // compute the line parameters of the two closest points
        if (D < EPSILON)    // the lines are almost parallel
        {
                sN = 0.0;
                tN = e;
                tD = c;
        }
        else                 // get the closest points on the infinite lines
        {
                sN = (b*e - c*d);
                tN = (a*e - b*d);
                if (sN < 0)         // sc < 0 => the s=0 edge is visible
                {
                        sN = 0.0;
                        tN = e;
                        tD = c;
                }
                else if (sN > sD)  // sc > 1 => the s=1 edge is visible
                {
                        sN = sD;
                        tN = e + b;
                        tD = c;
                }
        }
        if (tN < 0)           // tc < 0 => the t=0 edge is visible
        {
                tN = 0.0;
                // recompute sc for this edge
                if (-d < 0)
                {
                        sN = 0.0;
                }
                else if (-d > a)
                {
                        sN = sD;
                }
                else
                {
                        sN = -d;
                        sD = a;
                }
        }
        else if (tN > tD)      // tc > 1 => the t=1 edge is visible
        {
                tN = tD;
                // recompute sc for this edge
                if ((-d + b) < 0)
                {
                        sN = 0;
                }
                else if ((-d + b) > a)
                {
                        sN = sD;
                }
                else
                {
                        sN = (-d + b);
                        sD = a;
                }
        }
        // finally do the division to get sc and tc
        sc = sN / sD;
        tc = tN / tD;
        // get the difference of the two closest points
        dp = w + u*sc - v*tc; // = S1(sc) - S2(tc)
        return(dp.length()); // return the closest distance
}

} // namespace vector

#endif // FIELD_VECTORS_H

#ifndef FIELD_GEOMETRY_H
#define FIELD_GEOMETRY_H


#include "user_interface/field_related/motion_simulation_tools/field_vectors.h"


//==== Vector types ====//

typedef MyVector::Vector2d<double> MyVector2d;
typedef MyVector::Vector3d<double> MyVector3d;
typedef MyVector::Vector2d<double> point2d;
typedef MyVector::Vector3d<double> point3d;
typedef MyVector::vector2d_struct<double> MyVector2d_struct;

typedef MyVector2d vector2f;
typedef MyVector3d vector3f;
typedef vector2f point2f;
typedef vector3f point3f;

//==== Some conversion functions ====//


inline vector2f vstructtof(MyVector2d_struct &a)
{
        vector2f r((float) a.x, (float) a.y);
        return (r);
}

inline MyVector2d vstructtod(MyVector2d_struct &a)
{
        MyVector2d r(a.x, a.y);
        return (r);
}

inline void vftostruct(vector2f a, MyVector2d_struct &b)
{
        b.x = a.x;
        b.y = a.y;
}

inline void vdtostruct(MyVector2d a, MyVector2d_struct &b)
{
        b.x = a.x;
        b.y = a.y;
}

inline vector2f vdtof(MyVector2d &a)
{
        vector2f r(a.x,a.y);
        return(r);
}

inline MyVector2d vftod(vector2f &a)
{
        MyVector2d r(a.x,a.y);
        return(r);
}

inline vector3f vdtof(MyVector3d &a)
{
        vector3f r(a.x,a.y,a.z);
        return(r);
}

inline MyVector3d vftod(vector3f &a)
{
        MyVector3d r(a.x,a.y,a.z);
        return(r);
}

inline void vdtodarray(MyVector2d &a, double *array)
{
        array[0] = a.x;
        array[1] = a.y;
}

inline void vftodarray(vector2f &a, double *array)
{
        array[0] = (double) a.x;
        array[1] = (double) a.y;
}

inline void vdtofarray(MyVector2d &a, float *array)
{
        array[0] = (float) a.x;
        array[1] = (float) a.y;
}

inline void vftofarray(vector2f &a, float *array)
{
        array[0] = (float)(a.x);
        array[1] = (float)(a.y);
}

#define RAD(deg) ((deg) * (M_PI / 180.0)) /* convert radians to degrees */
#define DEG(rad) ((rad) * (180.0 / M_PI)) /* convert degrees to radians */

#ifndef HUGE_VALF
#define HUGE_VALF (1E37)
#endif

#endif // FIELD_GEOMETRY_H

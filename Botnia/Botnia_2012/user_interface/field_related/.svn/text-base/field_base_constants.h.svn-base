#ifndef FIELD_BASE_CONSTANTS_H
#define FIELD_BASE_CONSTANTS_H

#include <float.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

#ifndef M_PI_2
#define M_PI_2		(M_PI*2)
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.4142135623730950488016887
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2   0.70710678118654752440
#endif

#if     defined(WIN32)
#ifndef FOUND_C99_ISXX
#undef  isnan
#undef  isinf
#endif
#endif

#if     !defined(isnan) && !defined(HAVE_ISNAN) && !defined(HAVE_C99_ISNAN)
#define isnan(val)      (0)
#endif
#if     !defined(isinf) && !defined(HAVE_ISINF) && !defined(HAVE_C99_ISINF)
#define isinf(val)      (0)
#endif


#define N	16
#define MASK	((unsigned)(1 << (N - 1)) + (1 << (N - 1)) - 1)
#define LOW(x)	((unsigned)(x) & MASK)
#define HIGH(x)	LOW((x) >> N)
#define MUL(x, y, z)	{ long l = (long)(x) * (long)(y); \
                (z)[0] = LOW(l); (z)[1] = HIGH(l); }
#define CARRY(x, y)	((long)(x) + (long)(y) > MASK)
#define ADDEQU(x, y, z)	(z = CARRY(x, (y)), x = LOW(x + (y)))
#define X0	0x330E
#define X1	0xABCD
#define X2	0x1234
#define A0	0xE66D
#define A1	0xDEEC
#define A2	0x5
#define C	0xB

static void next(void);
static unsigned x[3] =
    {X0, X1, X2}, a[3] =
        {A0, A1, A2}, c = C;

double drand48(void);
long lrand48();

#define copysign		copysign

void TimerInit();
//double GetTimeSec();
//unsigned long GetTimeUSec();


#endif // FIELD_BASE_CONSTANT_H

#include "algorithm_base.h"
#include <stdlib.h>

double
drand48(void)
{
	static double two16m = 1.0 / (1L << N);
	next();
	return (two16m * (two16m * (two16m * x[0] + x[1]) + x[2]));
}

static void
next(void)
{
	unsigned p[2], q[2], r[2], carry0, carry1;
	MUL(a[0], x[0], p);
	ADDEQU(p[0], c, carry0);
	ADDEQU(p[1], carry0, carry1);
	MUL(a[0], x[1], q);
	ADDEQU(p[1], q[0], carry0);
	MUL(a[1], x[0], r);
	x[2] = LOW(carry0 + carry1 + CARRY(p[1], r[0]) + q[1] + r[1] +
	           a[0] * x[2] + a[1] * x[1] + a[2] * x[0]);
	x[1] = LOW(p[1] + r[0]);
	x[0] = LOW(p[0]);
}

long lrand48()
{
	unsigned long temp;
	temp = rand() << 16;
	temp |= rand() << 1;
	temp |= (rand() & 0x1);
	return temp;
}

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define ul unsigned int
#define ull unsigned long long

#define rotateleft(x, n) (((x) << (n)) ^ ((x) >> (32 - n)))
#define update(a, b, c, n) ((a) ^ (rotateleft(((b) + (c)), (n))))
#define myrand32 ((ul)(4294967296.0 * drand48()))

void initializeR(ul *x)
{
	for (int i = 0; i < 16; i++)
	{
		x[i] = myrand32;
	}
	x[0]  = 0x61707865;
	x[5]  = 0x3320646e;
	x[10] = 0x79622d32;
	x[15] = 0x6b206574;
}

void copystate(ul *x1, ul *x)
{
	for (int i = 0; i < 16; i++)
		x1[i] = x[i];
}

void qr(ul *x0, ul *x1, ul *x2, ul *x3)
{
	ul z0, z1, z2, z3;

	z1 = update(*x1, *x3, *x0, 7);
	z2 = update(*x2, *x0, z1, 9);
	z3 = update(*x3, z1, z2, 13);
	z0 = update(*x0, z2, z3, 18);

	*x0 = z0, *x1 = z1, *x2 = z2, *x3 = z3;
}

void columnround(ul *x)
{
	qr(&(x[0]),  &(x[4]),  &(x[8]),  &(x[12]));
	qr(&(x[5]),  &(x[9]),  &(x[13]), &(x[1]));
	qr(&(x[10]), &(x[14]), &(x[2]),  &(x[6]));
	qr(&(x[15]), &(x[3]),  &(x[7]),  &(x[11]));
}

void rowround(ul *x)
{
	qr(&(x[0]),  &(x[1]),  &(x[2]),  &(x[3]));
	qr(&(x[5]),  &(x[6]),  &(x[7]),  &(x[4]));
	qr(&(x[10]), &(x[11]), &(x[8]),  &(x[9]));
	qr(&(x[15]), &(x[12]), &(x[13]), &(x[14]));
}

int main()
{
	ul x[16], x1[16], pattern, sum, pt;
	int a, b, a1, b1;

	ull loop = 0;
	double cnt = 0;

	ull LOOP = pow(2, 45);

	srand48(time(NULL));

	while (loop < LOOP)
	{

		initializeR(x);
		copystate(x1, x);

		pt = 0x00000001 << 31;
		x1[7] = x[7] ^ pt;    //ID position

		columnround(x);
		columnround(x1);

		rowround(x);
		rowround(x1);

		columnround(x);
		columnround(x1);

		rowround(x);
		rowround(x1);

		  /*4-round completed */
		  

		a = 12, b = 0;        	 // first OD position
		
		a1 = 4, b1 = 7;  		// second OD position

		pattern = 0x00000001 << b;

		sum = x[a] ^ x1[a] ^ ((x[a1] ^ x1[a1]) >> b1);   // Checking the XOR of two OD positions

		if ((sum & pattern) == 0)
		{
			cnt = cnt + 1.0;
		}
		loop++;

		if (loop > 0 && loop % (1024 * 1024 * 16) == 0)
		{
			printf("#Exp: 2^17*%llu   %0.10f         Bias of XOR of (%d,%d) and  (%d,%d): %0.10f    \n", loop / (1024 * 1024 * 16), cnt / loop, a, b, a1, b1, (2 * (cnt / loop) - 1));
		}
	}
}

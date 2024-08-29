#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define ul unsigned int
#define ull unsigned long long

#define rotateleft(x, n) (((x) << (n)) ^ ((x) >> (32 - n)))
#define update(a, b, c, n) ((a) ^ (rotateleft(((b) + (c)), (n))))
#define updatexor(a, b, c, n) ((a) ^ (rotateleft(((b) ^ (c)), (n))))
#define myrand32 ((ul)(4294967296.0 * drand48()))

void initializeR(ul *x)
{
	int i;
	for (i = 0; i < 16; i++)
	{
		x[i] = myrand32;
	}
	x[0] = 0x61707865;
	x[5] = 0x3320646e;
	x[10] = 0x79622d32;
	x[15] = 0x6b206574;
}

void copystate(ul *x1, ul *x)
{
	int i;
	for (i = 0; i < 16; i++)
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
void qrxor(ul *x0, ul *x1, ul *x2, ul *x3)
{
	ul z0, z1, z2, z3;

	z1 = updatexor(*x1, *x3, *x0, 7);
	z2 = updatexor(*x2, *x0, z1, 9);
	z3 = updatexor(*x3, z1, z2, 13);
	z0 = updatexor(*x0, z2, z3, 18);

	*x0 = z0, *x1 = z1, *x2 = z2, *x3 = z3;
}
void qrspecial(ul *x0, ul *x1, ul *x2, ul *x3)
{
	ul z0, z1, z2, z3;

	z1 = updatexor(*x1, *x3, *x0, 7);
	z2 = update(*x2, *x0, z1, 9);
	z3 = update(*x3, z1, z2, 13);
	z0 = update(*x0, z2, z3, 18);

	*x0 = z0, *x1 = z1, *x2 = z2, *x3 = z3;
}

void transpose(ul *x)
{
	ul temp;
	temp = x[1];
	x[1] = x[4];
	x[4] = temp;
	temp = x[2];
	x[2] = x[8];
	x[8] = temp;
	temp = x[3];
	x[3] = x[12];
	x[12] = temp;
	temp = x[6];
	x[6] = x[9];
	x[9] = temp;
	temp = x[7];
	x[7] = x[13];
	x[13] = temp;
	temp = x[11];
	x[11] = x[14];
	x[14] = temp;
}

void rounds(ul *x)
{
	qr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
	qr(&(x[5]), &(x[9]), &(x[13]), &(x[1]));
	qr(&(x[10]), &(x[14]), &(x[2]), &(x[6]));
	qr(&(x[15]), &(x[3]), &(x[7]), &(x[11]));
	transpose(x);
}

void roundsxor(ul *x)
{
	qrxor(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
	qrxor(&(x[5]), &(x[9]), &(x[13]), &(x[1]));
	qrxor(&(x[10]), &(x[14]), &(x[2]), &(x[6]));
	qrxor(&(x[15]), &(x[3]), &(x[7]), &(x[11]));
	transpose(x);
}

void roundspecial(ul *x)
{
	qr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
	qr(&(x[5]), &(x[9]), &(x[13]), &(x[1]));
	qr(&(x[10]), &(x[14]), &(x[2]), &(x[6]));
	qrspecial(&(x[15]), &(x[3]), &(x[7]), &(x[11]));
	transpose(x);
}

void columnround(ul *x)
{
	qr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
	qr(&(x[5]), &(x[9]), &(x[13]), &(x[1]));
	qr(&(x[10]), &(x[14]), &(x[2]), &(x[6]));
	qr(&(x[15]), &(x[3]), &(x[7]), &(x[11]));
}

void rowround(ul *x)
{
	qr(&(x[0]), &(x[1]), &(x[2]), &(x[3]));
	qr(&(x[5]), &(x[6]), &(x[7]), &(x[4]));
	qr(&(x[10]), &(x[11]), &(x[8]), &(x[9]));
	qr(&(x[15]), &(x[12]), &(x[13]), &(x[14]));
}

int main()
{
	ul x[16], x1[16], pattern, sum, pt;
	int i, l, j, k, a, b, a1, b1, MISS_MATCH_1, MISS_MATCH_2;
	ull loop = 0, val = 0, val1 = 0;
	double cnt = 0;
	ull LOOP = pow(2, 45);
	srand48(time(NULL));

	while (loop < LOOP)
	{

		while (1)
		{
			initializeR(x);
			copystate(x1, x);
			pt = 0x00000001 << 31;
			x1[7] = x[7] ^ pt;

			columnround(x);
			columnround(x1);


			MISS_MATCH_1 = 0;

			for (k = 3; k < 16; k = k + 4)
			{
				pt = 0x80000000;
				for (j = 31; j >= 0; j--)
				{
					if (((x[k] ^ x1[k]) & pt) != 0)
						MISS_MATCH_1++;
					pt = pt >> 1;
				}
			}

			

			 if (MISS_MATCH_1 == 4)  		
						break;  
		}      


			
		rowround(x);
		rowround(x1);
		columnround(x);
		columnround(x1);
		rowround(x);
		rowround(x1);

		a = 12;
		a1 = 4;
		b = 0;
		b1 = 7;

		pattern = 0x00000001 << b;

		sum = x[a] ^ x1[a] ^ ((x[a1] ^ x1[a1]) >> b1);

		if ((sum & pattern) == 0)
		{
			cnt = cnt + 1.0;
		}
		loop++;

		if (loop > 0 && loop % (1024 * 1024 * 16) == 0)
		{
			printf("OD=XOR of (%d,%d) and  (%d,%d)   %llu   %0.10f\n", a, b, a1, b1, loop / (1024 * 1024 * 1024), (2 * (cnt / loop) - 1));
		}
	}
}

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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

int main()
{
	ul x[16], x1[16], x0[16], x01[16], pattern, pt;
	int i, j, k, cell, bit, l, a, b, a1, b1, a2, b2, a3, b3;

	int A[] = {0, 1, 2, 3, 4, 5, 11, 12, 13, 14, 15, 16, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 50, 51, 52, 53, 54, 55, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};
	int LL = 98;

	ull loop = 0;
	double cnt = 0;

	srand48(time(NULL));

	while (loop < 1024 * 128)
	{
		while (1)
		{
			initializeR(x);
			copystate(x1, x);
			copystate(x0, x);
			copystate(x01, x1);
			copystate(x, x0);
			copystate(x1, x01);
			pt = 0x00000001 << 31;
			x1[7] = x[7] ^ pt;

			for (i = 0; i < 1; i++)
			{
				rounds(x);
				rounds(x1);
			}

			l = 0;

			for (j = 0; j < 16; j++)
			{
				for (k = 0; k < 32; k++)
				{
					pattern = 0x00000001 << k;
					if (((x[j] ^ x1[j]) & pattern) != 0)
						l++;
				}
			}

			if (l == 4)
			{
				rounds(x);
				rounds(x1);
				l = 0;
				for (k = 0; k < 32; k++)
				{
					pattern = 0x00000001 << k;
					if (((x[12] ^ x1[12]) & pattern) != 0)
						l++;
				}
				if (l == 2)
					break;
			}
		}

		copystate(x, x0);
		copystate(x1, x01);

		for (j = 0; j < LL; j++)
		{

			cell = (A[j] / 32) + 6;
			bit = A[j] % 32;

			if (drand48() < 0.5)
			{
				x[cell] = x[cell] ^ (0x00000001 << bit);
				x1[cell] = x1[cell] ^ (0x00000001 << bit);
			}
		}

		x1[7] = x[7] ^ pt;
		for (i = 0; i < 1; i++)
		{
			rounds(x);
			rounds(x1);
		}

		l = 0;

		for (j = 0; j < 16; j++)
		{
			for (k = 0; k < 32; k++)
			{
				pattern = 0x00000001 << k;
				if (((x[j] ^ x1[j]) & pattern) != 0)
					l++;
			}
		}

		if (l == 4)
		{
			rounds(x);
			rounds(x1);

			l = 0;

			for (k = 0; k < 32; k++)
			{
				pattern = 0x00000001 << k;
				if (((x[12] ^ x1[12]) & pattern) != 0)
					l++;
			}

			if (l == 2)
			{
				cnt = cnt + 1.0;
			}
		}
		loop++;
	}

	printf(" Prob %0.10f    \n", cnt / loop);
}
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <x86intrin.h>
#include <math.h>

#define ul unsigned int
#define ull unsigned long long

#define rotateleft(x, n) (((x) << (n)) ^ ((x) >> (32 - n)))
#define rotateright(x, n) (((x) >> (n)) | ((x) << (32 - n)))
#define update(a, b, c, n) ((a) ^ (rotateleft(((b) + (c)), (n))))

#define myrand32 ((ul)(4294967296.0 * drand48()))

#define LOOP ((1024 * 1024 * 1024ULL) * 1024ULL)

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

        *x0 = z0;
        *x1 = z1, *x2 = z2, *x3 = z3;
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

        ul x[16], x1[16], pt, m0, m1, m2;
        int i, j, k, MISS_MATCH_1, MISS_MATCH_2, z0[32];
        ull loop = 0, val = 0, val1 = 0;
        srand48(time(NULL) ^ getpid());

        while (loop < LOOP)
        {

                while (1)
                {

                        x[0] = 0x61707865;
                        x[5] = 0x3320646e;
                        x[10] = 0x79622d32;

                        x[15] = 0x6b206574;

                        x[1] = myrand32;

                        x[2] = myrand32;
                        x[3] = myrand32;

                        x[4] = myrand32;
                        x[6] = myrand32;
                        x[7] = myrand32;
                        x[8] = myrand32;

                        x[9] = myrand32;

                        x[11] = myrand32;
                        x[12] = myrand32;
                        x[13] = myrand32;
                        x[14] = myrand32;

                        copystate(x1, x);

                        pt = 0x00000001;

                        x1[7] = x1[7] ^ (pt << 31);

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
                        if (MISS_MATCH_1 > 4)
                                continue;

                        rowround(x);
                        rowround(x1);

                        MISS_MATCH_2 = 0;
                        pt = 0x80000000;

                        for (j = 31; j >= 0; j--)
                        {
                                if (((x[12] ^ x1[12]) & pt) != 0)
                                        MISS_MATCH_2++;
                                pt = pt >> 1;
                        }

                        if (MISS_MATCH_1 == 4 && MISS_MATCH_2 == 2)
                                break;
                }

                columnround(x);
                columnround(x1);
                rowround(x);
                rowround(x1);

                m0 = x[0] ^ x1[0];
                m0 = m0 % 2;

                m1 = x[4] ^ x1[4];

                for (j = 0; j < 8; j++)
                {
                        z0[j] = m1 % 2;
                        m1 = m1 / 2;
                }

                m2 = x[12] ^ x1[12];
                m2 = m2 % 2;

                if (m0 == 0x1)
                {
                        val = val + 1.0;

                        if (z0[7] ^ m2 == 0x1)
                                val1++;
                }

                loop++;

                if (loop > 0 && loop % (1024 * 128) == 0)
                {

                        printf("#Exp: 2^17*%llu   %0.10f         Bias of \\Delta X^{(4)}_0[0]: %0.10f    Conditional Bias: %0.10f\n", loop / (1024 * 128), (double)val / loop, (double)(2 * val) / loop - 1.0, (double)(2 * val1) / val - 1.0);
                }
        }
}

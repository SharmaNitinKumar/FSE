#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define myrand32 ((ul)(4294967296.0 * drand48()))
#define ul unsigned int
#define ull unsigned long long
#define rotateleft(x, n) (((x) << (n)) ^ ((x) >> (32 - n)))
#define rotateright(x, n) (((x) >> (n)) ^ ((x) << (32 - n)))
#define Add(a, b, n) ((rotateleft(a + b, n)))

void initial(ul *x)
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

void initialkey(ul *k)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        k[i] = myrand32;
    }
}

void same(ul *x, ul *y)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        y[i] = x[i];
    }
}

ul *sum(ul *x, ul *y, ul *z)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        z[i] = (x[i] + y[i]) % 4294967296;
    }
    return z;
}

ul *subt(ul *x, ul *y, ul *z)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        z[i] = (x[i] - y[i]) % 4294967296;
    }
    return z;
}

ul *add(ul *x, ul *y, ul *f)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        f[i] = (x[i] ^ y[i]);
    }
    return f;
}

void fn(ul *a, ul *b, ul *c, ul *d)
{
    ul a1, b1, c1, d1;

    b1 = (*b ^ Add(*a, *d, 7));
    c1 = (*c ^ Add(b1, *a, 9));
    d1 = (*d ^ Add(c1, b1, 13));
    a1 = (*a ^ Add(c1, d1, 18));

    *a = a1;
    *b = b1;
    *c = c1;
    *d = d1;
}

void column(ul *x)
{
    fn(&x[0], &x[4], &x[8], &x[12]);
    fn(&x[5], &x[9], &x[13], &x[1]);
    fn(&x[10], &x[14], &x[2], &x[6]);
    fn(&x[15], &x[3], &x[7], &x[11]);
}

void row(ul *x)
{
    fn(&x[0], &x[1], &x[2], &x[3]);
    fn(&x[5], &x[6], &x[7], &x[4]);
    fn(&x[10], &x[11], &x[8], &x[9]);
    fn(&x[15], &x[12], &x[13], &x[14]);
}

void refn(ul *a, ul *b, ul *c, ul *d)
{
    ul a1, b1, c1, d1;

    a1 = (*a ^ Add(*c, *d, 18));
    d1 = (*d ^ Add(*c, *b, 13));
    c1 = (*c ^ Add(*b, a1, 9));
    b1 = (*b ^ Add(a1, d1, 7));

    *a = a1;
    *b = b1;
    *c = c1;
    *d = d1;
}

void recolumn(ul *x)
{
    refn((&x[0]), &(x[4]), &(x[8]), &(x[12]));
    refn((&x[5]), &(x[9]), &(x[13]), &(x[1]));
    refn((&x[10]), &(x[14]), &(x[2]), &(x[6]));
    refn((&x[15]), &(x[3]), &(x[7]), &(x[11]));
}

void rerow(ul *x)
{
    refn((&x[0]), &(x[1]), &(x[2]), &(x[3]));
    refn((&x[5]), &(x[6]), &(x[7]), &(x[4]));
    refn((&x[10]), &(x[11]), &(x[8]), &(x[9]));
    refn((&x[15]), &(x[12]), &(x[13]), &(x[14]));
}

void halffn(ul *a, ul *b, ul *c, ul *d)
{
    ul b1 = *b ^ Add(*a, *d, 7);
    ul c1 = *c ^ Add(b1, *a, 9);

    *b = b1;
    *c = c1;
}

void halfcolumn(ul *x)
{
    halffn((&x[0]), &(x[4]), &(x[8]), &(x[12]));
    halffn((&x[5]), &(x[9]), &(x[13]), &(x[1]));
    halffn((&x[10]), &(x[14]), &(x[2]), &(x[6]));
    halffn((&x[15]), &(x[3]), &(x[7]), &(x[11]));
}

void halfrow(ul *x)
{
    halffn((&x[0]), &(x[1]), &(x[2]), &(x[3]));
    halffn((&x[5]), &(x[6]), &(x[7]), &(x[4]));
    halffn((&x[10]), &(x[11]), &(x[8]), &(x[9]));
    halffn((&x[15]), &(x[12]), &(x[13]), &(x[14]));
}


int main()
{
    printf("----------------The PNB backward bias---------------\n");

    ul x[16], x1[16], y[16], y1[16], z[16], z1[16], f[16], b[16], k[8], pattern, pt, forward, backward;
    int i, j, p, diff, p1, p2;

    int A[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 36, 37, 38, 39, 40, 41, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 100, 103, 104, 105, 106, 107, 108, 109, 110, 111, 115, 116, 117, 118, 119, 120, 121, 122, 123, 128, 129, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 174, 175, 176, 177, 178, 179, 180, 181, 186, 187, 188, 192, 193, 194, 195, 199, 200, 204, 205, 206, 207, 208, 213, 218, 224, 225, 231, 232, 233, 237, 238, 239, 240, 245, 249, 250, 251, 255};

    int array1[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 36, 37, 38, 39, 40, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 103, 104, 105, 106, 107, 108, 109, 110, 115, 116, 117, 118, 119, 120, 121, 122, 128, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 159, 160, 161, 162, 163, 164, 165, 166, 167, 174, 175, 176, 177, 178, 179, 180, 186, 187, 192, 193, 194, 199, 204, 205, 206, 207, 224, 231, 232, 237, 238, 239, 249, 250};

    int array2[] = {32, 41, 75, 95, 111, 123, 129, 154, 168, 181, 188, 195, 200, 208, 225, 233, 240, 251};

    int array3[] = {100, 213, 218, 245, 255};

    int a1 = 137, a2 = 18, a3 = 5;
    srand48(time(NULL));

    for(int yy=0;yy<128;yy++){
    ul loop = 0;
    double cnt = 0;

    initial(x);

    while (loop < 1024 * 1024*256 )
    {

        x[6]= myrand32; x[7]= myrand32; x[8]= myrand32; x[9]= myrand32;

          x[0] = 0x61707865;
         x[5] = 0x3320646e;
         x[10] = 0x79622d32;
         x[15] = 0x6b206574;

        same(x, x1);
        same(x, y);

        pt = 0x1 << 31;
        x1[7] = x1[7] ^ pt;

        same(x1, y1);

        column(x);
        column(x1);
        row(x);
        row(x1);
        column(x);
        column(x1);
        row(x);
        row(x1);
        column(x);
        column(x1);
        row(x);
        row(x1);

        add(x, x1, f);

        // forward = (f[4]);

        forward = ((f[4] << 19) ^ (f[7]) ^ (f[6]) ^ (f[6] << 1));

        column(x);
        column(x1);
        row(x);
        row(x1);

        sum(x, y, z);
        sum(x1, y1, z1);

        for (p = 0; p < a1; p++)
        {
            if ((array1[p] / 32) < 4)
            {
                p1 = (array1[p] / 32) + 1;
            }
            else
            {
                p1 = (array1[p] / 32) + 7;
            }
            p2 = array1[p] % 32;

            if ((y[p1] >> p2) & 0x1)
            {
                y[p1] ^= (0x1 << p2);
                y1[p1] ^= (0x1 << p2);
            }
        }

        for (p = 0; p < a2; p++)
        {
            if ((array2[p] / 32) < 4)
            {
                p1 = (array2[p] / 32) + 1;
            }
            else
            {
                p1 = (array2[p] / 32) + 7;
            }
            p2 = array2[p] % 32;

            if (!((y[p1] >> p2) & 0x1))
            {
                y[p1] ^= (0x1 << p2);
                y1[p1] ^= (0x1 << p2);
            }
        }

        for (p = 0; p < a3; p++)
        {
            if ((array3[p] / 32) < 4)
            {
                p1 = (array3[p] / 32) + 1;
            }
            else
            {
                p1 = (array3[p] / 32) + 7;
            }
            p2 = array3[p] % 32;

            if (drand48() < 0.5)
            {
                y[p1] ^= (0x1 << p2);
                y1[p1] ^= (0x1 << p2);
            }
        }

        subt(z, y, x);
        subt(z1, y1, x1);

        rerow(x);
        rerow(x1);
        recolumn(x);
        recolumn(x1);

        add(x, x1, b);
        // backward = ((b[4]));

        backward = ((b[4] << 19) ^ (b[7]) ^ (b[6]) ^ (b[6] << 1));

        pattern = 0x1 << 26;
        if (((forward ^ backward) & pattern) == 0)
        {
            cnt = cnt + 1.0;
        }
        loop++;

        //if (loop > 0 && loop % (1024 * 1024) == 0)
        //{
          //  printf("%u   Bias %0.10f \n", loop / (1024 * 1024), 2 * (cnt / loop) - 1.0);
        //}
    }

   printf("   8-Bias %0.10f        %d \n",  2 * (cnt / loop) - 1.0, yy);
  }
}

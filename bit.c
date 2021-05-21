#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define N 32

unsigned char A[N] = {0};


unsigned rotr(unsigned x, unsigned n)
{
	return (x >> n % 32) | (x << (32 - n) % 32);
}

unsigned int bitswap(unsigned int a, int b1, int b2)
{
	unsigned int c = (a >> b1 ^ a >> b2) & 1;
	return (c << b1 | c << b2) ^ a;
}

unsigned int xor (void)
{
	static unsigned int y = 2463534242;
	y ^= rand();
	y = y ^ (y << 13);
	y = y ^ (y >> 17);
	return y = y ^ (y << 15);
}

void init_m()
{
	int i;

	for (i = 0; i < N / 2; i++)
		A[i] = 1;
}

unsigned long long int toInt(unsigned char *a)
{
	unsigned long long int i = 0, s = 1, ret = 0;
	for (i = 0; i < N; i++)
	{

		ret += s * a[i];
		s *= 2;
		ret << 1;
	}
	//ret >>= 1;

	return ret;
}


unsigned long long int p_rand()
{
	static unsigned int first = 1;
	unsigned int a=0,r;
	int i;

	// a ^= a * y
	a^=toInt(A);
	for (i = 0; i < N; i++)
	{	r=rand()%N;
		a=bitswap(a,i,r);
	}


	return a; //toInt(A);
}

int main()
{
	FILE *fp = fopen("out.dat", "wb");
	unsigned int i, j = 0, count = 0, nn = 0;
	unsigned int a, b, c, d, v = 0;
	//unsigned char a[4]={0,1,0,1};
	//unsigned long long int

	time_t t;

	srand(time(&t));
	//i=1;
	//i=bitswap(i,0,31);
	//printf("%x\n",i);
	//exit(1);

ku:

	init_m();
	//printf("\n");

	j = 0;
	count = 0;
	nn = 0;
	v = 0;
	a = p_rand();
	printf("a=%d\n", a);
	//exit(1);
	b = p_rand();

	while (1)
	{
		c ^= p_rand();
		d = p_rand();
		printf(" c=%u d=%u %d\n", c, d, j);
		if (a == c && b == d)
		{
			nn++;
			printf(" f1=%u %u count=%d\n", a, b, (j + 1) / nn);
		}
		if (nn > 100000)
			break;

		if (a % 2 == 0)
			count++;
		if (j == 0xffffff)
		{
			a = p_rand();
			printf("f2=%d\n", a);
			b = p_rand();
			nn = 0;
			v++;
			j = 0;
		}
		if (v == 3)
		{
			exit(1);
		}
		j++;
        c^=d;
    fwrite(&c,4,1,fp);
	}
    fclose(fp);
	printf("county %d %d\n", a, b);
	printf("count f1=%d %d %d\n", count, (j + 1) / nn, nn);

	return 0;
}

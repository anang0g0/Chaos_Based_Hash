#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 8
#define BIT_VERSION

static unsigned char x[N] = {1, 5, 4, 0, 6, 7, 2, 3};
static unsigned char y[N] = {6, 3, 4, 5, 1, 2, 7, 0};
static unsigned char inv_x[N];
static unsigned char x2[N];
static unsigned char x3[N];
static unsigned char x4[N];
static unsigned char x5[N];
static unsigned char x6[N];
static unsigned char tmp[N];

unsigned int xor (void)
{
	static unsigned int y = 2463534242;
	y ^= rand();
	y = y ^ (y << 13);
	y = y ^ (y >> 17);
	return y = y ^ (y << 15);
}

	unsigned long long int xor64(void)
{
	static unsigned long long int x = 88172645463325252ULL;
	x = x ^ (x << 13);
	x = x ^ (x >> 7);
	return x = x ^ (x << 17);
}

void random_permutation(unsigned char *a)
{
	int i, j, x;
	for (i = 0; i < N; i++)
	{
		a[i] = i;
	}
	for (i = 0; i < N - 2; i++)
	{
		// rand from i+1 to N-1
		j = (rand() % (N - 1 - i)) + i + 1;

		// swap a[i] and a[j]
		x = a[j];
		a[j] = a[i];
		a[i] = x;
	}
	if (a[N - 1] == N - 1)
	{
		a[N - 1] = a[N - 2];
		a[N - 2] = N - 1;
	}
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

static unsigned char A[N] = {0};

int sec() //(unsigned char *x0, unsigned char *x1)
{
	int i, j, aaa, flg, count=0, count2=0;
	unsigned char x0[N] = {0}, w[N] = {0}, x1[N];

lu:

	random_permutation(x0);
	random_permutation(x1);
	aaa = 0;
	flg = 0;
	count = 0;
	for (i = 0; i < N; i++)
		x2[x0[i]] = i;
	for (i = 0; i < N; i++)
		printf("%d,", x2[x0[i]]);
	printf("\n");
	for (i = 0; i < N; i++)
		w[i] = x0[x1[x2[i]]];
	memcpy(x6, w, sizeof(w));

	count = 0;
ke:

	aaa = 0;
	for (i = 0; i < N; i++)
	{
		if (w[i] == x5[i])
			count++;
	}
	if (count == N)
	{
		printf("affun\n");
		exit(1);
	}
	count = 0;

	memcpy(x4, w, sizeof(x1));
	memcpy(x5, w, sizeof(x1));
	while (1)
	{
		flg = 0;
		for (i = 0; i < N; i++)
		{
			x3[i] = x4[x5[i]];
			printf("%d,", x3[i]);
			if (x3[i] == i)
				flg++;
		}
		printf("\n");
		if (flg == N)
			break;
		if (flg < N)
			count++;
		memcpy(x5, x3, sizeof(x1));
	}
	printf("count1=%d\n", count);
	//exit(1);

	memcpy(x1, w, sizeof(w));
	for (i = 0; i < N; i++)
		w[i] = x0[x1[x2[i]]];

	for (i = 0; i < N; i++)
	{
		if (w[i] == x6[i])
			aaa++;
	}

	//memcpy(x1,w,sizeof(x1));
	if (aaa == N)
	{
		for (i = 0; i < N; i++)
			printf("%d,", x6[i]);
		printf("\n");
		for (i = 0; i < N; i++)
			printf("%d,", w[i]);
		printf("\ncount2=%d total=%d\n", count2,(count+1)*(count2+1));
		//memcpy(x1,w,sizeof(x1));
		//exit(1);
		return (count+1)*(count2+1);
	}
	count2++;
	memcpy(x1, w, sizeof(x1));
	goto ke;

	return -1;
}

void init_m()
{
	int i;

	memset(A, 0, sizeof(A));
	for (i = 0; i < N / 2; i++)
		A[i] = 1;
}

int Count = 0;

unsigned long long int p_rand2()
{
	/*
#ifdef BIT_VERSION

	static unsigned char a[N] = {
		1, 1, 1, 1, 0, 0, 0, 0,
		//1,1,1,1,1,1,1,1,
		//0,0,0,0,0,0,0,0,
		//0,0,0,0,0,0,0,0,
		//1,1,1,1,1,1,1,1,
		//1,1,1,1,1,1,1,1,
		//0,0,0,0,0,0,0,0,
		//0,0,0,0,0,0,0,0,
	};
	
#else
	static unsigned char a[N] = {
		1,	2,	3,	5,
		7,	11,	13,	17,
		19,	23,	29,	31,
		33,	37,	39,	41,
		143,151,157,159,
		161,167,171,175,
		212,85 ,1,0,
		0,0,0,0,
	};
#endif
*/
	//static unsigned char x[N];
	//static unsigned char y[N];
	//static unsigned char inv_x[N];
	static unsigned char first = 1;

	//static unsigned char tmp[N];
	int i, flg = 0, aaa = 0;

	
	if (first)
	{
		first = 0;
		// y = x * y * ~x
		for (i = 0; i < N; i++)
		{
			tmp[i] = x[y[inv_x[i]]];
		}
		memcpy(x6, tmp, sizeof(x6));
		
		//random_permutation(x);
		//random_permutation(y);
		/*
		for (i = 0; i < N; i++)
		{
			//	printf("%d,",x[i]);
			inv_x[x[i]] = i;
		}
		//printf("\n");
		*/
	memcpy(x6, tmp, sizeof(x));
	memcpy(x4, tmp, sizeof(x));
	memcpy(x5, tmp, sizeof(x));

	}
	

	for (i = 0; i < N; i++)
		printf("%d,", tmp[i]);
	printf("\n");
	//exit(1);
	flg = 0;

/*
	for (i = 0; i < N; i++)
	{
		x3[i] = x4[x5[i]];
		printf("%d,", x3[i]);
		if (x3[i] == i)
			flg++;
	}
	printf("\n");
	if (flg == N)
	{
		memcpy(y, tmp, sizeof(y));
		for (i = 0; i < N; i++)
			tmp[i] = x[y[inv_x[i]]];

		for (i = 0; i < N; i++)
		{
			if (tmp[i] == y[i])
				aaa++;
		}

		memcpy(y,tmp,sizeof(y));
		if (aaa == N)
		{
			for (i = 0; i < N; i++)
				printf("%d,", x6[i]);
			printf("\n");
			for (i = 0; i < N; i++)
				printf("%d,", tmp[i]);
			//printf("\ncount2=%d\n", count2);
			memcpy(x5,tmp,sizeof(x5));
			//exit(1);
		}
	}

	//if(flg<N)
	memcpy(x5, x3, sizeof(tmp));
*/
	//break;

	// a ^= a * y
	memcpy(tmp, A, sizeof(tmp)); // tmp = a;
	for (i = 0; i < N; i++)
	{
		A[i] ^= A[y[i]];
	}

	flg = 0;
	for(i=0;i<N;i++)
		tmp[i]=x[y[inv_x[i]]];

	// y = x * y * ~x
	memcpy(y, tmp, sizeof(tmp));

	return toInt(A);
}


unsigned long long int p_rand()
{
#ifdef BIT_VERSION
	static unsigned char a[N] = {
		1, 1, 1, 1, 0, 0, 0, 0,
		//1,1,1,1,1,1,1,1,
		//0,0,0,0,0,0,0,0,
		//0,0,0,0,0,0,0,0,
		//1,1,1,1,1,1,1,1,
		//1,1,1,1,1,1,1,1,
		//0,0,0,0,0,0,0,0,
		//0,0,0,0,0,0,0,0,
	};
#else
	static unsigned char a[N] = {
		1,
		2,
		3,
		5,
		7,
		11,
		13,
		17,
		19,
		23,
		29,
		31,
		33,
		37,
		39,
		41,
		143,
		151,
		157,
		159,
		161,
		167,
		171,
		175,
		212,
		85,
		1,
		0,
		0,
		0,
		0,
		0,
	};
#endif

	//static unsigned char x[N];
	//static unsigned char y[N];
	//static unsigned char inv_x[N];
	static unsigned char first = 1;

	static unsigned char tmp[N];
	int i;

	if (first)
	{
		first = 0;

		//random_permutation(x);
		//random_permutation(y);

		for (i = 0; i < N; i++)
		{
			//	printf("%d,",x[i]);
			inv_x[x[i]] = i;
		}
		//printf("\n");
	}

	//for(i=0;i<N;i++)
	//printf("%d,",tmp[i]);
	//printf("\n");

	// a ^= a * y
	//memcpy(tmp, a, sizeof(tmp)); 	// tmp = a;
	for (i = 0; i < N; i++)
	{
		A[i] ^= A[y[i]];
	}

	// y = x * y * ~x
	for (i = 0; i < N; i++)
	{
		tmp[i] = x[y[inv_x[i]]];
	}
	memcpy(y, tmp, sizeof(tmp));

	return toInt(A);
}

int main()
{
	FILE *fp = fopen("out.txt", "wb");
	unsigned int i, j = 0, count = 0, nn = 0;
	unsigned short a, b, c, d, v = 0;
	//unsigned char a[4]={0,1,0,1};
	//unsigned long long int

	time_t t;

	srand(time(&t));

	//printf("%d\n",a);

ku:
	random_permutation(x);
	random_permutation(y);
	init_m();

	for (i = 0; i < N; i++)
	{
		inv_x[x[i]] = i;
	}
	printf("\n");

	// y = x * y * ~x
	for (i = 0; i < N; i++)
	{
		tmp[i] = x[y[inv_x[i]]];
	}
	memcpy(x6, tmp, sizeof(x6));
	for (i = 0; i < N; i++)
		printf("%d,", tmp[i]);
	printf("\n");
	//exit(1);
	j = 0;
	count = 0;
	nn = 0;
	v = 0;
	a = p_rand();
	b = p_rand();
	//sec();
	//exit(1);

	while (1)
	{
		// y = x * y * ~x
		for (i = 0; i < N; i++)
		{
			tmp[i] = x[y[inv_x[i]]];
		}
		memcpy(y, tmp, sizeof(x6));

		c = p_rand();
		d = p_rand();
		printf(" c=%d d=%d %d\n", c, d, j);
		if (a == c && b == d)
		{
			nn++;
			printf(" f1=%d %d count=%d\n", a, b, (j + 1) / nn);
		}
		if (nn > 2)
			break;

		if (a % 2 == 0)
			count++;
		if (j == 0xffff)
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
			for (i = 0; i < N; i++)
				printf("%d,", x[i]);
			printf("\n");
			exit(1);
		}
		/*
		if(fp){
			fprintf(fp, "%u\n", a);
		}else{
			printf("%u\n", a);
		}
	*/
		j++;
	}
	if ((j + 1) / nn > 3800)
	{
		for (i = 0; i < N; i++)
			printf("%d,", x[i]);
		printf(" count x\n");
		for (i = 0; i < N; i++)
			printf("%d,", y[i]);
		printf(" count y\n");
	}
	else
	{
		goto ku;
	}
	for (i = 0; i < N; i++)
		printf("%d,", A[i]);
	printf(" count-A\n");
	printf("county %d %d\n", a, b);
	printf("count f1=%d %d %d\n", count, (j + 1) / nn, nn);

	return 0;
}

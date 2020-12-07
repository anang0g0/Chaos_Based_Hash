#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


#define N 256
#define NN 64
#define BYTE 1			//1=hashfile,0=dumpdata
#define NB 4			/* 128bit 固定として規格されている(データの長さ) */
#define NBb 16


typedef union
{
  unsigned long long int u[8];
  unsigned char d[64];
} arrayul;

typedef union
{
  unsigned long long int u[N];
  unsigned char d[N * 8];
} arrayull;


typedef struct a4
{
  unsigned char ar[4];
} array;

typedef struct a8
{
  unsigned char ar[8];
} array8;

typedef union
{
  unsigned int h[16];
  unsigned char c[64];
} array16;

typedef union aN
{
  unsigned int d[64];
  unsigned long long int u[32];
  unsigned char ar[NN];
  //
} arrayn;

typedef struct pub
{
  unsigned char a[NN];
  unsigned char b[NN];
} set;

arrayn c={0};


#define I8T char
#define U8C(v) (v##U)

#define U8V(v) ((unsigned char)(v) & U8C(0xFF))
#define ROTL8(v, n) \
  (U8V((v) << (n)) | ((v) >> (8 - (n))))



void rp(unsigned char* a){
  int i,j,x;
  for(i = 0; i < NN; i++){
    a[i] = i;
  }
  for(i = 0; i < NN - 2; i++){
    // rand from i+1 to N-1
    j = (rand() % (NN-1-i)) + i + 1;
    
    // swap a[i] and a[j]
    x = a[j];
    a[j] = a[i];
		a[i] = x;
  }
  if(a[NN-1] == NN-1){
    a[NN-1] = a[NN-2];
    a[NN-2] = NN - 1;
  }
}



//hash関数の鍵----------------------------

unsigned char x0[NN]={0};  
unsigned char inv_x[NN] = { 0 };
unsigned char x1[NN]={0};
//--------------------ここまで


#define str_length 128
#define password_length 256

unsigned char password[password_length + 1];


//言わずと知れたxorshift
unsigned int
xorshift (void)
{
  static unsigned int y = 2463534242;
  y = y ^ (y << 13);
  y = y ^ (y >> 17);
  return y = y ^ (y << 15);
}

unsigned long long int
xorshift64 (void)
{
  static unsigned long long int x = 88172645463325252ULL;
  x = x ^ (x << 13);
  x = x ^ (x >> 7);
  return x = x ^ (x << 17);
}

//ゴミのリサイクル
void
seed (void)
{
  /*
   * 変数宣言
   */
  char str[str_length + 1];
  time_t t;
  int i, j, k, rnd;
  char confirm[2];

  /*
   * 乱数の初期化
   */
 
  i = 0;
  while (i < 1000)
    i++;
  /*
   * 乱数生成とパスワードの生成
   */
  for (i = 0; i < str_length; i++)
    {
      for (j = 0; j < 2; j++)
	{
	  k = i * 2 + j;
	  do
	    {
	      rnd = rand ();
	      password[k] = str[i] + rnd;
	    }
	  while (!isalnum (password[k]));
	}
    }

  /*
   * NULL文字の挿入
   */
  password[password_length] = '\0';

  /*
   * パスワードの出力
   */
//    printf("生成パスワード：%s",password);

  return;

}


//ハッシュ関数本体
arrayn
chash (unsigned char b[2048])
{
  int i, j = 0;
  arrayn n;

  unsigned char salt[NN] ={ 148, 246, 52, 251, 16, 194, 72, 150, 249, 23, 90, 107, 151, 42, 154, 124, 48, 58, 30, 24, 42, 33, 38, 10, 115, 41, 164, 16, 33, 32, 252, 143, 86, 175, 8, 132, 103, 231, 95, 190, 61, 29, 215, 75, 251, 248, 72, 48, 224, 200, 147, 93, 112, 25, 227, 223, 206, 137, 51, 88, 109, 214, 17, 172};

  unsigned char z[NN], w[NN]={0};
  unsigned char v[NN] = { 0 }, f[NN] = { 0 };
  unsigned char inv_y[NN];
  FILE *fp, *op;
  int  count = 1;
  time_t t;
  int k;

  
  for (i = 0; i < NN; i++)
    {
      inv_x[x0[i]] = i;
    }

  //デバッグ中なので省略
  //for (i = 0; i < NN; i++)
    //   f[i] ^= salt[i];


  k = 0;
  for (i = 0; i < NN; i++)
    f[i] ^= b[i];



   for(i=0;i<NN;i++)
     printf("%d,",f[i]);
   printf("\n");

   //  exit(1);
  //バッファを埋める回数だけ回す
  for (j = 0; j < 2048/NN; j++)
    {
      for (i = 0; i < NN; i++)
	z[i] = x0[x1[inv_x[i]]];
      
      memcpy (x1, z, sizeof (unsigned char) * NN);

      for (i = 0; i < NN; i++)
	{
	  
	  //mode 2(自己書き換え系)
	  f[x1[i]]+=abs(ROTL8(f[(i+1)%NN],3)-ROTL8(f[i],5));	
	  
	}

      if(count < 2048 / NN)
	{			//k=1;k<2048/NN;k++){
	  for (i = 0; i < NN; i++){
	    f[i] ^= b[count * NN + i];
	    // f[i]^=c.ar[i];
	  }
	}
      
      count++;
    }

  memcpy (n.ar, f, sizeof (unsigned char) * NN);


  return n;

}


//ファイル操作
array16
hash (int argc, char *argv[])
{
  int i, j, k, n;
  array16 h = { 0 };

  unsigned char buf[2048] = { 0 };
  FILE *fp;
  arrayn a = { 0 }, b = { 0 };


  if (BYTE)
    {

      fp = fopen (argv[1], "rb");
      if (fp == NULL)
	{
	  printf ("no file\n");
	  exit (1);
	}
      while ((n = fread (buf, 1, 2048, fp)) > 0)
	{
	  //paddaing
	  if(n<2048){
	    for(i=n;i<2048;i++)
	      buf[i]=0xc6;
	  }
	  
	  //memset(h.h,0,sizeof(h.h));
	  n = 0;
	  a = chash (buf);
	  for (k = 0; k < NN / 64; k++)
	    {
#pragma omp parallel for
	      for (i = 64 * k; i < 64 * k + 64; i++)
		{
		  h.c[i - 64 * k] ^= a.ar[i];
		}
	    }
	  
	  //      fwrite(h.h,4,16,fp);
	}
  }
  

  return h;   
}

//蛇足
arrayul
crand (unsigned char u[NN])
{
  arrayn a = { 0 };
  int i, j;
  arrayul b = { 0 };

  a = chash (u);
  
  j = 0;
  memset (b.u, 0, sizeof (b.u));
  memcpy(b.d,a.ar,sizeof(unsigned char)*NN);

  return b;
}


int
main (int argc, char *argv[])
{
  int i, j, n;
  arrayul p;
  array16 t;
  unsigned char *b;


  srand (clock () + time (&t));

  rp(x0);
  rp(x1);
  for(i=0;i<NN;i++)
    inv_x[x0[i]]=i;
  

    t = hash (argc, argv);
    //慎ましくここは256ビットだけ
    for (i = 0; i < 16 / 2; i++)
	    printf ("%08x", t.h[i]);
    printf (" %s", argv[1]);
    printf ("\n");


  return 0;
}

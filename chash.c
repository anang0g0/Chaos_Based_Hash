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



//p.cで作った置換（鍵）
//unsigned char x0[NN] ={98,175,221,11,87,29,222,105,121,50,41,166,103,23,144,249,164,234,82,212,33,235,122,117,55,220,37,80,12,229,73,236,8,240,126,44,97,124,132,210,85,104,95,145,88,136,244,216,60,81,34,185,242,2,133,94,99,134,251,89,75,18,24,100,129,223,140,179,21,102,207,38,174,74,69,46,78,107,173,96,48,125,91,246,254,90,227,139,187,233,215,189,47,170,57,72,226,217,76,196,53,248,181,39,197,31,70,62,219,150,30,79,201,32,250,112,225,19,147,43,16,58,182,167,228,193,232,239,83,42,191,67,71,45,169,115,26,237,213,205,198,162,65,7,119,28,148,161,116,160,101,186,4,114,209,123,165,127,49,152,22,154,146,68,171,241,184,51,245,163,168,231,253,177,86,190,158,200,195,9,130,17,149,92,61,13,230,142,143,20,247,135,66,224,5,238,15,155,151,111,199,108,178,0,137,52,3,109,252,138,159,141,211,6,188,128,156,194,63,1,36,203,255,35,113,64,14,208,120,206,25,54,110,176,84,153,59,243,214,183,204,106,218,118,180,40,93,77,157,27,56,172,10,202,131,192};

//unsigned char x1[NN]={235,42,200,88,24,146,20,53,107,128,165,78,74,204,0,18,183,75,169,148,123,227,117,155,97,114,37,76,213,244,190,231,174,46,91,238,7,233,4,62,50,56,121,127,43,147,40,84,39,79,119,136,28,139,219,163,250,159,189,246,198,118,26,106,140,242,162,182,2,8,80,153,142,216,239,224,94,226,89,145,255,105,247,49,171,5,90,19,161,73,195,152,197,199,77,232,124,206,57,167,156,1,217,144,176,249,154,164,131,33,59,63,60,32,225,65,36,86,120,98,188,191,87,12,47,180,150,66,81,9,208,207,6,14,130,101,11,102,122,254,229,170,109,93,212,132,3,69,253,202,45,210,13,41,194,221,104,251,160,61,234,214,137,205,222,103,193,95,211,138,10,21,16,126,237,67,58,30,220,177,92,218,133,83,72,245,96,85,129,108,115,192,25,157,186,68,48,215,112,125,116,27,241,149,34,15,82,223,31,29,201,71,172,64,141,187,151,240,111,70,51,22,52,236,203,230,178,168,143,23,35,110,252,134,228,243,100,173,175,158,55,17,184,99,185,248,166,209,38,179,44,196,54,181,113,135};

//hash関数の鍵----------------------------

unsigned char x0[NN]={12,24,4,2,45,25,30,22,27,28,53,35,34,59,7,62,39,50,42,21,16,60,49,6,43,32,15,26,18,11,0,33,52,55,3,9,10,47,37,17,61,46,56,40,20,54,63,36,38,19,57,58,44,41,23,31,5,29,51,48,14,13,8,1};

unsigned char inv_x[NN] = { 0 };


unsigned char x1[NN]={49,13,8,28,27,63,2,39,34,46,12,45,41,4,1,42,35,21,50,26,10,47,56,57,11,5,53,14,0,43,29,33,40,36,38,51,20,30,7,6,19,60,3,48,58,23,55,32,59,61,9,16,62,25,31,24,22,54,52,15,18,44,37,17};
//--------------------ここまで

//unsigned char y0[32]={27,24,23,6,18,12,11,14,4,5,2,29,22,1,3,17,15,21,31,26,19,30,0,8,28,7,20,13,10,25,16,9};
//unsigned char y1[32]={20,7,26,9,6,12,8,16,15,22,23,17,29,25,10,24,30,28,27,31,18,13,19,14,4,1,3,11,0,2,5,21};


#define str_length 128
#define password_length 256

unsigned char password[password_length + 1];


//言わずと知れたxorshift
unsigned int
xor (void)
{
  static unsigned int y = 2463534242;
  y = y ^ (y << 13);
  y = y ^ (y >> 17);
  return y = y ^ (y << 15);
}

unsigned long long int
xor64 (void)
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
  srand (clock () + time (&t));
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
      //inv_y[tt[i]]=i;
    }

  //for(i=0;i<NN;i++)
  //  printf("%d,",inv_x[i]);
  //  exit(1);


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
	  /*
	  //paddaing
	  if(n<2048){
	    for(i=n;i<2048;i++)
	      buf[i]=0xc6;
	  }
	  */
	  //memset(h.h,0,sizeof(h.h));
	  n = 0;
	  a = chash (buf);
	  /*
	     for(j=0;j<4;j++){
	     for(i=j*64;i<j*64+64;i++){
	     buf2[i]=buf.d[i];
	     a=chash(buf2);
	     for(k=0;k<64;k++)
	     b.ar[k]^=a.ar[k];
	     }
	     }
	   */
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
  else
    {

      fp = fopen ("test.bin", "wb");
      /*
         for(i=0;i<NN;i++)
         buf[i]=rand()%256;
         printf("input number! ->");
         scanf("%d",&n);
         buf[0]^=n;
         printf("%d\n",buf[0]);
       */
      memset (h.h, 0, sizeof (h.h));
      n = 0;

      while (n < 1000000)
	{
	  a = chash (buf);

	  for (i = 0; i < 16; i++)
	    {
	      for (j = 0; j < 4; j++)
		{
		  h.h[i] ^= a.ar[i * 4 + j];
		  if (j != 3)
		    h.h[i] = (h.h[i] << 8);
		}
	    }
	  fwrite (h.h, 4, 16, fp);
	  n++;
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
  //  for(i=0;i<NN;i++)
  //printf("%d,",a.ar[i]);
  //  exit(1);
  
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
  
  if (BYTE)
    {

      /*
         for(i=0;i<256;i++)
         password[i]=rand()%256;
         p=crand(password);
         for(i=0;i<8;i++)
         printf("%llx",p.u[i]);
         printf("\n");
       */
      //
      //  for(i=0;i<32;i++)
      /*
      scanf("%s",b);
      while(1){
	p=crand(b);
	for(i=0;i<NN;i++)
	  b[i]^=p.d[i];
	for(i=0;i<32;i++)
	  printf("%d,",b[i]);
	printf("\n");
      }
      exit(1);
      */
      t = hash (argc, argv);
      //慎ましくここは256ビットだけ
      for (i = 0; i < 16 / 2; i++)
	printf ("%08x", t.h[i]);
      printf (" %s", argv[1]);
      printf ("\n");

    }

  return 0;
}

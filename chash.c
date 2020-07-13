#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


#define N 256
#define NN 256
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



//p.cで作ったランダムな置換
unsigned char x0[NN] =
  { 55, 98, 138, 234, 151, 127, 184, 128, 60, 135, 164, 157, 213, 212, 175,
21, 87, 188, 71, 94, 178, 102, 110, 235, 191, 236, 44, 114, 196, 16, 3, 201, 169, 197, 143,
203, 245, 28, 148, 218, 37, 120, 96, 182, 125, 153, 226, 223, 198, 129, 224, 97, 25, 18, 70,
180, 86, 24, 8, 38, 112, 9, 199, 149, 124, 63, 200, 237, 51, 42, 36, 239, 142, 33, 84, 152,
209, 64, 174, 154, 206, 187, 240, 130, 14, 107, 183, 181, 137, 210, 13, 252, 115, 232, 147, 20,
72, 1, 43, 249, 204, 53, 12, 118, 217, 161, 208, 30, 15, 238, 254, 162, 170, 141, 22, 26, 227,
41, 134, 103, 91, 190, 168, 248, 216, 100, 56, 246, 251, 5, 78, 66, 104, 243, 101, 211, 185,
50, 166, 189, 59, 90, 35, 67, 133, 65, 186, 122, 40, 81, 105, 7, 230, 159, 46, 121, 195, 119,
74, 167, 23, 207, 123, 158, 136, 57, 140, 80, 73, 52, 68, 2, 77, 106, 61, 225, 79, 176, 62, 48,
139, 89, 253, 11, 4, 221, 145, 219, 69, 83, 82, 29, 241, 88, 6, 27, 165, 126, 173, 222, 160,
58, 0, 150, 76, 156, 116, 205, 192, 19, 244, 214, 229, 131, 32, 111, 99, 49, 163, 92, 95, 228,
215, 231, 247, 177, 108, 31, 172, 45, 193, 132, 255, 179, 34, 113, 39, 144, 194, 85, 202, 93,
171, 220, 155, 233, 109, 146, 250, 17, 117, 242, 10, 54, 75, 47 };

//unsigned char x0[NN]={2,38,6,59,63,9,17,52,23,47,53,15,33,8,35,1,39,28,26,20,50,36,43,10,54,11,16,3,51,41,4,58,25,7,18,27,5,56,19,32,45,30,55,29,21,14,34,12,24,31,44,62,42,48,22,57,61,60,46,37,40,13,0,49};


unsigned char inv_x[NN] = { 0 };

  //62,15,0,27,30,36,2,33,13,5,23,25,47,61,45,11,26,6,34,38,19,44,54,8,48,32,18,35,17,43,41,49,39,12,46,14,21,59,1,16,60,29,52,22,50,40,58,9,53,63,20,28,7,10,24,42,37,55,31,3,57,56,51,4};


unsigned char x1[NN] =
  { 98, 73, 0, 170, 16, 80, 100, 30, 147, 191, 47, 254, 22, 253, 242, 103,
112, 44, 69, 48, 28, 174, 173, 182, 188, 18, 150, 138, 66, 61, 207, 208, 78, 59, 169, 51, 158,
246, 202, 45, 247, 178, 71, 104, 134, 62, 228, 34, 243, 237, 109, 221, 9, 49, 143, 41, 46, 235,
36, 154, 42, 99, 159, 67, 166, 233, 17, 107, 232, 43, 250, 121, 40, 146, 25, 126, 92, 95, 190,
94, 195, 139, 115, 87, 50, 205, 111, 11, 140, 211, 251, 157, 142, 239, 131, 176, 217, 93, 118,
97, 153, 74, 210, 21, 76, 123, 83, 68, 227, 196, 238, 105, 187, 72, 206, 135, 35, 96, 161, 129,
116, 75, 244, 130, 219, 171, 226, 117, 149, 252, 3, 214, 55, 29, 33, 113, 119, 223, 194, 127,
165, 151, 15, 26, 7, 91, 32, 125, 200, 180, 199, 90, 183, 163, 24, 88, 5, 230, 212, 229, 204,
175, 8, 77, 241, 240, 155, 201, 85, 2, 137, 198, 162, 58, 63, 31, 60, 255, 10, 101, 193, 248,
197, 12, 133, 79, 164, 23, 38, 222, 168, 6, 102, 203, 13, 56, 81, 1, 19, 53, 220, 231, 64, 124,
145, 136, 160, 128, 89, 172, 234, 225, 144, 189, 132, 110, 167, 224, 152, 82, 114, 70, 209,
122, 213, 86, 156, 245, 216, 184, 192, 148, 84, 186, 141, 54, 27, 249, 181, 177, 65, 39, 218,
20, 120, 185, 215, 4, 14, 108, 236, 179, 106, 37, 57, 52 };

//unsigned char x1[NN]={33,37,25,9,5,35,41,57,53,19,61,8,0,31,47,60,46,54,26,32,44,36,14,48,58,59,38,22,6,16,51,56,4,13,23,42,24,39,2,50,1,15,30,45,34,20,21,28,12,29,17,27,10,55,52,7,63,43,18,40,49,11,62,3};



#define str_length 128
#define password_length 256

unsigned char password[password_length + 1];


//AES S-box
unsigned char Sbox[256] = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x1
};

/************************************************************/
/* FIPS 197  P.22 Figure 14 */
unsigned char invSbox[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
    0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44,
    0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b,
    0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
    0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc,
    0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57,
    0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
    0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03,
    0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce,
    0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
    0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e,
    0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe,
    0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
    0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f,
    0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c,
    0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
    0x55, 0x21, 0x0c, 0x7d
};

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

  unsigned char salt[NN] = { 0 };	//={ 148, 246, 52, 251, 16, 194, 72, 150, 249, 23, 90, 107, 151, 42, 154, 124, 48, 58, 30, 24, 42, 33, 38, 10, 115, 41, 164, 16, 33, 32, 252, 143, 86, 175, 8, 132, 103, 231, 95, 190, 61, 29, 215, 75, 251, 248, 72, 48, 224, 200, 147, 93, 112, 25, 227, 223, 206, 137, 51, 88, 109, 214, 17, 172};

  unsigned char z[NN], w[NN]={0};
  unsigned char v[256] = { 0 }, g[NN] = { 0 } , f[NN] = { 0 };
  unsigned char inv_y[NN];
  FILE *fp, *op;
  int  count = 1;
  time_t t;
  int k;

  
  for (i = 0; i < NN; i++)
    salt[i] = xor64 ();

  for (i = 0; i < NN; i++)
    {
      inv_x[x0[i]] = i;
      //inv_y[tt[i]]=i;
    }

  //for(i=0;i<NN;i++)
  //  printf("%d,",inv_x[i]);
  //  exit(1);


  //#pragma omp parallel for
  for (i = 0; i < NN; i++)
    f[i] ^= salt[i];


  k = 0;
  for (i = 0; i < 256; i++)
    f[i] ^= b[i];


  memcpy (v, f, sizeof (unsigned char) * NN);

  //1byteデータが256バイト埋め尽くされる大体のループ
  for (j = 0; j < 10; j++)
    {
      for (i = 0; i < NN; i++)
	z[i] = x0[x1[inv_x[i]]];

      memcpy (x1, z, sizeof (unsigned char) * NN);
      
      c.u[0]=xor64();
      //printf("%llu\n",c.u[0]);
      
      for (i = 0; i < NN; i++)
	{
	  if (f[x1[i]] > 0)
	    {
	      //f[i]^=Sbox[ROTL8(f[x1[i]],3)];
	      v[i] ^= Sbox[ROTL8 (f[x1[i]]^c.ar[i], 3)];

	    }
	}
      memcpy (f, v, sizeof (unsigned char) * NN);

      /*
         for(i=0;i<NN;i++)
         printf("%d,",f[i]);
         printf("\n");
       */

      if(count < 2048 / NN)
	{			//k=1;k<2048/NN;k++){
	  for (i = 0; i < 256; i++){
	    f[i] ^= b[count * NN + i];
	    // f[i]^=c.ar[i];
	  }
	}
      
      //padding?
      if(count>=2048/NN){
	for(i=0;i<NN;i++)
	  f[i]^=i;
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
  j = 0;
  memset (b.u, 0, sizeof (b.u));
  for (i = 0; i < NN; i++)
    {
      b.u[j] ^= a.ar[i];
      b.u[j] = b.u[j] << 8;
      if (i > 0 && i % 8 == 0)
	j++;
    }

  return b;
}


int
main (int argc, char *argv[])
{
  int i, j, n;
  arrayul p;
  array16 t;

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
      t = hash (argc, argv);

      //慎ましくここは256ビットだけ
      for (i = 0; i < 16 / 2; i++)
	printf ("%08x", t.h[i]);
      printf (" %s", argv[1]);
      printf ("\n");

    }
  else
    {
      /*
         seed();
         p=crand(password);
         for(i=0;i<8;i++)
         printf("%llx",p.u[i]);
         printf("\n");
         t=hash(argc,argv);
         for(i=0;i<16/2;i++)
         printf("%08x",t.h[i]);
         printf(" %s",argv[1]);
         printf("\n");
       */
    }

  return 0;
}

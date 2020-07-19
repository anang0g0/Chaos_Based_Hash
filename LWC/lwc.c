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
  unsigned int d[NN/4];
  unsigned long long int u[NN/8];
  unsigned char ar[NN];
  //
} arrayn;

typedef struct {

  unsigned char c[2048];
  
} arrayA;


typedef struct pub
{
  unsigned char a[NN];
  unsigned char b[NN];
} set;

arrayn c={0};



//#define I8T char
#define U32C(v) (v##UL)
#define U32V(v) ((unsigned int)(v) & U32C(0xFFFFFFFF))
#define U8V(v) ((unsigned char)(v) & (0xFF))
//#define U8V(v) ((unsigned char)(v) & U8V(0xFF))
#define U16V(v) ((u16)(v) & U16V(0xFFFF))
//#define U32V(v) ((unsigned int )(v) & U32V(0xFFFFFFFF))
//#define U64C(v) (v##ULL)
#define U64V(v) ((unsigned long long int)(v) & (0xFFFFFFFFFFFFFFFFULL))
#define ROTL8(v, n) \
  (U8V((v) << (n)) | ((v) >> (8 - (n))))

#define ROTL16(v, n)				\
  (U16V((v) << (n)) | ((v) >> (16 - (n))))

#define ROTL32(v, n) \
  (U32V((v) << (n)) | ((v) >> (32 - (n))))

#define ROTL64(v, n) \
  (U64V((v) << (n)) | ((v) >> (64 - (n))))

#define ROTR8(v, n) ROTL8(v, 8 - (n))
#define ROTR16(v, n) ROTL16(v, 16 - (n))
#define ROTR32(v, n) ROTL32(v, 32 - (n))
#define ROTR64(v, n) ROTL64(v, 64 - (n))








unsigned char inv_x[NN] = { 0 };




//substitution
unsigned char gf[N]={0,1,2,4,8,16,32,64,128,29,58,116,232,205,135,19,38,76,152,45,90,180,117,234,201,143,3,6,12,24,48,96,192,157,39,78,156,37,74,148,53,106,212,181,119,238,193,159,35,70,140,5,10,20,40,80,160,93,186,105,210,185,111,222,161,95,190,97,194,153,47,94,188,101,202,137,15,30,60,120,240,253,231,211,187,107,214,177,127,254,225,223,163,91,182,113,226,217,175,67,134,17,34,68,136,13,26,52,104,208,189,103,206,129,31,62,124,248,237,199,147,59,118,236,197,151,51,102,204,133,23,46,92,184,109,218,169,79,158,33,66,132,21,42,84,168,77,154,41,82,164,85,170,73,146,57,114,228,213,183,115,230,209,191,99,198,145,63,126,252,229,215,179,123,246,241,255,227,219,171,75,150,49,98,196,149,55,110,220,165,87,174,65,130,25,50,100,200,141,7,14,28,56,112,224,221,167,83,166,81,162,89,178,121,242,249,239,195,155,43,86,172,69,138,9,18,36,72,144,61,122,244,245,247,243,251,235,203,139,11,22,44,88,176,125,250,233,207,131,27,54,108,216,173,71,142};

unsigned char fg[N]={0,1,2,26,3,51,27,199,4,224,52,239,28,105,200,76,5,101,225,15,53,142,240,130,29,194,106,249,201,9,77,114,6,139,102,48,226,37,16,34,54,148,143,219,241,19,131,70,30,182,195,126,107,40,250,186,202,155,10,121,78,229,115,167,7,192,140,99,103,222,49,254,227,153,38,180,17,146,35,137,55,209,149,207,144,151,220,190,242,211,20,93,132,57,71,65,31,67,183,164,196,73,127,111,108,59,41,85,251,134,187,62,203,95,156,160,11,22,122,44,79,213,230,173,116,244,168,88,8,113,193,248,141,129,100,14,104,75,223,238,50,198,255,25,228,166,154,120,39,185,181,125,18,69,147,218,36,33,138,47,56,64,210,92,150,189,208,206,145,136,152,179,221,253,191,98,243,87,212,172,21,43,94,159,133,61,58,84,72,110,66,163,32,46,68,217,184,124,165,119,197,24,74,237,128,13,112,247,109,162,60,83,42,158,86,171,252,97,135,178,188,205,63,91,204,90,96,177,157,170,161,82,12,246,23,236,123,118,45,216,80,175,214,234,231,232,174,233,117,215,245,235,169,81,89,176};



#define str_length 128
#define password_length 256

unsigned char password[password_length + 1];


//AES S-box
static unsigned char Sbox[256] = {
0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76
,0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0
,0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15
,0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75
,0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84
,0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf
,0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8
,0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2
,0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73
,0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb
,0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79
,0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08
,0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a
,0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e
,0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf
,0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
  /*
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
  */
};

/************************************************************/
/* FIPS 197  P.22 Figure 14 */
static unsigned char invSbox[256] = {
0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb
,0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb
,0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e
,0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25
,0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92
,0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84
,0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06
,0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b
,0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73
,0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e
,0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b
,0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4
,0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f
,0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef
,0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61
,0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
  /*
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
  */
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
arrayA
enc (unsigned char b[2048])
{
  int i, j = 0;
  arrayA n;
  arrayn tmp={0};
  unsigned char key[NN]={12,24,4,2,45,25,30,22,27,28,53,35,34,59,7,62,39,50,42,21,16,60,49,6,43,32,15,26,18,11,0,33,52,55,3,9,10,47,37,17,61,46,56,40,20,54,63,36,38,19,57,58,44,41,23,31,5,29,51,48,14,13,8,1};
  unsigned char x0[NN]={12,24,4,2,45,25,30,22,27,28,53,35,34,59,7,62,39,50,42,21,16,60,49,6,43,32,15,26,18,11,0,33,52,55,3,9,10,47,37,17,61,46,56,40,20,54,63,36,38,19,57,58,44,41,23,31,5,29,51,48,14,13,8,1};
  unsigned char x1[NN]={49,13,8,28,27,63,2,39,34,46,12,45,41,4,1,42,35,21,50,26,10,47,56,57,11,5,53,14,0,43,29,33,40,36,38,51,20,30,7,6,19,60,3,48,58,23,55,32,59,61,9,16,62,25,31,24,22,54,52,15,18,44,37,17};

 //p.cで作った置換
  //unsigned char x0[NN] ={98,175,221,11,87,29,222,105,121,50,41,166,103,23,144,249,164,234,82,212,33,235,122,117,55,220,37,80,12,229,73,236,8,240,126,44,97,124,132,210,85,104,95,145,88,136,244,216,60,81,34,185,242,2,133,94,99,134,251,89,75,18,24,100,129,223,140,179,21,102,207,38,174,74,69,46,78,107,173,96,48,125,91,246,254,90,227,139,187,233,215,189,47,170,57,72,226,217,76,196,53,248,181,39,197,31,70,62,219,150,30,79,201,32,250,112,225,19,147,43,16,58,182,167,228,193,232,239,83,42,191,67,71,45,169,115,26,237,213,205,198,162,65,7,119,28,148,161,116,160,101,186,4,114,209,123,165,127,49,152,22,154,146,68,171,241,184,51,245,163,168,231,253,177,86,190,158,200,195,9,130,17,149,92,61,13,230,142,143,20,247,135,66,224,5,238,15,155,151,111,199,108,178,0,137,52,3,109,252,138,159,141,211,6,188,128,156,194,63,1,36,203,255,35,113,64,14,208,120,206,25,54,110,176,84,153,59,243,214,183,204,106,218,118,180,40,93,77,157,27,56,172,10,202,131,192};

  //unsigned char x1[NN]={235,42,200,88,24,146,20,53,107,128,165,78,74,204,0,18,183,75,169,148,123,227,117,155,97,114,37,76,213,244,190,231,174,46,91,238,7,233,4,62,50,56,121,127,43,147,40,84,39,79,119,136,28,139,219,163,250,159,189,246,198,118,26,106,140,242,162,182,2,8,80,153,142,216,239,224,94,226,89,145,255,105,247,49,171,5,90,19,161,73,195,152,197,199,77,232,124,206,57,167,156,1,217,144,176,249,154,164,131,33,59,63,60,32,225,65,36,86,120,98,188,191,87,12,47,180,150,66,81,9,208,207,6,14,130,101,11,102,122,254,229,170,109,93,212,132,3,69,253,202,45,210,13,41,194,221,104,251,160,61,234,214,137,205,222,103,193,95,211,138,10,21,16,126,237,67,58,30,220,177,92,218,133,83,72,245,96,85,129,108,115,192,25,157,186,68,48,215,112,125,116,27,241,149,34,15,82,223,31,29,201,71,172,64,141,187,151,240,111,70,51,22,52,236,203,230,178,168,143,23,35,110,252,134,228,243,100,173,175,158,55,17,184,99,185,248,166,209,38,179,44,196,54,181,113,135};

 
  unsigned char salt[NN] = { 0 };	//={ 148, 246, 52, 251, 16, 194, 72, 150, 249, 23, 90, 107, 151, 42, 154, 124, 48, 58, 30, 24, 42, 33, 38, 10, 115, 41, 164, 16, 33, 32, 252, 143, 86, 175, 8, 132, 103, 231, 95, 190, 61, 29, 215, 75, 251, 248, 72, 48, 224, 200, 147, 93, 112, 25, 227, 223, 206, 137, 51, 88, 109, 214, 17, 172};

  unsigned char z[NN]={0}, w[NN]={0},aa[NN]={0},a=0;
  unsigned char v[NN] = { 0 }, f[NN] = { 0 };
  unsigned char inv_y[NN];
  FILE *fp, *op;
  int  count = 1;
  time_t t;
  int k;

  srand(111);
  //printf("in enc\n");
  for (i = 0; i < NN; i++)
    {
      inv_x[x0[i]] = i;
      //inv_y[tt[i]]=i;
    }

  //for(i=0;i<NN;i++)
  //  printf("%d,",inv_x[i]);
  //  exit(1);


  //デバッグ中なので省略
  for (i = 0; i < NN; i++)
    //f[i] ^= salt[i];


  k = 0;
  for (i = 0; i < NN; i++)
    f[i] ^= b[i];

  int l;
  memcpy (v, f, sizeof (unsigned char) * NN);
  memcpy(tmp.ar,key,sizeof(unsigned char)*NN);//Sbox[key[z[i]]];
  //バッファを埋める回数だけ回す
  for (j = 0; j < 2048/NN; j++)
    {

      
	for (l = 0; l < NN; l++)
	  z[l] = x0[x1[inv_x[l]]];
	
      
      memcpy (x1, z, sizeof (unsigned char) * NN);

      unsigned long long int u[NN/8]={0};
      
      for(k=0;k<10;k++){

	/*
	a=tmp.ar[0];
	for(i=1;i<NN;i++)
	  key[i-1]=tmp.ar[i];
	key[NN-1]=a;
	*/
	//memcpy(key,tmp.ar,sizeof(unsigned char)*NN);

	for(i=0;i<NN/8;i++)
	u[i]=ROTL64(tmp.u[i],3);
	memcpy(tmp.u,u,sizeof(unsigned long long int)*(NN/8));
	memcpy(key,tmp.ar,sizeof(unsigned char)*(NN));
	
	/*
	for (l = 0; l < NN; l++)
	  z[l] = x0[x1[inv_x[l]]];
	
      memcpy (x1, z, sizeof (unsigned char) * NN);
	*/
	
	//for(i=0;i<NN;i++)
	//key[i]=rand()%256;
	
      for (i = 0; i < NN; i++)
	{
	  
	  v[i] = Sbox[f[z[i]]]^key[i];//gf[f[z[i]]];

	}
      
      //roun();
      }
      memcpy (f, v, sizeof (unsigned char) * NN);      

      //}

      //print for debugging
      for(i=0;i<NN;i++){
	//printf("%02x",f[i]);
	n.c[j*NN+i]=f[i];
      }
      //printf("\n");
      

      if(count < 2048 / NN)
	{			//k=1;k<2048/NN;k++){
	  for (i = 0; i < NN; i++){
	    f[i] = b[count * NN + i];
	  }
	}

      count++;
    }

  //memcpy (n.ar, f, sizeof (unsigned char) * NN);

  //exit(1);


  
  return n;

    }


//ハッシュ関数本体
arrayA
dec (unsigned char b[2048])
{
  int i, j = 0;
  arrayA n;
  arrayn tmp={0};
  unsigned char key[NN]={12,24,4,2,45,25,30,22,27,28,53,35,34,59,7,62,39,50,42,21,16,60,49,6,43,32,15,26,18,11,0,33,52,55,3,9,10,47,37,17,61,46,56,40,20,54,63,36,38,19,57,58,44,41,23,31,5,29,51,48,14,13,8,1};
  arrayn key2={0};

  
    unsigned char x0[NN]={12,24,4,2,45,25,30,22,27,28,53,35,34,59,7,62,39,50,42,21,16,60,49,6,43,32,15,26,18,11,0,33,52,55,3,9,10,47,37,17,61,46,56,40,20,54,63,36,38,19,57,58,44,41,23,31,5,29,51,48,14,13,8,1};
  unsigned char x1[NN]={49,13,8,28,27,63,2,39,34,46,12,45,41,4,1,42,35,21,50,26,10,47,56,57,11,5,53,14,0,43,29,33,40,36,38,51,20,30,7,6,19,60,3,48,58,23,55,32,59,61,9,16,62,25,31,24,22,54,52,15,18,44,37,17};

//p.cで作った置換
//unsigned char x0[NN] ={98,175,221,11,87,29,222,105,121,50,41,166,103,23,144,249,164,234,82,212,33,235,122,117,55,220,37,80,12,229,73,236,8,240,126,44,97,124,132,210,85,104,95,145,88,136,244,216,60,81,34,185,242,2,133,94,99,134,251,89,75,18,24,100,129,223,140,179,21,102,207,38,174,74,69,46,78,107,173,96,48,125,91,246,254,90,227,139,187,233,215,189,47,170,57,72,226,217,76,196,53,248,181,39,197,31,70,62,219,150,30,79,201,32,250,112,225,19,147,43,16,58,182,167,228,193,232,239,83,42,191,67,71,45,169,115,26,237,213,205,198,162,65,7,119,28,148,161,116,160,101,186,4,114,209,123,165,127,49,152,22,154,146,68,171,241,184,51,245,163,168,231,253,177,86,190,158,200,195,9,130,17,149,92,61,13,230,142,143,20,247,135,66,224,5,238,15,155,151,111,199,108,178,0,137,52,3,109,252,138,159,141,211,6,188,128,156,194,63,1,36,203,255,35,113,64,14,208,120,206,25,54,110,176,84,153,59,243,214,183,204,106,218,118,180,40,93,77,157,27,56,172,10,202,131,192};

//unsigned char x1[NN]={235,42,200,88,24,146,20,53,107,128,165,78,74,204,0,18,183,75,169,148,123,227,117,155,97,114,37,76,213,244,190,231,174,46,91,238,7,233,4,62,50,56,121,127,43,147,40,84,39,79,119,136,28,139,219,163,250,159,189,246,198,118,26,106,140,242,162,182,2,8,80,153,142,216,239,224,94,226,89,145,255,105,247,49,171,5,90,19,161,73,195,152,197,199,77,232,124,206,57,167,156,1,217,144,176,249,154,164,131,33,59,63,60,32,225,65,36,86,120,98,188,191,87,12,47,180,150,66,81,9,208,207,6,14,130,101,11,102,122,254,229,170,109,93,212,132,3,69,253,202,45,210,13,41,194,221,104,251,160,61,234,214,137,205,222,103,193,95,211,138,10,21,16,126,237,67,58,30,220,177,92,218,133,83,72,245,96,85,129,108,115,192,25,157,186,68,48,215,112,125,116,27,241,149,34,15,82,223,31,29,201,71,172,64,141,187,151,240,111,70,51,22,52,236,203,230,178,168,143,23,35,110,252,134,228,243,100,173,175,158,55,17,184,99,185,248,166,209,38,179,44,196,54,181,113,135};

  memcpy(key2.ar,key,sizeof(unsigned char)*NN); 
  unsigned char salt[NN] = { 0 };

  unsigned char z[NN]={0}, w[NN]={0},aa[NN]={0},a=0;
  unsigned char v[NN] = { 0 }, f[NN] = { 0 };
  unsigned char inv_y[NN];
  FILE *fp, *op;
  int  count = 1;
  time_t t;
  int k;

  srand(111);
  printf("in dec\n");

  printf("\n");
  //  exit(1);
  
  for (i = 0; i < NN; i++)
    {
      inv_x[x0[i]] = i;
      //inv_y[tt[i]]=i;
    }


  //デバッグ中なので省略
  for (i = 0; i < NN; i++)
    //f[i] ^= salt[i];


  k = 0;
  for (i = 0; i < NN; i++)
    f[i] ^= b[i];

  int l;
  memcpy (v, f, sizeof (unsigned char) * NN);
  memcpy(tmp.ar,key,sizeof(unsigned char)*NN);
  //バッファを埋める回数だけ回す
  for (j = 0; j < 2048/NN; j++)
    {

      
      for (l = 0; l < NN; l++)
	z[l] = x0[x1[inv_x[l]]];
      

	for(l=0;l<NN;l++)
	w[z[l]]=l;

      memcpy (x1, z, sizeof (unsigned char) * NN);
      
      
      unsigned long long int u[NN/8]={0};

      for(k=0;k<10;k++){
	
	/*
      for (l = 0; l < NN; l++)
	z[l] = x0[x1[inv_x[l]]];

      

      //for(l=0;l<NN;l++)
      //w[z[l]]=l;

      memcpy (x1, z, sizeof (unsigned char) * NN);
	*/

	
	for(i=0;i<NN/8;i++)
	u[i]=ROTL64(tmp.u[i],3);
	memcpy(tmp.u,u,sizeof(unsigned long long int)*(NN/8));
	memcpy(key,tmp.ar,sizeof(unsigned char)*(NN));
	
	/*
	a=tmp.ar[0];
	for(i=1;i<NN;i++)
	  key[i-1]=tmp.ar[i];
	key[NN-1]=a;
	*/
		
	//memcpy(tmp.ar,key,sizeof(unsigned char)*NN);
	
	/*
      for(i=0;i<NN;i++){
	key[i]=rand()%256;
      }
	*/
	/*
	//#pragma omp parallel for
	for(i=0;i<NN;i++)
	  key[i]=tmp.ar[z[i]];
	memcpy(tmp.ar,key,sizeof(unsigned char)*NN);
	*/
      for (i = 0; i < NN; i++)
	{

	  v[i]=invSbox[f[i]^key[i]];//^key[i];
	    	      
	}
      
      //roun();
      }      
      
      for(i=0;i<NN;i++)
	f[i]=v[w[i]];
      //memcpy (f, v, sizeof (unsigned char) * NN);

      //}
      
      //print for debugging
      for(i=0;i<NN;i++){
	//printf("%c",f[i]);
	n.c[j*NN+i]=f[i];
      }
      //printf("\n");
      

      if(count < 2048 / NN)
	{			//k=1;k<2048/NN;k++){
	  for (i = 0; i < NN; i++){
	    f[i] = b[count * NN + i];
	  }
	}

      count++;
    }

  //memcpy (n.ar, f, sizeof (unsigned char) * NN);

  //exit(1);


  
  return n;

}


//ファイル操作
arrayA
hash (int argc, char *argv[])
{
  int i, j, k, n;
  array16 h = { 0 };

  unsigned char buf[2048] = { 0 };
  FILE *fp,*fq;
  arrayA a = { 0 };
  arrayA b = { 0 };


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
	  //printf("@\n");
	  
	  //paddaing
	  if(n<2048){
	    for(i=n;i<2048;i++)
	      buf[i]=0xc6;
	  }
	  
	  //memset(h.h,0,sizeof(h.h));

	  a = enc (buf);
	  /*
	  for(j=0;j<n;j++)
	    printf("%02x",a.c[j]);
	  printf("\n");
	  
	  
	  b=dec(a.c);
      	  for(j=0;j<n;j++)
	  printf("%c",b.c[j]);
	  printf("\n");
	  */
	  n = 0;
	}
    }

  //exit(1);
  
  return a;
}


//蛇足
arrayA
  crand (unsigned char u[NN])
{
  arrayA a = { 0 };
  int i, j;
  arrayA b = { 0 };

  a = enc (u);
  j = 0;
  memset (b.c, 0, sizeof (b.c));
  for (i = 0; i < 2048; i++)
    {
      b.c[j] ^= a.c[i];
      b.c[j] = b.c[j] << 8;
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
  arrayA t;

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

      /*
      //慎ましくここは256ビットだけ
      for (i = 0; i < 2048; i++)
	printf ("%02x", t.c[i]);
      */
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

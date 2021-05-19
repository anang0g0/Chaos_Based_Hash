#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "8192.h"

//符号のパラーメータの指定。通常[N,K,T]として、
//Nは符号の長さ、Kが符号の次元、Tは訂正エラー数
//を表す。ここではDは符号長にしている。
#define N 256  // (^^;)?
#define M 256 //有限体の元の数
#define K (16) //符号の次元
#define DEG (K * E)
#define T (K / 2) //エラーの数
#define E (8)    //拡大体のビット数
#define D (256)  //符号長（短縮符号）
//#define F E *K    //2040
#define NN 64
#define KK NN/4

unsigned int counter=0;
unsigned int counter2=0;
unsigned char inv_x[NN]={0};
unsigned char z[NN]={0}; 
//unsigned char tmp[N][E * K] = {0};
//unsigned char pub[E * K][N] = {0};
//unsigned char BH[E * K][N] = {0};
static unsigned short c[E * K + 1] = {0};
unsigned short mat[N][K*E] = {0};
unsigned short ma[N][K*E] = {0};
unsigned short bm[N][K*E]={0};
unsigned short bm2[N][K*E]={0};

unsigned char x0[NN]={0}; //{61,33,59,39,5,58,25,29,54,49,52,35,46,57,34,6,47,9,31,42,17,24,44,2,23,3,51,30,60,45,14,50,7,12,41,48,21,0,62,32,10,20,55,18,1,26,19,43,27,53,56,11,15,28,22,38,13,36,8,37,4,40,63,16};
unsigned char x1[NN]={0}; //{37,29,14,40,15,6,57,30,41,1,53,10,13,34,11,9,50,32,44,61,55,54,39,26,43,12,25,22,19,35,24,56,21,4,58,45,8,59,47,46,0,42,49,63,2,51,33,36,28,62,17,7,3,31,23,5,48,60,27,20,38,52,18,16};

//unsigned short syn[K]={0};
unsigned short P[N] = {0};
unsigned short inv_P[N] = {0};
unsigned short uu = 0;

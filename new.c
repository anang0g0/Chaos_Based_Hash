#include "global.h"
#include "struct.h"
#include "chash.c"



int main()
{
    arrayul key={.d= {101,137,148,250,46,76,21,18,68,214,242,212,171,101,172,255,65,46,229,215,117,24,133,142,57,105,237,241,182,192,55,159,151,56,154,153,8,164,198,102,209,45,28,123,15,140,230,83,72,90,87,141,6,100,190,97,39,9,25,194,162,143,78,128}}; 
    FILE *fp;
    int count = 0, i,j;
    time_t t;

    srand(time(&t));

    rp2(x0);
    rp2(x1);
    /*
    for(i=0;i<NN;i++)
    printf("%d,",x0[i]);
    printf("\n");
    for(i=0;i<NN;i++)
    printf("%d,",x1[i]);
    printf("\n");
    exit(1);
    */
   fp=fopen("test.dat","wb");
   //for(i=0;i<NN;i++)
   //key.d[i]=i;
    //memcpy(key.d,salt,sizeof(salt));
    //key.t[0] = (unsigned int)time(&t);
    //key.t[1] = (unsigned long)clock();
    //for(i=0;i<NN;i++)
    //key.d[i]=0;
    printf("%lld, %lld\n", key.u[0], key.u[1]);
    nonce.u[0]=0; //0x1234567890abcdef;
    

    for (i = 0; i < NN; i++)
    {
        printf("%d,", key.d[i]);
    }
    printf("\n");

    for (i = 0; i < NN; i++)
        inv_x[x0[i]] = i;

//  最初の4ループを捨てる
    for (i = 0; i < 4; i++)
    {
        chash(&key);

    for (j = 0; j < NN; j++)
        printf("%d,", key.d[j]);
    printf("\n");

    }

    //exit(1);

    while (1)
    {
        //i=rand();

        chash(&key);

        count++;
        if (count == 1000004)
            break;

            fwrite(key.d,1,64,fp);
/*
    for(i=0;i<NN;i++)
    printf("%d,",key.d[i]);
    printf("\n");

    for (i = 0; i < NN / 16; i++)
    {
        print_uint128(key.z[i]);
        printf(" ");
    }
    printf("\n");
*/
    }
    for (i = 0; i < NN / 16; i++)
    {
        print_uint128(key.z[i]);
        printf(" ");
    }
    printf("\n");
    printf("counter2=%d\n",counter2);

    return 0;
}
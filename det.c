main(){
double a[4][4]={{2,-2,4,2},{2,-1,6,3},{3,-2,12,12},{-1,3,-4,4}};
double det=1.0,buf;
int n=4;  //配列の次数
int i,j,k;
 
//三角行列を作成
for(i=0;i<n;i++){
 for(j=0;j<n;j++){
  if(i<j){
   buf=a[j][i]/a[i][i];
   for(k=0;k<n;k++){
   a[j][k]-=a[i][k]*buf;
   }
  }
 }
}
 
//対角部分の積
for(i=0;i<n;i++){
 det*=a[i][i];
}
 
printf("%f\n",det); // -> 120.000000
}
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

const int N=3;
//Первая матрица:
int A[3][3];
  //Вторая матрица:
int B[3][3];
  //Треть матрица (результат):
int C[3][3];

void* thread1(void* dummy)
{
	
	int j = 0;
	for(int i=0; i < N; i++){
         
	   for(int k=0; k < N; k++)
          
           C[i][j]+=A[i][k]*B[k][j];
          
        }
	return NULL;
}

void* thread2(void* dummy)
{
	
	int j = 1;
	for(int i=0; i < N; i++){
         
	   for(int k=0; k < N; k++)
          
           C[i][j]+=A[i][k]*B[k][j];
          }
        
	return NULL;
}

void* thread3(void* dummy)
{
	int j = 2;
	for(int i=0; i < N; i++){
        
	   
          for(int k=0; k < N; k++)
          
           C[i][j]+=A[i][k]*B[k][j];
          
        }
	return NULL;
}


int main(){
  /*//Размер матриц:
   const int N=3;
*/
  /*
  //Первая матрица:
  global int A[N][N];
  //Вторая матрица:
  global int B[N][N];
  //Третья матрица (результат):
  global int C[N][N];
   */
 for(int i=0; i < N; i++){
  for(int j=0; j < N; j++)
  C[i][j]=0;
 } 

  //Ввод (построчный) элементов первой матрицы:
  printf("Matrix A:\n");
  for(int i=0; i < N; i++){
   for(int j=0; j < N; j++)
   scanf ( "%d ",   &A[i][j]);
  }

  //Ввод (построчный) элементов первой матрицы:
  printf("Matrix B:\n");
  for(int i=0; i < N; i++){
   for(int j=0; j < N; j++)
   scanf ( "%d ",   &B[i][j]);
  }
  
  
  //Вычесление новой матрицы 
  pthread_t thid1;
  pthread_t thid2;
  pthread_t thid3;
  pthread_create(&thid1, NULL, thread1, NULL);
  pthread_create(&thid2, NULL, thread2, NULL);
  pthread_create(&thid3, NULL, thread3, NULL);
  pthread_join(thid1, NULL);
  pthread_join(thid2, NULL);
  pthread_join(thid3, NULL);



for(int i=0; i < N; i++){
 for(int j=0; j < N; j++)
   printf("%d", C[i][j]);
 }
return 0;

}

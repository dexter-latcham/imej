#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "vectors.h"

int main(){
  srand(time(0));
  mat4 matrix;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      matrix.m[i][j] = rand();
    }
  }
  mat4 matrix2;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      matrix2.m[i][j] = rand();
    }
  }

  mat4 matrix3;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      matrix3.m[i][j] = rand();
    }
  }

  mat4 matrix4;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      matrix3.m[i][j] = rand();
    }
  }

  #define FACES 4
  #define LOOPS 1000000

  vec3 before;
  volatile vec3 after;
  volatile vec3 foo;
  time_t start = clock();
  for(volatile int x=0;x<LOOPS;x++){
    for(volatile int i=0;i<FACES;i++){
      before = (vec3){.x=rand(),.y=rand(),.z=rand()};
      multMat4Vec3Ptr(&foo,&matrix,&before);
      multMat4Vec3Ptr(&after,&matrix2,&foo);
      multMat4Vec3Ptr(&foo,&matrix3,&after);
      multMat4Vec3Ptr(&after,&matrix4,&foo);
    }
  }
  time_t end = clock();
  printf("%d iters in %fs\n",LOOPS,((double)(end - start))/CLOCKS_PER_SEC);

  start = clock();
  for(volatile int x=0;x<LOOPS;x++){
    mat4 comb = multMat4Mat4(matrix,matrix2);
    comb = multMat4Mat4(comb,matrix3);
    comb = multMat4Mat4(comb,matrix4);
    for(volatile int i=0;i<FACES;i++){
      before = (vec3){.x=rand(),.y=rand(),.z=rand()};
      after = multMat4Vec3(comb,before);
    }
  }
  end = clock();
  printf("%d iters in %fs\n",LOOPS,((double)(end - start))/CLOCKS_PER_SEC);
}

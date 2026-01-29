#include "../include/window.h"
#include "../include/imej.h"
#include "../include/extras/cube.h"
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define OBJECTS 1999
#define LOOPS 40000

int main(){
  Object* objList[OBJECTS];
  Object* obj = loadObjectWithColour("model.obj",WHITECOL);
  Object* new;
  for(int i=0;i<OBJECTS;i++){
    objList[i] = cloneObject(obj);
  }
  objectList* list = createObjectList();
  addObjectToList(list,obj);
  volatile int i;
  time_t start = clock();
  for(i=0;i<OBJECTS;i++){
    addObjectToList(list,objList[i]);
  }
  time_t end = clock();
  printf("Took %fs to create a list with %d objects\n",((double)(end - start))/CLOCKS_PER_SEC, OBJECTS);
  start = clock();
  for(volatile int j=0;j<LOOPS;j++){
    Object* iter = objectListIterator(list);
    while(iter!= NULL){
      i++;
      iter = objectListIterator(NULL);
    }
  }
  end = clock();
  printf("iterated %d times in %fs\n",LOOPS, ((double)(end - start))/CLOCKS_PER_SEC);
  deleteObjectList(list);
  return 1;
}


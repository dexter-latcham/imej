#include "object.h"
#include "translation.h"
#include <stdlib.h>

#define MAXOBJECTS 2000

typedef struct objectList{
  Object* list[MAXOBJECTS];
  int freePtr;
}objectList;

Object* objectListIterator(objectList* silo){
  static objectList* storedList;
  static int index=0;
  if(silo!=NULL){
    storedList=silo;
    index=0;
  }
  if(index < storedList->freePtr){
    index++;
    return storedList->list[index-1];
  }
  return NULL;
}

objectList *createObjectList(){
  objectList * silo = (objectList*)malloc(sizeof(objectList));
  if(silo == NULL){
    return NULL;
  }
  silo->freePtr=0;
  return silo;
}

int addObjectToList(objectList *list, Object * newObj){
  if(list->freePtr!=MAXOBJECTS){
    list->list[list->freePtr++]=newObj;
    return 0;
  }
  return 1;
}

int deleteObjectFromList(objectList * silo, Object * obj){ 
  int index=0;
  for(index=0;index<MAXOBJECTS;index++){
    if(silo->list[index]==obj){
      deleteObject(obj);
      if(index == silo->freePtr-1){
        silo->freePtr-=1;
        return 0;
      }else{
        silo->list[index] = silo->list[silo->freePtr -1];
        silo->freePtr-=1;
        return 0;
      }
    }
  }
  return 1;
}

void deleteObjectList(objectList* silo){
  if(silo==NULL){
    return;
  }
  int index=0;
  for(index=0;index<MAXOBJECTS;index++){
    if(silo->list[index]!=NULL){
      deleteObject(silo->list[index]);
      silo->list[index]=NULL;
    }
  }
  free(silo);
}

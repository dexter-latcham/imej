#include "object.h"
#include "translation.h"
#include <stdlib.h>

#define nodeContentsSize 400

typedef struct objectList{
  Object* obj[nodeContentsSize];
  int freePtr;
  struct objectList *next;
}objectList;

objectList *createObjectList(){
  objectList * silo = (objectList*)malloc(sizeof(objectList));
  if(silo == NULL){
    return NULL;
  }
  silo->next = NULL;
  silo->freePtr=0;
  return silo;
}

int addObjectToList(objectList *silo, Object * newObj){
  if(silo->freePtr == nodeContentsSize){
    if(silo->next==NULL){
      silo->next = createObjectList();
    }
    return addObjectToList(silo->next,newObj);
  }
  silo->obj[silo->freePtr]=newObj;
  silo->freePtr+=1;
  return 0;
}

int deleteObjectFromList(objectList * list, Object * obj){ 
  int index =0;
  for(index=0;index<list->freePtr;index++){
    if(list->obj[index]==obj){
      deleteObject(obj);
      if(index == list->freePtr -1){
        list->freePtr-=1;
        return 0;
      }else{
        list->freePtr-=1;
        list->obj[index]=list->obj[list->freePtr];
        return 0;
      }
    }
  }
  if(list->freePtr == nodeContentsSize){
    if(list->next != NULL){
      return deleteObjectFromList(list->next,obj);
    }
  }
  return 1;
}

void deleteObjectList(objectList* list){
  if(list->next != NULL){
    deleteObjectList(list->next);
    list->next = NULL;
  }
  for(int i=0;i<list->freePtr;i++){
    deleteObject(list->obj[i]);
  }
  free(list);
}

Object* objectListIterator(objectList* list){
  static objectList* cur = NULL;
  static int counter =0;
  if(list != NULL){
    cur = list;
    counter =0; 
  }
  if(counter < cur->freePtr){
    counter++;
    return cur->obj[counter-1];
  }
  if(cur->next != NULL){
    cur = cur->next;
    counter =0;
    return objectListIterator(NULL);
  }
  return NULL;
}

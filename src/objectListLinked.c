#include "object.h"
#include "translation.h"
#include <stdlib.h>

typedef struct objectList{
  Object* obj;
  struct objectList *next;
}objectList;

Object* objectListIterator(objectList* list){
  static objectList* cur = NULL;
  if(list != NULL){
    cur = list->next;
    return list->obj;
  }
  if(cur == NULL){
    return NULL;
  }
  Object* ret = cur->obj;
  cur = cur->next;
  return ret;
}

objectList *createObjectList(){
  objectList * node = (objectList*)malloc(sizeof(objectList));
  if(node == NULL){
    return NULL;
  }
  node->next = NULL;
  node->obj = NULL;
  return node;
}

int addObjectToList(objectList *list, Object * newObj){
  if(list->obj == NULL){
    list->obj = newObj;
    return 0;
  }
  objectList* newNode = createObjectList();
  if(newNode == NULL){
    return 1;
  }
  newNode->next = list->next;
  newNode->obj = newObj;
  list->next = newNode;
  return 0;
}

int deleteObjectFromList(objectList * node, Object * obj){ 
  objectList* prev = NULL;
  while(node->obj != obj){
    if(node->next != NULL){
      prev = node;
      node = node->next;
    }else{
      deleteObject(obj);
      return 1;
    }
  }
  prev->next = node->next;
  deleteObject(obj);
  free(node);
  return 0;
}

void deleteObjectList(objectList* list){
  objectList* next;
  while(list!=NULL){
    deleteObject(list->obj);
    next = list->next;
    free(list);
    list=next;
  }
}

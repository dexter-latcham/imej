#ifndef IMEJ_OBJECT
#define IMEJ_OBJECT
#include "colour.h"
#include "vectors.h"


#define VERTMIN -32767
#define VERTMAX 32767

typedef struct{
  uint16_t a,b,c;
}Face;

typedef struct{
  int16_t x,y,z;
}vec3Int;


typedef struct transform transform;

typedef struct Object{
  vec3Int * vertices;
  int vertexCount;
  Face * faces;
  int faceCount;
  transform* transformation;
  void (*update)(struct Object*, int *);
  int *refCount;
  colType col;
}Object;


Object * createObject(int vertexCount,int faceCount);
void deleteObject(Object* obj);
Object* cloneObject(Object* obj);


typedef struct objectList objectList;

objectList *createObjectList();
int addObjectToList(objectList *Silo, Object * obj);
int deleteObjectFromList(objectList * Silo, Object * obj);
void deleteObjectList(objectList* silo);

Object* objectListIterator(objectList* silo);

Object * loadObjectFromDisk(char* filepath);
Object * loadObjectWithColour(char* filepath,colType faceColour);
#endif

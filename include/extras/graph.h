#include "../include/colour.h"
#include "../include/object.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//takes values from 0 to 1
//scales to 0 to vertmax

void updateGraph(Object* graph, int16_t* newInfo, int width, int height){
  for(int i=0; i<width; i++){
    for(int j=0;j<height;j++){
      Face f1 = graph->faces[(i+width*j)*2];
      Face f2 = graph->faces[(i+width*j)*2+1];
      graph->vertices[f1.a].y = newInfo[i*width+j];
      graph->vertices[f1.b].y = newInfo[i*width+j];
      graph->vertices[f1.c].y = newInfo[i*width+j];
      graph->vertices[f2.c].y = newInfo[i*width+j];
    }
  }
}

vec3Int getScaledVec(float x, int16_t y, float z, int xMax, int zMax){
  int16_t xInt = (int16_t)((x*VERTMAX)/xMax);
  int16_t zInt = (int16_t)((z*VERTMAX)/zMax);
  return (vec3Int){.x=xInt,.y=y,.z=zInt};
}

Object* getGraph(uint16_t* info, int width, int height){
  int w = width;
  int h = height;
  int maxVertices = (4*w*h)+(w)*2 + (h)*2;
  int maxFaces = ((w*h)+w*(h+1)+h*(w+1))*2;
  Object*obj = createObject(maxVertices, maxFaces);
  obj->col=IMEJ_COL_DEFAULT;
  //info ranges from 0 to 1
  //x coord has range 0 to w+1
  //y coord has range 0 to h+1
  for(int x=0;x<w;x++){
    for(int y=0;y<h;y++){
      vec3Int p1 = getScaledVec(x,info[x*width+y],y,w+1,h+1);
      vec3Int p2 = getScaledVec(x+1,info[x*width+y],y,w+1,h+1);
      vec3Int p3 = getScaledVec(x,info[x*width+y],y+1,w+1,h+1);
      vec3Int p4 = getScaledVec(x+1,info[x*width+y],y+1,w+1,h+1);
      uint16_t p1Ind = (x+y*w)*4;
      uint16_t p2Ind = (x+y*w)*4+1;
      uint16_t p3Ind = (x+y*w)*4+2;
      uint16_t p4Ind = (x+y*w)*4+3;
      obj->vertices[p1Ind] = p1;
      obj->vertices[p2Ind] = p2;
      obj->vertices[p3Ind] = p3;
      obj->vertices[p4Ind] = p4;
      obj->faces[(x+y*w)*2] = (Face){(uint16_t)(p1Ind),(uint16_t)(p3Ind),(uint16_t)(p2Ind)};
      obj->faces[(x+y*w)*2+1] = (Face){(uint16_t)(p2Ind),(uint16_t)(p3Ind),(uint16_t)(p4Ind)};
    }
  }
  int faceCount = w*h*2;
  int vertCount = w*h*4;
  //adding base vertex in the x direction
  for(int x=0;x<w+1;x++){
    vec3Int pNear = getScaledVec(x,0,0,w+1,h+1);
    vec3Int pFar = getScaledVec(x,0,h,w+1,h+1);
    obj->vertices[vertCount+x*2] = pNear;
    obj->vertices[vertCount+(x*2)+1] = pFar;
  }

  //painting sides on x plane
  for(int x=0;x<w;x++){
    Face topFace = obj->faces[x*2];
    uint16_t p1Ind = topFace.a; //tl
    uint16_t p2Ind = topFace.c; //tr
    uint16_t p3Ind = (vertCount + x*2); // bl
    uint16_t p4Ind = (vertCount + (x+1)*2); // br
    obj->faces[faceCount++]= (Face){p1Ind,p2Ind,p3Ind};
    obj->faces[faceCount++]= (Face){p2Ind,p4Ind,p3Ind};


    topFace = obj->faces[(x+(h-1)*w)*2+1];
    p1Ind = topFace.b; //tl
    p2Ind = topFace.c; //tr
    p3Ind = (vertCount + x*2)+1; // bl
    p4Ind = (vertCount + (x+1)*2)+1; // br
    obj->faces[faceCount++]= (Face){p3Ind,p2Ind,p1Ind};
    obj->faces[faceCount++]= (Face){p3Ind,p4Ind,p2Ind};
  }
  vertCount = w*h*4 + (w+1)*2;

  for(int y=0;y<h-1;y++){
    vec3Int pNear = getScaledVec(0,0,y+1,w+1,h+1);
    vec3Int pFar = getScaledVec(w,0,y+1,w+1,h+1);
    obj->vertices[vertCount+y*2] = pNear;
    obj->vertices[vertCount+(y*2)+1] = pFar;
  }

  //edge case
  // 0,0,0 to 0,0,1
  Face topFace = obj->faces[0];
  uint16_t p1Ind = topFace.a; //tl
  uint16_t p2Ind = topFace.b;//tr
  uint16_t p3Ind = w*h*4; //0,0,0
  uint16_t p4Ind = vertCount;
  obj->faces[faceCount++]= (Face){p3Ind,p2Ind,p1Ind};
  obj->faces[faceCount++]= (Face){p2Ind,p3Ind,p4Ind};

  // 0,0,h-1 to 0,0,h
  topFace = obj->faces[(h-1)*2*w];
  p1Ind = topFace.a; //tl
  p2Ind = topFace.b;//tr
  p3Ind = w*h*4 +1; //0,0,h
  p4Ind = vertCount +(h-2)*2;
  obj->faces[faceCount++]= (Face){p4Ind,p2Ind,p1Ind};
  obj->faces[faceCount++]= (Face){p4Ind,p3Ind,p2Ind};

  //w,0,0 to w,0,1
  topFace = obj->faces[(w-1)*2+1];
  p1Ind = topFace.a; //tl //w,t,0
  p2Ind = topFace.c;//tr  // w,t,1
  p3Ind = w*h*4 +(w*2); //w,0,0
  p4Ind = vertCount + 1;  //w,0,1
  obj->faces[faceCount++]= (Face){p1Ind,p2Ind,p3Ind};
  obj->faces[faceCount++]= (Face){p4Ind,p3Ind,p2Ind};

  //w,0,h to w,0,h-1
  topFace = obj->faces[((w-1)+(h-1)*w)*2+1];
  p1Ind = topFace.a; //tl //w,t,h-1
  p2Ind = topFace.c;//tr  // w,t,h
  p3Ind = w*h*4 +(w*2)+1; //w,0,h
  p4Ind = vertCount + ((h-2)*2)+1;  //w,0,h-1
  obj->faces[faceCount++]= (Face){p1Ind,p2Ind,p4Ind};
  obj->faces[faceCount++]= (Face){p2Ind,p3Ind,p4Ind};

  //sides in y plane
  for(int y=0;y<h-2;y++){
    Face topFace = obj->faces[((y+1)*w)*2];
    uint16_t p1Ind = topFace.a; //tl
    uint16_t p2Ind = topFace.b;//tr
    uint16_t p3Ind = (vertCount + (y)*2);
    uint16_t p4Ind = (vertCount + (y+1)*2);
    obj->faces[faceCount++]= (Face){p3Ind,p2Ind,p1Ind};
    obj->faces[faceCount++]= (Face){p3Ind,p4Ind,p2Ind};
    topFace = obj->faces[((w-1)+(y+1)*w)*2+1];
    p1Ind = topFace.a;
    p2Ind = topFace.c;
    p3Ind = (vertCount + (y)*2)+1;
    p4Ind = (vertCount + (y+1)*2)+1;
    obj->faces[faceCount++]= (Face){p1Ind,p2Ind,p3Ind};
    obj->faces[faceCount++]= (Face){p2Ind,p4Ind,p3Ind};
  }

  for(int x=0;x<w;x++){
    for(int y=0;y<h-1;y++){
      Face thisFace = obj->faces[(x+y*w)*2+1];
      Face nextFace = obj->faces[(x+(y+1)*w)*2];
      uint16_t tlIndex = thisFace.b; //x,y+1
      uint16_t trIndex = thisFace.c; //x+1,y+1
      uint16_t blIndex = nextFace.a; //x,y
      uint16_t brIndex = nextFace.c; //x+1,y
      obj->faces[(faceCount++)] = (Face){blIndex,trIndex,tlIndex};
      obj->faces[(faceCount++)] = (Face){blIndex,brIndex,trIndex};
    }
  }
  for(int x=0;x<w-1;x++){
    for(int y=0;y<h;y++){
      Face leftFace = obj->faces[(x+y*w)*2+1];
      Face rightFace = obj->faces[((x+1)+y*w)*2];
      uint16_t tlIndex = leftFace.c;
      uint16_t trIndex = leftFace.a;
      uint16_t blIndex = rightFace.b;
      uint16_t brIndex = rightFace.a;
      obj->faces[(faceCount++)] = (Face){blIndex,trIndex,tlIndex};
      obj->faces[(faceCount++)] = (Face){blIndex,brIndex,trIndex};
    }
  }
  obj->vertexCount = maxVertices;
  obj->faceCount = faceCount;
  return obj;
}

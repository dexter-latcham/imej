#include "projection.h"
#include "translation.h"
#include "matrix.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef DEBUG
#define IMEJ_DEBuG_LOG(string) printf(string) 
#else
#define IMEJ_DEBuG_LOG(string)
#endif

vec3 clipLineZ(vec3 p1, vec3 p2, float zDepth){
  float t = (zDepth - p1.z) / (p2.z - p1.z);
  vec3 ret;
  ret.x = p1.x + t * (p2.x - p1.x);
  ret.y = p1.y + t * (p2.y - p1.y);
  ret.z = zDepth;
  return ret;
}

void zCulling(vec3* points, int *pointsNum, float zDepth){
  vec3 newPoints[10] = {0};
  int newNum =0;
  for(int i=0; i< *pointsNum; i++){
    int k = (i+1)%*pointsNum;
    vec3 point1 = points[i];
    vec3 point2 = points[k];
    if(point1.z > zDepth && point2.z >0){
      newPoints[newNum++]=point2;
    }else if(point1.z <= zDepth && point2.z > zDepth){
      newPoints[newNum++] = clipLineZ(point1, point2, zDepth);
      newPoints[newNum++] = point2;
    }else if(point1.z > zDepth && point2.z <= zDepth){
      newPoints[newNum++] = clipLineZ(point1, point2, zDepth);
    }
  }

  *pointsNum = newNum;
  for(int i=0; i<*pointsNum; i++){
    points[i] = newPoints[i];
  }
}

void hogs3D(vec3* points, int *pointsNum, int maxPoints, vec3 plane, vec3 planeNormal){
  static vec3 newPoints[10] = {0};
  int newNum =0;
  int k;
  vec3 point1,point2;
  float p1ToPlaneDist,p2ToPlaneDist;
  vec3 intersection;
  for(int i=0; i< *pointsNum && i<10; i++){
    k = (i+1)%*pointsNum;
    point1 = points[i];
    point2 = points[k];
    p1ToPlaneDist = distanceToPlane(point1,plane, planeNormal); 
    p2ToPlaneDist = distanceToPlane(point2,plane, planeNormal); 
    if(p1ToPlaneDist <0 && p2ToPlaneDist <0){
      newPoints[newNum++]=point2;
    }else if(p1ToPlaneDist >=0 && p2ToPlaneDist <0){
      intersection = lineIntersectsPlane(point1, point2, plane, planeNormal);
      newPoints[newNum++] = intersection;
      newPoints[newNum++] = point2;
    }else if(p1ToPlaneDist <0 && p2ToPlaneDist >=0){
      intersection = lineIntersectsPlane(point1, point2, plane, planeNormal);
      newPoints[newNum++] = intersection;
    }
  }
  *pointsNum = newNum;
  for(int i=0; i<*pointsNum && i<maxPoints; i++){
    points[i] = newPoints[i];
  }
}

void drawClippedFace(vec3 p1, vec3 p2, vec3 p3, Rasterizer*r, colType col, Camera* cam){
  static vec3 points[10];
  points[0] = p1;
  points[1] = p2;
  points [2] = p3;
  int pointsCount = 3;
  float left = cam->projectedBoundsLeft;
  float right = cam->projectedBoundsRight;
  float base = cam->projectedBoundsBase;
  float top = cam->projectedBoundsTop;
  float near =1;
  float zMax = ZBUFFERLIMIT;

  int doXClip=0;
  int doYClip=0;
  int doZClip=0;
  for(int i=0;i<pointsCount;i++){
    if(points[i].x <left || points[i].x >right){
      doXClip=1;
    }
    if(points[i].y <base || points[i].y >top){
      doYClip=1;
    }
    if(points[i].z < near || points[i].z >zMax){
      doZClip=1;
    }
    if(doXClip && doYClip && doZClip){
      break;
    }
  }
  if(doXClip){
    hogs3D(points, &pointsCount, 10,(vec3){left,base,0},(vec3){-1,0,0});
    hogs3D(points, &pointsCount, 10,(vec3){right,base,0},(vec3){1,0,0});
  }
  if(doYClip){
    hogs3D(points, &pointsCount, 10,(vec3){left,top,0},(vec3){0,1,0});
    hogs3D(points, &pointsCount, 10,(vec3){left,base,0},(vec3){0,-1,0});
  }

  if(doZClip){
    hogs3D(points, &pointsCount, 10,(vec3){left,base,zMax},(vec3){0,0,-1});
    hogs3D(points, &pointsCount, 10,(vec3){left,base,near},(vec3){0,0,1});
  }

  if(pointsCount < 3){
    return;
  }
  vec3 v1 = points[0];
  for(int i=1; i< pointsCount-1; i++){
    vec3 v2 = points[i];
    vec3 v3 = points[i+1];
    drawTriangle(r,v1.x,v1.y,v1.z,v2.x,v2.y,v2.z,v3.x,v3.y,v3.z,col);
  }
}

void projectObject(Camera* cam,Rasterizer* r,vec3 lightSource,Object* obj){
  static const float VERTDIV = 1.0f/VERTMAX;
  mat4* objToWorld = getObjectTransformationMatrix(obj);
  mat4* toCamera = getWorldToCameraMatrix(cam);

  mat4 objToCamera;
  multmat4mat4Ptr(&objToCamera,toCamera,objToWorld);
  float xScale = objToCamera.m[0][0]+objToCamera.m[0][1]+objToCamera.m[0][2];
  float yScale = objToCamera.m[1][0]+objToCamera.m[1][1]+objToCamera.m[1][2];
  float zScale = objToCamera.m[2][0]+objToCamera.m[2][1]+objToCamera.m[2][2];

  float scaleMax = xScale;
  if(yScale > scaleMax){
    scaleMax = yScale;
  }
  if(zScale > scaleMax){
    scaleMax = zScale;
  }

  static const float multiplier = 1.732050801f;
  scaleMax = scaleMax * multiplier;

  vec3 objOffset = {
    .x=objToCamera.m[0][3],
    .y=objToCamera.m[1][3],
    .z=objToCamera.m[2][3],
  };
  
  //near bounds check
  if((objOffset.z + scaleMax) < cam->frustumNear){
    IMEJ_DEBuG_LOG("NEAR\n");
    return;
  }
  
  //left frustum check
  float d = distanceToPlane(objOffset,(vec3){-scaleMax,0,-scaleMax},cam->frustumLeftNormal); 
  if(d > 0){
    IMEJ_DEBuG_LOG("LEFT\n");
    return; // object is left of the view frustum
  }

  d = distanceToPlane(objOffset,(vec3){scaleMax,0,-scaleMax},cam->frustumRightNormal); 
  if(d>0){
    IMEJ_DEBuG_LOG("RIGHT\n");
    return; // object is right of view frustum
  }

  d = distanceToPlane(objOffset,(vec3){0,scaleMax,-scaleMax},cam->frustumTopNormal); 
  if(d>0){
    IMEJ_DEBuG_LOG("ABOVE\n");
    return; // object is above the frustum
  }
  d = distanceToPlane(objOffset,(vec3){0,-scaleMax,-scaleMax},cam->frustumBaseNormal); 
  if(d>0){
    IMEJ_DEBuG_LOG("BELOW\n");
    return; // object is below the frustum
  }
  IMEJ_DEBuG_LOG("PAINTED\n");

  Face f;
  vec3Int p1Int,p2Int,p3Int;
  vec3 p1,p2,p3,norm;
  colType objectColour = obj->col;
  float percent;
  for(int i=0;i<obj->faceCount;i++){
    f = obj->faces[i];
    p1Int=obj->vertices[f.a];
    p2Int=obj->vertices[f.b];
    p3Int=obj->vertices[f.c];

    p1 = (vec3){.x=(float)p1Int.x*VERTDIV,.y=(float)p1Int.y*VERTDIV,.z=(float)p1Int.z*VERTDIV};
    p2 = (vec3){.x=(float)p2Int.x*VERTDIV,.y=(float)p2Int.y*VERTDIV,.z=(float)p2Int.z*VERTDIV};
    p3 = (vec3){.x=(float)p3Int.x*VERTDIV,.y=(float)p3Int.y*VERTDIV,.z=(float)p3Int.z*VERTDIV};

    p1 = multMat4Vec3(*objToWorld,p1);
    p1 = multMat4Vec3(*toCamera,p1);
    p1 = multMat4Vec3(cam->perspectiveMat,p1);

    p2 = multMat4Vec3(*objToWorld,p2);
    p2 = multMat4Vec3(*toCamera,p2);
    p2 = multMat4Vec3(cam->perspectiveMat,p2);

    p3 = multMat4Vec3(*objToWorld,p3);
    p3 = multMat4Vec3(*toCamera,p3);
    p3 = multMat4Vec3(cam->perspectiveMat,p3);
    norm = paneNormal(p1,p2,p3);
    if(vec3DotProduct(norm,vec3Normalised(p1))<0){
      continue;
    }

    percent = (1-vec3DotProduct(lightSource, norm)) *0.5f;
    colType newCol = SCALE_COLOUR( objectColour , percent );
    if(
      (p1.x <cam->projectedBoundsLeft) || (p1.x > cam->projectedBoundsRight)||
      (p1.y <cam->projectedBoundsBase) || (p1.y > cam->projectedBoundsTop)||
      (p1.z <0) || (p1.z > ZBUFFERLIMIT)){
      drawClippedFace(p1,p2,p3,r,newCol,cam);
      continue;
    }
    if(
      (p2.x <cam->projectedBoundsLeft) || (p2.x > cam->projectedBoundsRight)||
      (p2.y <cam->projectedBoundsBase) || (p2.y > cam->projectedBoundsTop)||
      (p2.z <0) || (p2.z > ZBUFFERLIMIT)){
      drawClippedFace(p1,p2,p3,r,newCol,cam);
      continue;
    }
    if(
      (p3.x <cam->projectedBoundsLeft) || (p3.x > cam->projectedBoundsRight)||
      (p3.y <cam->projectedBoundsBase) || (p3.y > cam->projectedBoundsTop)||
      (p3.z <0) || (p3.z > ZBUFFERLIMIT)){
      drawClippedFace(p1,p2,p3,r,newCol,cam);
      continue;
    }
    drawTriangle(r,p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z,newCol);
  }
}



// void drawClippedFace(vec3 p1, vec3 p2, vec3 p3, Rasterizer*r, colType col, Camera* cam){
//   float xMin = cam->projectedBoundsLeft+1;
//   float xMax = cam->projectedBoundsRight-1;
//   float yMin = cam->projectedBoundsBase+1;
//   float yMax = cam->projectedBoundsTop-1;
//   float zMin = 1;
//   float zMax = ZBUFFERLIMIT-1;
//   vec3 pointsBufferA[20];
//   pointsBufferA[0]=p1;
//   pointsBufferA[1]=p2;
//   pointsBufferA[2]=p3;
//   vec3 pointsBufferB[20];
//   int bufferBCount =0;
//   int bufferACount =3;
//   int k;
//   vec3 point1,point2;
//   int p1Clip,p2Clip;
//   vec3 tmp;
//   float scale;

//   for(int i=0;i<bufferACount;i++){
//     k = (i+1)%bufferACount;
//     point1 = pointsBufferA[i];
//     point2 = pointsBufferA[k];
//     p1Clip = (point1.z >= zMin) && (point1.z <= zMax);
//     p2Clip = (point2.z >= zMin) && (point2.z <= zMax);
//     if(p1Clip && p2Clip){
//       pointsBufferB[bufferBCount++]=point2;
//     }else if(p2Clip){
//       tmp = point1;
//       if(tmp.z < zMin){
//         scale = (zMin-tmp.z)/(point2.z- tmp.z);
//       }else{
//         scale = (zMax-tmp.z)/(point2.z- tmp.z);
//       }
//       tmp.x = (point2.x-tmp.x)*scale + tmp.x;
//       tmp.y = (point2.y-tmp.y)*scale + tmp.y;
//       tmp.z = (point2.z-tmp.z)*scale + tmp.z;
//       pointsBufferB[bufferBCount++]=tmp;
//       pointsBufferB[bufferBCount++]=point2;
//     }else if(p1Clip){
//       tmp = point2;
//       if(tmp.z < zMin){
//         scale = (zMin-tmp.z)/(point1.z- tmp.z);
//       }else{
//         scale = (zMax-tmp.z)/(point1.z- tmp.z);
//       }
//       tmp.x = (point1.x-tmp.x)*scale + tmp.x;
//       tmp.y = (point1.y-tmp.y)*scale + tmp.y;
//       tmp.z = (point1.z-tmp.z)*scale + tmp.z;
//       pointsBufferB[bufferBCount++]=tmp;
//     }
//   }
//   if(bufferBCount <3){
//     return;
//   }
//   bufferACount=0;
//   for(int i=0;i<bufferBCount;i++){
//     k = (i+1)%bufferBCount;
//     point1 = pointsBufferB[i];
//     point2 = pointsBufferB[k];
//     p1Clip = (point1.y >= yMin) && (point1.y <= yMax);
//     p2Clip = (point2.y >= yMin) && (point2.y <= yMax);
//     if(p1Clip && p2Clip){
//       pointsBufferA[bufferACount++]=point2;
//     }else if(p2Clip){
//       tmp = point1;
//       if(tmp.y < yMin){
//         scale = (yMin-tmp.y)/(point2.y- tmp.y);
//       }else{
//         scale = (yMax-tmp.y)/(point2.y- tmp.y);
//       }
//       tmp.x = (point2.x-tmp.x)*scale + tmp.x;
//       tmp.y = (point2.y-tmp.y)*scale + tmp.y;
//       tmp.z = (point2.z-tmp.z)*scale + tmp.z;
//       pointsBufferA[bufferACount++]=tmp;
//       pointsBufferA[bufferACount++]=point2;
//     }else if(p1Clip){
//       tmp = point2;
//       if(tmp.y < yMin){
//         scale = (yMin-tmp.y)/(point1.y- tmp.y);
//       }else{
//         scale = (yMax-tmp.y)/(point1.y- tmp.y);
//       }
//       tmp.x = (point1.x-tmp.x)*scale + tmp.x;
//       tmp.y = (point1.y-tmp.y)*scale + tmp.y;
//       tmp.z = (point1.z-tmp.z)*scale + tmp.z;
//       pointsBufferA[bufferACount++]=tmp;
//     }
//   }

//   if(bufferACount <3){
//     return;
//   }
//   bufferBCount=0;
//   for(int i=0;i<bufferACount;i++){
//     k = (i+1)%bufferACount;
//     point1 = pointsBufferA[i];
//     point2 = pointsBufferA[k];
//     p1Clip = (point1.x >= xMin) && (point1.x <= xMax);
//     p2Clip = (point2.x >= xMin) && (point2.x <= xMax);
//     if(p1Clip && p2Clip){
//       pointsBufferB[bufferBCount++]=point2;
//     }else if(p2Clip){
//       tmp = point1;
//       if(tmp.x < xMin){
//         scale = (xMin-tmp.x)/(point2.x- tmp.x);
//       }else{
//         scale = (xMax-tmp.x)/(point2.x- tmp.x);
//       }
//       tmp.x = (point2.x-tmp.x)*scale + tmp.x;
//       tmp.y = (point2.y-tmp.y)*scale + tmp.y;
//       tmp.z = (point2.z-tmp.z)*scale + tmp.z;
//       pointsBufferB[bufferBCount++]=tmp;
//       pointsBufferB[bufferBCount++]=point2;
//     }else if(p1Clip){
//       tmp = point2;
//       if(tmp.x < xMin){
//         scale = (xMin-tmp.x)/(point1.x- tmp.x);
//       }else{
//         scale = (xMax-tmp.x)/(point1.x- tmp.x);
//       }

//       tmp.x = (point1.x-tmp.x)*scale + tmp.x;
//       tmp.y = (point1.y-tmp.y)*scale + tmp.y;
//       tmp.z = (point1.z-tmp.z)*scale + tmp.z;
//       pointsBufferB[bufferBCount++]=tmp;
//     }
//   }

//   if(bufferBCount <3){
//     return;
//   }
//   vec3 v1 = pointsBufferB[0];
//   for(int i=1; i< bufferBCount-1; i++){
//     vec3 v2 = pointsBufferB[i];
//     vec3 v3 = pointsBufferB[i+1];
//     drawTriangle(r,v1.x,v1.y,v1.z,v2.x,v2.y,v2.z,v3.x,v3.y,v3.z,col);
//   }
// }

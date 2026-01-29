#include "input.h"
#include "translation.h"
#include <stdlib.h>
#include <stdio.h>

#define FORWARDS_SPEED_CAP 100
#define SIDEWAYS_SPEED_CAP 100
#define FORWARDS_SPEED_INCREMENT 20
#define SIDEWYS_SPEED_INCREMENT  10
#define ROTATE_INCREMENT 0.017 //1 degree

typedef enum {
  FORWARDS = 'w',
  BACKWARDS = 's',
  RIGHT = 'a',
  LEFT = 'd',
  LOOKLEFT = 'h',
  LOOKRIGHT = 'l',
  LOOKUP = 'k',
  LOOKDOWN = 'j'
}keypress;

void handleKeypress(struct InputHandler* input, char c){
  keypress kp = (keypress)c;
  switch(kp){
    case FORWARDS:
      input->movementSpeed.z+=FORWARDS_SPEED_INCREMENT;
      if(input->movementSpeed.z > FORWARDS_SPEED_CAP){
        input->movementSpeed.z = FORWARDS_SPEED_CAP;
      }
      break;
    case BACKWARDS:
      input->movementSpeed.z-=FORWARDS_SPEED_INCREMENT;
      if(input->movementSpeed.z < -FORWARDS_SPEED_CAP){
        input->movementSpeed.z = -FORWARDS_SPEED_CAP;
      }
      break;
    case RIGHT:
      input->movementSpeed.x+=SIDEWYS_SPEED_INCREMENT;
      if(input->movementSpeed.x > SIDEWAYS_SPEED_CAP){
        input->movementSpeed.x = SIDEWAYS_SPEED_CAP;
      }
      break;
    case LEFT:
      input->movementSpeed.x-=SIDEWYS_SPEED_INCREMENT;
      if(input->movementSpeed.x < -SIDEWAYS_SPEED_CAP){
        input->movementSpeed.x = -SIDEWAYS_SPEED_CAP;
      }
      break;
    case LOOKLEFT:
      rotateCamera(input->cam,-ROTATE_INCREMENT,0);
      break;
    case LOOKRIGHT:
      rotateCamera(input->cam,ROTATE_INCREMENT,0);
      break;
    case LOOKUP:
      rotateCamera(input->cam,0,ROTATE_INCREMENT);
      break;
    case LOOKDOWN:
      rotateCamera(input->cam,0,-ROTATE_INCREMENT);
      break;
  }
}


void handleMouseClick(struct InputHandler* input, float xPosition, float yPosition){
  input->xClickPosition=xPosition;
  input->yClickPosition=yPosition;
  input->isClicked=1;
}

void handleMouseMove(struct InputHandler* input, float xPosition, float yPosition){
  if(input->isClicked){
    float Xangle = (input->xClickPosition - xPosition)*0.5;
    float Yangle = (input->yClickPosition - yPosition)*0.5;
    rotateCameraDegrees(input->cam,-Xangle,Yangle);
    input->xClickPosition=xPosition;
    input->yClickPosition=yPosition;
  }
}

void handleMouseRelease(struct InputHandler* input, float xPosition, float yPosition){
  input->isClicked=0;
}


void tick(struct InputHandler* inHandler, float msSinceLastTick){
  if(inHandler->movementSpeed.x == 0 && inHandler->movementSpeed.y ==0 && inHandler->movementSpeed.z ==0){
    return;
  }
  Camera* camera = inHandler->cam;
  
  //move more if ms is larger
  vec3 move = vec3MultScalar(inHandler->movementSpeed,0.0001*msSinceLastTick);
  // printf("x: %f\ny: %f\nz: %f\n",move.x,move.y,move.z);
  moveCameraForwards(camera,move.x,move.y,move.z);
  //decrease speed, decrease more if ms is laeger
  inHandler->movementSpeed = vec3SubVec3(inHandler->movementSpeed,vec3MultScalar(move,30));
  return; 
}

struct InputHandler* createInputHandler(Camera*c){
  struct InputHandler* inHandler = (struct InputHandler*) malloc(sizeof(struct InputHandler));
  if(inHandler == NULL){
    return NULL;
  }
  inHandler->movementSpeed =(vec3){.x=0,.y=0,.z=0};
  inHandler->cam = c;
  inHandler->xClickPosition =0;
  inHandler->yClickPosition =0;
  inHandler->isClicked =0;
  inHandler->tick = tick;
  inHandler->handleKeypress = handleKeypress;
  inHandler->handleMouseMove = handleMouseMove;
  inHandler->handleMouseClick = handleMouseClick;
  inHandler->handleMouseRelease = handleMouseRelease;
  return inHandler;
}


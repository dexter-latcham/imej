#ifndef IMEJ_INPUT
#define IMEJ_INPUT
#include "camera.h"

struct InputHandler{
  Camera* cam;
  vec3 movementSpeed;
  float xClickPosition;
  float yClickPosition;
  int isClicked;
  void (*tick)(struct InputHandler*, float msSinceLastTick);
  void (*handleKeypress)(struct InputHandler*, char);
  void (*handleMouseClick)(struct InputHandler*,float, float);
  void (*handleMouseMove)(struct InputHandler*, float, float);
  void (*handleMouseRelease)(struct InputHandler*, float, float);
};

struct InputHandler* createInputHandler(Camera*c);
void tick(struct InputHandler* inHadle, float msSinceLastTick);
#endif

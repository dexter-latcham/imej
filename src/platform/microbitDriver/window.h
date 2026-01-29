#ifndef IMEJ_DISPLAY
#define IMEJ_DISPLAY
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include <stdint.h>
#include "raster.h"
#include "scene.h"

struct data;
EXTERNC struct data *openWindow(Canvas *c);
EXTERNC void  renderFrame(struct data *context,Canvas* c);
EXTERNC void  closeWindow(struct data* context);
EXTERNC void startRenderingLoop(struct data* context, Scene* s);
#undef EXTERNC
#endif 

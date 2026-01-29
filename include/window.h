#ifndef IMEJ_DISPLAY
#define IMEJ_DISPLAY
typedef struct Scene Scene;
#ifdef __cplusplus
extern "C" {
#endif

int openWindow(Scene* s);
void renderFrame(Scene* s);
void startRenderingLoop(Scene* s);
void closeWindow();

#ifdef __cplusplus
}
#endif
#endif 

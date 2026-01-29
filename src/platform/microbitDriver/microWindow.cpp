#include "MicroBit.h"
#include "Adafruit_ST7735.h"
#include "../include/window.h"
#include <stdlib.h>
extern "C" {
#include "../include/raster.h"
#include "../include/camera.h"
#include "../include/input.h"
#include "../include/scene.h"
}

typedef struct displayHandler{
  Adafruit_ST7735 * lcd;
  colType* framebuffer;
  int width;
  int height;
}displayHandler;

#define LCD_PIN_CS      2
#define LCD_PIN_DC      1
#define LCD_PIN_RST     0
#define LCD_PIN_MOSI    15
#define LCD_PIN_MISO    14
#define LCD_PIN_SCLK    13

static displayHandler* disp = NULL;

MicroBit uBit;
void startRenderingLoop(Scene* s){
  long lastFrame = uBit.systemTime();
  long currentFrame = 0;
  #ifdef IMEJ_DO_INPUT
  struct InputHandler* input = getSceneInputHandler(s);
  #endif
  while(1){
    currentFrame = uBit.systemTime();
    renderFrame(s);
    renderScene(s);
#ifdef IMEJ_DO_INPUT
    input->tick(input,(float)(currentFrame-lastFrame)*1000);
    if(uBit.serial.isReadable()){
      char inputChar = uBit.serial.read();
      input->handleKeypress(input,inputChar);
    }
#endif
    lastFrame = currentFrame;
  }
}

void renderFrame(Scene* s){
  disp->lcd->sendData(0, 0, disp->width, disp->height, (uint8_t*)disp->framebuffer);
}

void  closeWindow(){
  if(disp!=NULL){
    delete disp->lcd;
    free(disp);
  }
}


int openWindow(Scene* s){
  extern MicroBit uBit;
  uBit.init();
  uBit.sleep(200);
  disp = (displayHandler* ) malloc(sizeof(displayHandler));
  if(disp == NULL){
    return 0;
  }
  Adafruit_ST7735 *lcd = new Adafruit_ST7735(LCD_PIN_CS, LCD_PIN_DC, LCD_PIN_RST, LCD_PIN_MOSI, LCD_PIN_MISO, LCD_PIN_SCLK);
  lcd->initR(INITR_GREENTAB);

  Rasterizer* r = getRasterizer(s);
  int width = getFrameWidth(r);
  int height = getFrameHeight(r);
  disp->lcd = lcd;
  disp->width=width;
  disp->height=height;
  disp->framebuffer = getFramebuffer(r);
  return 1;
}


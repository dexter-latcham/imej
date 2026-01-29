/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include "Adafruit_ST77xx.h"

#define SPI_DEFAULT_FREQ 64000000 ///< Default SPI data clock frequency

static NRF52SPI *spi = NULL;


/**************************************************************************/
/*!
    @brief  Instantiate Adafruit ST77XX driver with software SPI
    @param  w     Display width in pixels at default rotation setting (0)
    @param  h     Display height in pixels at default rotation setting (0)
    @param  cs    Chip select pin #
    @param  dc    Data/Command pin #
    @param  mosi  SPI MOSI pin #
    @param  sclk  SPI Clock pin #
    @param  rst   Reset pin # (optional, pass -1 if unused)
    @param  miso  SPI MISO pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
Adafruit_ST77xx::Adafruit_ST77xx(uint16_t w, uint16_t h, int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST, int8_t _MISO)
{
  _width = w;
  _height = h;

  _xstart = 0;
  _ystart = 0;

  CS = &uBit.io.pin[_CS];
  RST = &uBit.io.pin[_RST];
  DC = &uBit.io.pin[_DC];

  spi = new NRF52SPI(uBit.io.pin[_MOSI], uBit.io.pin[_MISO], uBit.io.pin[_SCLK]); 
}



/**************************************************************************/
/*!
    @brief  Companion code to the initiliazation tables. Reads and issues
            a series of LCD commands stored in PROGMEM byte array.
    @param  addr  Flash memory array with commands and data to send
*/
/**************************************************************************/
void Adafruit_ST77xx::displayInit(const uint8_t *addr) {

  uint8_t numCommands, cmd, numArgs;
  uint16_t ms;

  numCommands = *addr++; // Number of commands to follow
  while (numCommands--) {              // For each command...
    cmd = *addr++;       // Read command
    numArgs = *addr++;   // Number of args to follow
    ms = numArgs & ST_CMD_DELAY;       // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;          // Mask out delay bit
    sendCommand(cmd, addr, numArgs);
    addr += numArgs;

    if (ms) {
      ms = *addr++; // Read post-command delay time (ms)
      if (ms == 255)
        ms = 500; // If 255, delay for 500 ms
      target_wait(ms);
    }
  }
}

/**************************************************************************/
/*!
    @brief  Initialize ST77xx chip. Connects to the ST77XX over SPI and
            sends initialization procedure commands
    @param  freq  Desired SPI clock frequency
*/
/**************************************************************************/
void Adafruit_ST77xx::begin(uint32_t freq) {
  if (!freq) {
    freq = SPI_DEFAULT_FREQ;
  }
  _freq = freq;

  invertOnCommand = ST77XX_INVON;
  invertOffCommand = ST77XX_INVOFF;

  initSPI(freq, spiMode);
}

/**************************************************************************/
/*!
    @brief  Initialization code common to all ST77XX displays
    @param  cmdList  Flash memory array with commands and data to send
*/
/**************************************************************************/
void Adafruit_ST77xx::commonInit(const uint8_t *cmdList) {
  begin();

  if (cmdList) {
    displayInit(cmdList);
  }
}

/**************************************************************************/
/*!
  @brief  SPI displays set an address window rectangle for blitting pixels
  @param  x  Top left corner x coordinate
  @param  y  Top left corner x coordinate
  @param  w  Width of window
  @param  h  Height of window
*/
/**************************************************************************/
void Adafruit_ST77xx::setAddrWindow(uint16_t x, uint16_t y, uint16_t w,
                                    uint16_t h) {
  x += _xstart;
  y += _ystart;
  uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
  uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);

  CS->setDigitalValue(0);
  writeCommand(ST77XX_CASET); // Column addr set
  SPI_WRITE32(xa);

  writeCommand(ST77XX_RASET); // Row addr set
  SPI_WRITE32(ya);

  writeCommand(ST77XX_RAMWR); // write to RAM
  CS->setDigitalValue(1);

}

/**************************************************************************/
/*!
    @brief  Set origin of (0,0) and orientation of TFT display
    @param  m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Adafruit_ST77xx::setRotation(uint8_t m) {
  uint8_t madctl = 0;

  rotation = m % 4; // can't be higher than 3

  switch (rotation) {
  case 0:
    madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
    _xstart = _colstart;
    _ystart = _rowstart;
    break;
  case 1:
    madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
    _ystart = _colstart;
    _xstart = _rowstart;
    break;
  case 2:
    madctl = ST77XX_MADCTL_RGB;
    _xstart = _colstart;
    _ystart = _rowstart;
    break;
  case 3:
    madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
    _ystart = _colstart;
    _xstart = _rowstart;
    break;
  }

  sendCommand(ST77XX_MADCTL, &madctl, 1);
}

/**************************************************************************/
/*!
    @brief  Set origin of (0,0) of display with offsets
    @param  col  The offset from 0 for the column address
    @param  row  The offset from 0 for the row address
*/
/**************************************************************************/
void Adafruit_ST77xx::setColRowStart(int8_t col, int8_t row) {
  _colstart = col;
  _rowstart = row;
}

/**************************************************************************/
/*!
 @brief  Change whether display is on or off
 @param  enable True if you want the display ON, false OFF
 */
/**************************************************************************/
void Adafruit_ST77xx::enableDisplay(bool enable) {
  sendCommand(enable ? ST77XX_DISPON : ST77XX_DISPOFF);
}

/**************************************************************************/
/*!
 @brief  Change whether TE pin output is on or off
 @param  enable True if you want the TE pin ON, false OFF
 */
/**************************************************************************/
void Adafruit_ST77xx::enableTearing(bool enable) {
  sendCommand(enable ? ST77XX_TEON : ST77XX_TEOFF);
}

/**************************************************************************/
/*!
 @brief  Change whether sleep mode is on or off
 @param  enable True if you want sleep mode ON, false OFF
 */
/**************************************************************************/
void Adafruit_ST77xx::enableSleep(bool enable) {
  sendCommand(enable ? ST77XX_SLPIN : ST77XX_SLPOUT);
}

void Adafruit_ST77xx::initSPI(uint32_t freq, uint32_t mode)
{
    if (spi != NULL)
    {
      spi->setFrequency(freq);
      spi->setMode(mode);
    }
}

void Adafruit_ST77xx::writeCommand(uint8_t cmd)
{
  DC->setDigitalValue(0);
  spi->write(cmd);
  DC->setDigitalValue(1);
}  

void Adafruit_ST77xx::SPI_WRITE32(uint32_t l)
{
  spi->write(l >> 24);
  spi->write(l >> 16);
  spi->write(l >> 8);
  spi->write(l);
}

void Adafruit_ST77xx::sendCommand(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes)
{

  CS->setDigitalValue(0);
  DC->setDigitalValue(0);

  spi->write(commandByte);
  
  DC->setDigitalValue(1);

  for (int i = 0; i < numDataBytes; i++) {
    spi->write(*dataBytes); // Send the data bytes
    dataBytes++;
  }

  CS->setDigitalValue(1);
}

void Adafruit_ST77xx::sendData(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
  setAddrWindow(x, y, w, h);  

  target_wait_us(50);

  CS->setDigitalValue(0);

  target_wait_us(50);
  spi->transfer(data, w*h*2, NULL, 0);
  target_wait_us(50);
  
  CS->setDigitalValue(1);
}

# mbed_spectrogram
Simple spectrogram code for the ILI9341 display and ST Nucleo F401re board

Requires a potentiometer across +3.3V and GND, with wiper going to A0
ILI9341 SPI display on SPI channel 1, with :
```c++
  #define LCD_RESET PA_11
  #define LCD_CS PB_12 // Chip Select goes to Analog 3
  #define LCD_CD PA_12 // Command/Data goes to Analog 2
  #define LCD_WR A1 // LCD Write goes to Analog 1
  #define LCD_RD A0 // LCD Read goes to Analog 0
  #define LCD_clk D13
  #define LCD_miso D12
  #define LCD_mosi D11
```
Could be sped up with better DMA accesss to the dislay also probably by increasing SPI clock speed or going with a parallel interface to the display.

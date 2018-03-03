#include "stdio.h"
#include "mbed.h"
#include "string"
//#include "Arial12x12.h"
//#include "Arial24x23.h"
//#include "Arial43x48_numb.h"
//#include "pict.h"
//#include "pavement_48x34.h"
#include "ILI9341.h"

#define LCD_RESET PA_11
#define LCD_CS PB_12 // Chip Select goes to Analog 3
#define LCD_CD PA_12 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_clk D13
#define LCD_miso D12
#define LCD_mosi D11
#define LCD_speed 20000000 //12000000

ILI9341 myLCD(SPI_16, LCD_speed, LCD_mosi, LCD_miso, LCD_clk, LCD_CS, LCD_RESET, LCD_CD, "myLCD");
Serial pc(USBTX, USBRX);
AnalogIn aIN(A0);
DigitalOut timepin(PC_8);

unsigned short backgroundcolor = Blue;
unsigned short foregroundcolor = White;
char orient=3;

#define spec_width  100
#define spec_height  100

uint8_t col, row;
uint8_t sample;
uint8_t spectrogram[spec_width][spec_height]; // 20k bytes

void blank_spectrogram(void)
{
//fill spectrogram with zero
  for (col = 0; col <  spec_width; col++ )
  {
    for (row = 0; row < spec_height; row++)
        {
          spectrogram[col][row] = Black;
        }
  }
}

void draw_spectogram(uint8_t points[], uint8_t size)
{
  timepin = 1;
//assume size is the same as spec_height for now, so ignore it.
myLCD.FastWindow(true);
//copy columns to the left
  for (col = 0; col < spec_width-1; col++)
      {
        for (row = 0; row < spec_height; row+=2)
            {

               spectrogram[col][row] = spectrogram[col+1][row];
               
               myLCD.pixel(col,row,((0xF800 & spectrogram[col][row] << 11) | (0x7E0 & spectrogram[col][row] << 5) | (0x1F & spectrogram[col][row] << 2)));
               myLCD.pixel(col,row+1,((0xF800 & spectrogram[col][row] << 11) | (0x7E0 & spectrogram[col][row] << 5) | (0x1F & spectrogram[col][row] << 2)));
               //myLCD.pixel(col,row+1,spectrogram[col][row]);

               //myLCD.copy_to_lcd();

               // fill last column with new values
               if (col == (spec_width - 2))
               {
                 spectrogram[col][row] = points[row];
               }

            }
      }
myLCD.copy_to_lcd();
// pick rightmost column for insertion
//  col = spec_width-1;

// fill last column with new values
//  for (row = 0; row < spec_height ; row++)
//      {
//        spectrogram[col][row] = points[row];
//      }

//myLCD.FastWindow(true);

// for (col = 0; col < spec_width; col++)
//     {
//       for (row = 0; row < spec_height; row++)
//           {
//             //this will need fixing to give colours properly
//             myLCD.pixel(col,row,spectrogram[col][row]);
//           }
//     }
//
// myLCD.copy_to_lcd();
myLCD.FastWindow(false);
timepin = 0;
}

int main()
{
    uint16_t adc_val;
    uint16_t red_mask = 0xF800;
    uint16_t green_mask = 0x7E0;
    uint16_t blue_mask = 0x1F;
    uint8_t  red = 0;
    uint8_t  green = 0;
    uint8_t  blue = 0;
    uint8_t points[spec_height];
    myLCD.set_orientation(orient);
    pc.baud (115200);
    pc.printf("\n\nSystem Core Clock = %.3f MHZ\r\n",(float)SystemCoreClock/1000000);
    myLCD.cls();
    myLCD.background(backgroundcolor);    // set background to black
    myLCD.foreground(foregroundcolor);    // set chars to white
    //myLCD.width()
    //myLCD.height()
    blank_spectrogram();
    uint8_t sample_counter = 0;
    while(1)
    {
      if (sample_counter < spec_height)
      {
        adc_val = aIN.read_u16();
        red = (adc_val & red_mask) >> 11;
        green = (adc_val & green_mask) >> 5;
        blue = (adc_val & blue_mask);
        points[sample_counter] = (red << 5) | (green << 2) | blue;
        sample_counter++;
      }
      else
      {
        //timepin = 1;
        draw_spectogram(points,spec_height);
        //timepin = 0;
        //pc.printf("\n ADC Value = %d, Red = %d, Green = %d, Blue = %d, Colour = %d", adc_val,red,green,blue,points[sample_counter-1]);
        sample_counter = 0;
      }

    }
}
